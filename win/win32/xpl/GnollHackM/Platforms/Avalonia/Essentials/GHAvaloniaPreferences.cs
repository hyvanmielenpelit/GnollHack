using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Threading;
using Microsoft.Maui.Storage;

using PreferencesDictionary =
    System.Collections.Generic.Dictionary<string, System.Collections.Generic.Dictionary<string, string>>;

namespace GnollHackM
{
    /* IPreferences kept in memory and written behind to preferences.json in the data directory.
       Values are strings formatted with the invariant culture (DateTime as ToBinary(), DateTimeOffset
       as round-trip "O"), keyed by shared name, as in MAUI's unpackaged Windows implementation.
       Changes are flushed 400 ms after the last one, and on process exit; a flush writes a temporary
       file and moves it over the old one. An unreadable file is kept as preferences.json.corrupt. */
    public sealed class GHAvaloniaPreferences : IPreferences
    {
        private const string FileName = "preferences.json";
        private const int FlushDelayMilliseconds = 400;
        private const int RetryDelayMilliseconds = 5000;

        /* MAUI's Preferences.SupportedTypes, which is internal */
        private static readonly Type[] _supportedTypes =
        {
            typeof(string),
            typeof(int),
            typeof(bool),
            typeof(long),
            typeof(double),
            typeof(float),
            typeof(DateTime),
            typeof(DateTimeOffset)
        };

        private readonly object _lock = new object();
        private readonly object _flushLock = new object();
        private readonly PreferencesDictionary _preferences = new PreferencesDictionary(StringComparer.Ordinal);
        private readonly string _filePath;
        private readonly System.Threading.Timer _flushTimer;
        private bool _dirty = false;

        public GHAvaloniaPreferences()
        {
            _filePath = Path.Combine(GHAvaloniaPaths.DataDirectory, FileName);
            Load();
            _flushTimer = new System.Threading.Timer(OnFlushTimer, null, Timeout.Infinite, Timeout.Infinite);
            AppDomain.CurrentDomain.ProcessExit += OnProcessExit;
        }

        public bool ContainsKey(string key, string sharedName = null)
        {
            lock (_lock)
            {
                Dictionary<string, string> inner;
                if (_preferences.TryGetValue(CleanSharedName(sharedName), out inner))
                    return inner.ContainsKey(key);
                return false;
            }
        }

        public void Remove(string key, string sharedName = null)
        {
            lock (_lock)
            {
                Dictionary<string, string> inner;
                if (_preferences.TryGetValue(CleanSharedName(sharedName), out inner) && inner.Remove(key))
                    MarkDirty();
            }
        }

        public void Clear(string sharedName = null)
        {
            lock (_lock)
            {
                Dictionary<string, string> inner;
                if (_preferences.TryGetValue(CleanSharedName(sharedName), out inner) && inner.Count > 0)
                {
                    inner.Clear();
                    MarkDirty();
                }
            }
        }

        public void Set<T>(string key, T value, string sharedName = null)
        {
            CheckIsSupportedType<T>();

            string formatted;
            if (value == null)
                formatted = null;
            else if (value is DateTime dt)
                formatted = string.Format(CultureInfo.InvariantCulture, "{0}", dt.ToBinary());
            else if (value is DateTimeOffset dto)
                formatted = dto.ToString("O", CultureInfo.InvariantCulture);
            else
                formatted = string.Format(CultureInfo.InvariantCulture, "{0}", value);

            lock (_lock)
            {
                string name = CleanSharedName(sharedName);
                Dictionary<string, string> inner;
                if (!_preferences.TryGetValue(name, out inner))
                {
                    inner = new Dictionary<string, string>(StringComparer.Ordinal);
                    _preferences[name] = inner;
                }

                if (formatted == null)
                {
                    if (inner.Remove(key))
                        MarkDirty();
                }
                else
                {
                    string existing;
                    if (!inner.TryGetValue(key, out existing)
                        || !string.Equals(existing, formatted, StringComparison.Ordinal))
                    {
                        inner[key] = formatted;
                        MarkDirty();
                    }
                }
            }
        }

        public T Get<T>(string key, T defaultValue, string sharedName = null)
        {
            lock (_lock)
            {
                Dictionary<string, string> inner;
                string value;
                if (!_preferences.TryGetValue(CleanSharedName(sharedName), out inner)
                    || !inner.TryGetValue(key, out value) || value == null)
                    return defaultValue;

                if (defaultValue is DateTime)
                {
                    long binary;
                    if (long.TryParse(value, NumberStyles.Integer, CultureInfo.InvariantCulture, out binary))
                        return (T)(object)DateTime.FromBinary(binary);
                    DateTime parsedDateTime;
                    if (DateTime.TryParse(value, CultureInfo.InvariantCulture, DateTimeStyles.None, out parsedDateTime))
                        return (T)(object)parsedDateTime;
                }
                else if (defaultValue is DateTimeOffset)
                {
                    DateTimeOffset parsedDateTimeOffset;
                    if (DateTimeOffset.TryParse(value, CultureInfo.InvariantCulture, DateTimeStyles.None,
                        out parsedDateTimeOffset))
                        return (T)(object)parsedDateTimeOffset;
                }

                try
                {
                    return (T)Convert.ChangeType(value, typeof(T), CultureInfo.InvariantCulture);
                }
                catch (FormatException)
                {
                    return defaultValue;
                }
                catch (InvalidCastException)
                {
                    return defaultValue;
                }
                catch (OverflowException)
                {
                    return defaultValue;
                }
            }
        }

