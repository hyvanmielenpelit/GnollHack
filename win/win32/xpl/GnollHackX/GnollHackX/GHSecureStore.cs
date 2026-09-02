using System;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;
#if GNH_MAUI
using GnollHackM;              /* GHConstants lives in the GnollHackM namespace under MAUI */
using Microsoft.Maui.Storage;
#else
using Xamarin.Essentials;
#endif

namespace GnollHackX
{
    /// <summary>
    /// Wraps the platform secure credential store -- Android Keystore-backed storage,
    /// the iOS Keychain, and Windows DPAPI -- behind a small synchronous, exception-safe
    /// key/value API.
    /// </summary>
    /// <remarks>
    /// <para>
    /// Credentials must never be written to <c>Preferences</c>, which is plaintext on every
    /// platform. This class is the single place that knows about <c>SecureStorage</c>, so no
    /// caller has to deal with its asynchronous API or its platform-specific failure modes.
    /// </para>
    /// <para>
    /// Every accessor is synchronous by design. <see cref="GHApp.Initialize"/> is a void
    /// method called from the <c>App</c> constructor, and roughly thirty consumers read the
    /// credentials synchronously -- including the <c>SettingsPage</c> constructor and the
    /// <c>SaveFileTracking</c> default, which needs the values to be loaded already. The
    /// platform calls are marshalled through <see cref="Task.Run(Action)"/>, so no
    /// synchronization context is captured and blocking from the UI thread is safe, and
    /// every wait is bounded by <see cref="GHConstants.SecureStoreTimeoutMs"/>.
    /// </para>
    /// <para>
    /// When the store is unavailable -- a wiped Android Keystore after a device restore, an
    /// iOS simulator without the Keychain entitlement, a locked-down Windows profile --
    /// <see cref="IsAvailable"/> goes false and the accessors fail softly. Callers keep the
    /// value in memory for the session and tell the player it could not be saved. They must
    /// never fall back to <c>Preferences</c>: that would defeat the purpose of the store.
    /// </para>
    /// </remarks>
    public static class GHSecureStore
    {
        private static int _unavailable = 0;
        private static string _lastError = null;

        /// <summary>
        /// False once a store operation has failed. The caller should then keep the value in
        /// memory for the session only, and must not write it to <c>Preferences</c>.
        /// </summary>
        public static bool IsAvailable
        {
            get { return Interlocked.CompareExchange(ref _unavailable, 0, 0) == 0; }
        }

        /// <summary>
        /// The message of the most recent failure, or null if none has occurred. Never
        /// contains a stored value.
        /// </summary>
        public static string LastError
        {
            get { return Interlocked.CompareExchange(ref _lastError, null, null); }
        }

        /// <summary>
        /// Reads a value from the secure store.
        /// </summary>
        /// <param name="key">The secure store key.</param>
        /// <returns>The stored value, or null when the key is absent or the read failed.</returns>
        public static string Get(string key)
        {
            if (string.IsNullOrEmpty(key))
                return null;

            try
            {
                Task<string> task = Task.Run(() => SecureStorage.GetAsync(key));
                if (!task.Wait(GHConstants.SecureStoreTimeoutMs))
                {
                    MarkFailure("Get", "timed out after " + GHConstants.SecureStoreTimeoutMs + " ms");
                    return null;
                }
                return task.Result;
            }
            catch (Exception ex)
            {
                MarkFailure("Get", ex.Message);
                return null;
            }
        }

        /// <summary>
        /// Writes a value to the secure store. A null or empty value removes the key
        /// instead, because the platform stores differ on whether an empty string
        /// round-trips.
        /// </summary>
        /// <param name="key">The secure store key.</param>
        /// <param name="value">The value to store, or null/empty to remove the key.</param>
        /// <returns>True on success; false when the store is unavailable or the write failed.</returns>
        public static bool Set(string key, string value)
        {
            if (string.IsNullOrEmpty(key))
                return false;

            if (string.IsNullOrEmpty(value))
                return Remove(key);

            try
            {
                Task task = Task.Run(() => SecureStorage.SetAsync(key, value));
                if (!task.Wait(GHConstants.SecureStoreTimeoutMs))
                {
                    MarkFailure("Set", "timed out after " + GHConstants.SecureStoreTimeoutMs + " ms");
                    return false;
                }
                return true;
            }
            catch (Exception ex)
            {
                MarkFailure("Set", ex.Message);
                return false;
            }
        }

        /// <summary>
        /// Removes a single key from the secure store.
        /// </summary>
        /// <param name="key">The secure store key.</param>
        /// <returns>True when the key was removed or was already absent; false on failure.</returns>
        public static bool Remove(string key)
        {
            if (string.IsNullOrEmpty(key))
                return false;

            try
            {
                SecureStorage.Remove(key);
                return true;
            }
            catch (Exception ex)
            {
                MarkFailure("Remove", ex.Message);
                return false;
            }
        }

        /// <summary>
        /// Removes every key this application has stored in the secure store. Used by the
        /// factory reset on <c>ResetPage</c>, which clears <c>Preferences</c> alongside it.
        /// </summary>
        /// <returns>True on success; false on failure.</returns>
        public static bool RemoveAll()
        {
            try
            {
                SecureStorage.RemoveAll();
                return true;
            }
            catch (Exception ex)
            {
                MarkFailure("RemoveAll", ex.Message);
                return false;
            }
        }

        /// <summary>
        /// Records a failure and marks the store unavailable. Deliberately logs the failing
        /// operation and message only -- never a key's value.
        /// </summary>
        private static void MarkFailure(string operation, string message)
        {
            Interlocked.Exchange(ref _unavailable, 1);
            Interlocked.Exchange(ref _lastError, message);

            string logtext = "GHSecureStore: " + operation + " failed: " + message;
            Debug.WriteLine(logtext);
            try
            {
                GHApp.MaybeWriteGHLog(logtext);
            }
            catch (Exception)
            {
                /* Logging must never break startup; the Debug line above is enough. */
            }
        }
    }
}
