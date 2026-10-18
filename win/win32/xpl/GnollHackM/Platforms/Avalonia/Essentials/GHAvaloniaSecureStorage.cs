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

namespace GnollHackM
{
    /* ISecureStorage over securestore.bin in the data directory: one AES-GCM encrypted JSON object
       of all keys and values. The key is derived with HKDF-SHA256 from the machine id
       (/etc/machine-id on Linux, the machine name elsewhere) and a fixed application salt, so this is
       obfuscation-level protection only: any process of the same user can derive the same key. On
       Linux the file is created with mode 0600. A file that cannot be decrypted or parsed, for
       example after the machine id changed, is kept as securestore.bin.corrupt and the store starts
       empty. File layout: "GHS1", 12-byte nonce, 16-byte tag, ciphertext. */
    public sealed class GHAvaloniaSecureStorage : ISecureStorage
    {
        private const string FileName = "securestore.bin";
        private const int NonceSize = 12;
        private const int TagSize = 16;
        private const int KeySize = 32;
        private static readonly byte[] _fileMagic = { (byte)'G', (byte)'H', (byte)'S', (byte)'1' };
        private static readonly byte[] _keySalt = Encoding.UTF8.GetBytes("GnollHack.Avalonia.SecureStorage.v1");
        private static readonly byte[] _keyInfo = Encoding.UTF8.GetBytes(FileName);

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

            if (File.Exists(_filePath))
            {
                try
                {
                    byte[] plaintext = Decrypt(File.ReadAllBytes(_filePath));
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
                    PreserveCorruptFile(ex);
                }
            }

            _values = values;
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
#if LINUX
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
    }

    [JsonSerializable(typeof(Dictionary<string, string>), TypeInfoPropertyName = "SecureStoreDictionary")]
    internal partial class GHAvaloniaSecureStorageJsonContext : JsonSerializerContext
    {
    }
}
