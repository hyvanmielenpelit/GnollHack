using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Security.Cryptography;
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Threading.Tasks;
using Microsoft.Maui.Storage;
#if WINDOWS_AVALONIA
using System.Runtime.InteropServices;
#endif

namespace GnollHackM
{
    /* ISecureStorage over securestore.bin in the data directory: one encrypted JSON object of all keys
       and values.
       Linux and macOS: AES-GCM with a key derived with HKDF-SHA256 from the machine id (/etc/machine-id on
       Linux, the hardware UUID on macOS) and a fixed application salt, so this is obfuscation-level
       protection only: any process of the same user can derive the same key. The file is created with
       mode 0600. File layout: "GHS1", 12-byte nonce, 16-byte tag, ciphertext.
       Windows: DPAPI (CryptProtectData) for the current user, with a fixed application entropy. File
       layout: "GHS2", the DPAPI blob. A "GHS1" file, encrypted as above with a key derived from the
       machine name, is read and rewritten as "GHS2".
       A file that cannot be decrypted or parsed, for example after the machine id changed or the user's
       DPAPI master key was lost, is kept as securestore.bin.corrupt and the store starts empty. */
    public sealed class GHAvaloniaSecureStorage : ISecureStorage
    {
        private const string FileName = "securestore.bin";
        private const int NonceSize = 12;
        private const int TagSize = 16;
        private const int KeySize = 32;
        private static readonly byte[] _fileMagic = { (byte)'G', (byte)'H', (byte)'S', (byte)'1' };
        private static readonly byte[] _keySalt = Encoding.UTF8.GetBytes("GnollHack.Avalonia.SecureStorage.v1");
        private static readonly byte[] _keyInfo = Encoding.UTF8.GetBytes(FileName);
#if WINDOWS_AVALONIA
        private static readonly byte[] _windowsFileMagic = { (byte)'G', (byte)'H', (byte)'S', (byte)'2' };
        private static readonly byte[] _windowsEntropy = Encoding.UTF8.GetBytes("GnollHack.Avalonia.SecureStorage.v2");
#endif

        private readonly object _lock = new object();

        /* Loaded on first use, under _lock */
        private Dictionary<string, string> _values = null;
        private string _filePath = null;
        private byte[] _key = null;

        public Task<string> GetAsync(string key)
        {
            if (string.IsNullOrWhiteSpace(key))
                throw new ArgumentNullException(nameof(key));

            lock (_lock)
            {
                EnsureLoaded();
                string value;
                return Task.FromResult(_values.TryGetValue(key, out value) ? value : null);
            }
        }

        public Task SetAsync(string key, string value)
        {
            if (string.IsNullOrWhiteSpace(key))
                throw new ArgumentNullException(nameof(key));
            if (value == null)
                throw new ArgumentNullException(nameof(value));

            lock (_lock)
            {
                try
                {
                    EnsureLoaded();
                    string previous;
                    bool hadPrevious = _values.TryGetValue(key, out previous);
                    if (hadPrevious && string.Equals(previous, value, StringComparison.Ordinal))
                        return Task.CompletedTask;

                    _values[key] = value;
                    try
                    {
                        Save();
                    }
                    catch
                    {
                        if (hadPrevious)
                            _values[key] = previous;
                        else
                            _values.Remove(key);
                        throw;
                    }
                    return Task.CompletedTask;
                }
                catch (Exception ex)
                {
                    return Task.FromException(ex);
                }
            }
        }

        public bool Remove(string key)
        {
            if (string.IsNullOrWhiteSpace(key))
                return false;

            lock (_lock)
            {
                EnsureLoaded();
                string previous;
                if (!_values.TryGetValue(key, out previous))
                    return false;

                _values.Remove(key);
                try
                {
                    Save();
                }
                catch
                {
                    _values[key] = previous;
                    throw;
                }
                return true;
            }
        }

        public void RemoveAll()
        {
            lock (_lock)
            {
                EnsureLoaded();
                _values.Clear();
                if (File.Exists(_filePath))
                    File.Delete(_filePath);
                string tempPath = _filePath + ".tmp";
                if (File.Exists(tempPath))
                    File.Delete(tempPath);
            }
        }