        /* Writes pending changes now; safe from any thread */
        public void Flush()
        {
            lock (_flushLock)
            {
                byte[] bytes;
                lock (_lock)
                {
                    if (!_dirty)
                        return;
                    bytes = JsonSerializer.SerializeToUtf8Bytes(_preferences,
                        GHAvaloniaPreferencesJsonContext.Default.PreferencesDictionary);
                    _dirty = false;
                }

                try
                {
                    WriteAtomically(bytes);
                }
                catch (Exception ex)
                {
                    Debug.WriteLine("GHAvaloniaPreferences: writing " + _filePath + " failed: " + ex.Message);
                    lock (_lock)
                    {
                        _dirty = true;
                        _flushTimer.Change(RetryDelayMilliseconds, Timeout.Infinite);
                    }
                }
            }
        }

        /* Called with _lock held */
        private void MarkDirty()
        {
            _dirty = true;
            _flushTimer.Change(FlushDelayMilliseconds, Timeout.Infinite);
        }

        private void OnFlushTimer(object state)
        {
            try
            {
                Flush();
            }
            catch (Exception ex)
            {
                Debug.WriteLine("GHAvaloniaPreferences: flush failed: " + ex.Message);
            }
        }

        private void OnProcessExit(object sender, EventArgs e)
        {
            try
            {
                Flush();
            }
            catch (Exception ex)
            {
                Debug.WriteLine("GHAvaloniaPreferences: flush on exit failed: " + ex.Message);
            }
        }

        private void WriteAtomically(byte[] bytes)
        {
            string directory = Path.GetDirectoryName(_filePath);
            if (!string.IsNullOrEmpty(directory))
                Directory.CreateDirectory(directory);

            string tempPath = _filePath + ".tmp";
            using (FileStream stream = new FileStream(tempPath, FileMode.Create, FileAccess.Write, FileShare.None))
            {
                stream.Write(bytes, 0, bytes.Length);
                stream.Flush(true);
            }
            File.Move(tempPath, _filePath, true);
        }

        private void Load()
        {
            if (!File.Exists(_filePath))
                return;

            try
            {
                byte[] bytes = File.ReadAllBytes(_filePath);
                PreferencesDictionary loaded = JsonSerializer.Deserialize(bytes,
                    GHAvaloniaPreferencesJsonContext.Default.PreferencesDictionary);
                if (loaded == null)
                    throw new JsonException("The preferences file holds no object.");

                foreach (KeyValuePair<string, Dictionary<string, string>> pair in loaded)
                {
                    if (pair.Key == null || pair.Value == null)
                        continue;
                    Dictionary<string, string> inner = new Dictionary<string, string>(StringComparer.Ordinal);
                    foreach (KeyValuePair<string, string> entry in pair.Value)
                    {
                        if (entry.Key != null && entry.Value != null)
                            inner[entry.Key] = entry.Value;
                    }
                    _preferences[pair.Key] = inner;
                }
            }
            catch (Exception ex)
            {
                _preferences.Clear();
                PreserveCorruptFile(ex);
            }
        }

        private void PreserveCorruptFile(Exception reason)
        {
            string corruptPath = _filePath + ".corrupt";
            Debug.WriteLine("GHAvaloniaPreferences: " + _filePath + " is unreadable (" + reason.Message
                + "); keeping it as " + corruptPath);
            try
            {
                File.Move(_filePath, corruptPath, true);
            }
            catch (Exception ex)
            {
                Debug.WriteLine("GHAvaloniaPreferences: renaming the unreadable file failed: " + ex.Message);
            }
        }

        private static void CheckIsSupportedType<T>()
        {
            Type type = typeof(T);
            if (Array.IndexOf(_supportedTypes, type) < 0)
                throw new NotSupportedException($"Preferences using '{type}' type is not supported");
        }

        private static string CleanSharedName(string sharedName)
        {
            return string.IsNullOrEmpty(sharedName) ? string.Empty : sharedName;
        }
    }

    [JsonSourceGenerationOptions(WriteIndented = true)]
    [JsonSerializable(typeof(PreferencesDictionary), TypeInfoPropertyName = "PreferencesDictionary")]
    internal partial class GHAvaloniaPreferencesJsonContext : JsonSerializerContext
    {
    }
}