        /* Called with _lock held */
        private void EnsureLoaded()
        {
            if (_values != null)
                return;

            _filePath = Path.Combine(GHAvaloniaPaths.DataDirectory, FileName);
            _key = DeriveKey();
            Dictionary<string, string> values = new Dictionary<string, string>(StringComparer.Ordinal);
            bool rewrite = false;

            if (File.Exists(_filePath))
            {
                try
                {
                    byte[] plaintext = ReadStore(File.ReadAllBytes(_filePath), out rewrite);
                    try
                    {
                        Dictionary<string, string> loaded = JsonSerializer.Deserialize(plaintext,
                            GHAvaloniaSecureStorageJsonContext.Default.SecureStoreDictionary);
                        if (loaded == null)
                            throw new JsonException("The secure store holds no object.");
                        foreach (KeyValuePair<string, string> pair in loaded)
                        {
                            if (pair.Key != null && pair.Value != null)
                                values[pair.Key] = pair.Value;
                        }
                    }
                    finally
                    {
                        CryptographicOperations.ZeroMemory(plaintext);
                    }
                }
                catch (Exception ex)
                {
                    values.Clear();
                    rewrite = false;
                    PreserveCorruptFile(ex);
                }
            }

            _values = values;
            if (rewrite)
            {
                try
                {
                    Save();
                }
                catch (Exception ex)
                {
                    /* The old format stays readable; the next save writes the current one */
                    Debug.WriteLine("GHAvaloniaSecureStorage: rewriting " + _filePath + " failed: " + ex.Message);
                }
            }
        }

        /* The serialized store from the file's contents; rewrite is true when the file is in an older format.
           Windows reads "GHS2", and "GHS1" for rewriting; elsewhere "GHS1". */
        private byte[] ReadStore(byte[] data, out bool rewrite)
        {
#if WINDOWS_AVALONIA
            if (HasMagic(data, _windowsFileMagic))
            {
                rewrite = false;
                return Unprotect(data);
            }
            rewrite = true;
#else
            rewrite = false;
#endif
            return Decrypt(data);
        }

        /* Called with _lock held */
        private void Save()
        {
            byte[] plaintext = JsonSerializer.SerializeToUtf8Bytes(_values,
                GHAvaloniaSecureStorageJsonContext.Default.SecureStoreDictionary);
            byte[] encrypted;
            try
            {
                encrypted = Encrypt(plaintext);
            }
            finally
            {
                CryptographicOperations.ZeroMemory(plaintext);
            }

            string directory = Path.GetDirectoryName(_filePath);
            if (!string.IsNullOrEmpty(directory))
                Directory.CreateDirectory(directory);

            string tempPath = _filePath + ".tmp";
            if (File.Exists(tempPath))
                File.Delete(tempPath);

            FileStreamOptions options = new FileStreamOptions
            {
                Mode = FileMode.CreateNew,
                Access = FileAccess.Write,
                Share = FileShare.None,
            };
#if LINUX || MACOS
            if (!OperatingSystem.IsWindows())
                options.UnixCreateMode = UnixFileMode.UserRead | UnixFileMode.UserWrite;
#endif
            using (FileStream stream = new FileStream(tempPath, options))
            {
                stream.Write(encrypted, 0, encrypted.Length);
                stream.Flush(true);
            }
            File.Move(tempPath, _filePath, true);
        }

        private byte[] Encrypt(byte[] plaintext)
        {
#if WINDOWS_AVALONIA
            return Protect(plaintext);
#else
            byte[] result = new byte[_fileMagic.Length + NonceSize + TagSize + plaintext.Length];
            Span<byte> output = result;
            _fileMagic.CopyTo(output);
            Span<byte> nonce = output.Slice(_fileMagic.Length, NonceSize);
            Span<byte> tag = output.Slice(_fileMagic.Length + NonceSize, TagSize);
            Span<byte> ciphertext = output.Slice(_fileMagic.Length + NonceSize + TagSize);
            RandomNumberGenerator.Fill(nonce);
            using (AesGcm aes = new AesGcm(_key, TagSize))
            {
                aes.Encrypt(nonce, plaintext, ciphertext, tag, _fileMagic);
            }
            return result;
#endif
        }

        private byte[] Decrypt(byte[] data)
        {
            int headerSize = _fileMagic.Length + NonceSize + TagSize;
            if (data == null || data.Length < headerSize)
                throw new CryptographicException("The secure store file is too short.");

            ReadOnlySpan<byte> input = data;
            if (!input.Slice(0, _fileMagic.Length).SequenceEqual(_fileMagic))
                throw new CryptographicException("The secure store file has an unknown format.");

            ReadOnlySpan<byte> nonce = input.Slice(_fileMagic.Length, NonceSize);
            ReadOnlySpan<byte> tag = input.Slice(_fileMagic.Length + NonceSize, TagSize);
            ReadOnlySpan<byte> ciphertext = input.Slice(headerSize);
            byte[] plaintext = new byte[ciphertext.Length];
            using (AesGcm aes = new AesGcm(_key, TagSize))
            {
                aes.Decrypt(nonce, ciphertext, tag, plaintext, _fileMagic);
            }
            return plaintext;
        }

        private void PreserveCorruptFile(Exception reason)
        {
            string corruptPath = _filePath + ".corrupt";
            Debug.WriteLine("GHAvaloniaSecureStorage: " + _filePath + " is unreadable (" + reason.Message
                + "); keeping it as " + corruptPath);
            try
            {
                File.Move(_filePath, corruptPath, true);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("GHAvaloniaSecureStorage: renaming the unreadable file failed: " + ex.Message);
            }
        }

        private static byte[] DeriveKey()
        {
            byte[] inputKeyMaterial = Encoding.UTF8.GetBytes(ReadMachineId());
            return HKDF.DeriveKey(HashAlgorithmName.SHA256, inputKeyMaterial, KeySize, _keySalt, _keyInfo);
        }

        private static string ReadMachineId()
        {
#if LINUX
            string[] candidates = { "/etc/machine-id", "/var/lib/dbus/machine-id" };
            foreach (string path in candidates)
            {
                try
                {
                    if (!File.Exists(path))
                        continue;
                    string id = File.ReadAllText(path).Trim();
                    if (id.Length > 0)
                        return id;
                }
                catch (Exception)
                {
                    continue;
                }
            }
#elif MACOS
            try
            {
                byte[] uuid = new byte[16];
                Timespec wait = new Timespec();
                wait.tv_sec = 1;
                if (gethostuuid(uuid, ref wait) == 0)
                    return FormatUuid(uuid);
            }
            catch (Exception)
            {
            }
#endif
            try
            {
                string name = Environment.MachineName;
                if (!string.IsNullOrWhiteSpace(name))
                    return name;
            }
            catch (InvalidOperationException)
            {
            }
            return "GnollHack";
        }

#if WINDOWS_AVALONIA
        private const int CRYPTPROTECT_UI_FORBIDDEN = 0x1;

        [StructLayout(LayoutKind.Sequential)]
        private struct DataBlob
        {
            public int cbData;
            public IntPtr pbData;
        }

        [DllImport("crypt32.dll", CharSet = CharSet.Unicode, ExactSpelling = true, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool CryptProtectData(ref DataBlob pDataIn, string szDataDescr,
            ref DataBlob pOptionalEntropy, IntPtr pvReserved, IntPtr pPromptStruct, int dwFlags, out DataBlob pDataOut);

        [DllImport("crypt32.dll", ExactSpelling = true, SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool CryptUnprotectData(ref DataBlob pDataIn, IntPtr ppszDataDescr,
            ref DataBlob pOptionalEntropy, IntPtr pvReserved, IntPtr pPromptStruct, int dwFlags, out DataBlob pDataOut);

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern IntPtr LocalFree(IntPtr hMem);

        private static bool HasMagic(byte[] data, byte[] magic)
        {
            return data != null && data.Length >= magic.Length && data.AsSpan(0, magic.Length).SequenceEqual(magic);
        }

        /* "GHS2" and the DPAPI blob of the plaintext, for the current user */
        private static byte[] Protect(byte[] plaintext)
        {
            byte[] blob = RunDpapi(plaintext, 0, plaintext.Length, true);
            byte[] result = new byte[_windowsFileMagic.Length + blob.Length];
            _windowsFileMagic.CopyTo(result, 0);
            blob.CopyTo(result, _windowsFileMagic.Length);
            return result;
        }

        /* The plaintext of a "GHS2" file; throws CryptographicException when DPAPI cannot decrypt it */
        private static byte[] Unprotect(byte[] data)
        {
            return RunDpapi(data, _windowsFileMagic.Length, data.Length - _windowsFileMagic.Length, false);
        }

        /* CryptProtectData or CryptUnprotectData on count bytes of input from offset, never showing any UI. The
           output is copied into a managed array, cleared when it is plaintext, and freed with LocalFree. */
        private static byte[] RunDpapi(byte[] input, int offset, int count, bool protect)
        {
            if (count <= 0)
                throw new CryptographicException("The secure store holds no protected data.");

            GCHandle inputHandle = GCHandle.Alloc(input, GCHandleType.Pinned);
            GCHandle entropyHandle = GCHandle.Alloc(_windowsEntropy, GCHandleType.Pinned);
            DataBlob output = new DataBlob();
            try
            {
                DataBlob inputBlob = new DataBlob();
                inputBlob.cbData = count;
                inputBlob.pbData = inputHandle.AddrOfPinnedObject() + offset;
                DataBlob entropyBlob = new DataBlob();
                entropyBlob.cbData = _windowsEntropy.Length;
                entropyBlob.pbData = entropyHandle.AddrOfPinnedObject();

                bool ok;
                if (protect)
                    ok = CryptProtectData(ref inputBlob, null, ref entropyBlob, IntPtr.Zero, IntPtr.Zero,
                        CRYPTPROTECT_UI_FORBIDDEN, out output);
                else
                    ok = CryptUnprotectData(ref inputBlob, IntPtr.Zero, ref entropyBlob, IntPtr.Zero, IntPtr.Zero,
                        CRYPTPROTECT_UI_FORBIDDEN, out output);
                if (!ok)
                    throw new CryptographicException(Marshal.GetLastWin32Error());
                if (output.pbData == IntPtr.Zero || output.cbData < 0)
                    throw new CryptographicException("DPAPI returned no data.");

                byte[] result = new byte[output.cbData];
                Marshal.Copy(output.pbData, result, 0, output.cbData);
                return result;
            }
            finally
            {
                if (output.pbData != IntPtr.Zero)
                {
                    if (!protect && output.cbData > 0)
                        Marshal.Copy(new byte[output.cbData], 0, output.pbData, output.cbData);
                    LocalFree(output.pbData);
                }
                entropyHandle.Free();
                inputHandle.Free();
            }
        }
#endif

#if MACOS
        [System.Runtime.InteropServices.StructLayout(System.Runtime.InteropServices.LayoutKind.Sequential)]
        private struct Timespec
        {
            public long tv_sec;
            public long tv_nsec;
        }

        [System.Runtime.InteropServices.DllImport("libSystem.dylib")]
        private static extern int gethostuuid(byte[] id, ref Timespec wait);

        /* In the byte order of uuid_unparse_upper */
        private static string FormatUuid(byte[] uuid)
        {
            StringBuilder sb = new StringBuilder(36);
            int i;
            for (i = 0; i < uuid.Length; i++)
            {
                if (i == 4 || i == 6 || i == 8 || i == 10)
                    sb.Append('-');
                sb.Append(uuid[i].ToString("X2"));
            }
            return sb.ToString();
        }
#endif
    }

    [JsonSerializable(typeof(Dictionary<string, string>), TypeInfoPropertyName = "SecureStoreDictionary")]
    internal partial class GHAvaloniaSecureStorageJsonContext : JsonSerializerContext
    {
    }
}
