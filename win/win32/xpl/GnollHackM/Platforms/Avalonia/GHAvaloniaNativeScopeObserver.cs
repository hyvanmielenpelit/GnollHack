#if GNH_AVALONIA && LINUX && SENTRY
using System;
using System.Globalization;
using System.Text.Json;
using Sentry;

namespace GnollHackM
{
    /* Mirrors Sentry .NET's scope into sentry-native's, so that a native crash event carries the same tags, extras,
       breadcrumbs and user. Called by the SDK while it updates the scope; never throws. */
    internal sealed class GHAvaloniaNativeScopeObserver : IScopeObserver
    {
        public void AddBreadcrumb(Breadcrumb breadcrumb)
        {
            try
            {
                if (breadcrumb == null)
                    return;

                GHAvaloniaSentryNative.SentryValue crumb = GHAvaloniaSentryNative.sentry_value_new_breadcrumb(breadcrumb.Type, breadcrumb.Message);
                GHAvaloniaSentryNative.sentry_value_set_by_key(crumb, "level",
                    GHAvaloniaSentryNative.sentry_value_new_string(breadcrumb.Level.ToString().ToLowerInvariant()));
                GHAvaloniaSentryNative.sentry_value_set_by_key(crumb, "timestamp",
                    GHAvaloniaSentryNative.sentry_value_new_string(breadcrumb.Timestamp.UtcDateTime.ToString("yyyy-MM-dd'T'HH:mm:ss.fff'Z'", CultureInfo.InvariantCulture)));
                if (breadcrumb.Category != null)
                    GHAvaloniaSentryNative.sentry_value_set_by_key(crumb, "category", GHAvaloniaSentryNative.sentry_value_new_string(breadcrumb.Category));
                GHAvaloniaSentryNative.sentry_add_breadcrumb(crumb);
            }
            catch (Exception)
            {
            }
        }

        /* Strings as they are, other values as JSON */
        public void SetExtra(string key, object value)
        {
            try
            {
                if (key == null)
                    return;

                if (value == null)
                {
                    GHAvaloniaSentryNative.sentry_remove_extra(key);
                    return;
                }

                string text = value as string ?? JsonSerializer.Serialize(value);
                GHAvaloniaSentryNative.sentry_set_extra(key, GHAvaloniaSentryNative.sentry_value_new_string(text));
            }
            catch (Exception)
            {
            }
        }

        public void SetTag(string key, string value)
        {
            try
            {
                if (key == null)
                    return;

                if (value == null)
                    GHAvaloniaSentryNative.sentry_remove_tag(key);
                else
                    GHAvaloniaSentryNative.sentry_set_tag(key, value);
            }
            catch (Exception)
            {
            }
        }

        public void UnsetTag(string key)
        {
            try
            {
                if (key != null)
                    GHAvaloniaSentryNative.sentry_remove_tag(key);
            }
            catch (Exception)
            {
            }
        }

        public void SetUser(SentryUser user)
        {
            try
            {
                if (user == null)
                {
                    GHAvaloniaSentryNative.sentry_remove_user();
                    return;
                }

                GHAvaloniaSentryNative.SentryValue nativeUser = GHAvaloniaSentryNative.sentry_value_new_object();
                SetIfNotNull(nativeUser, "id", user.Id);
                SetIfNotNull(nativeUser, "username", user.Username);
                SetIfNotNull(nativeUser, "email", user.Email);
                SetIfNotNull(nativeUser, "ip_address", user.IpAddress);
                GHAvaloniaSentryNative.sentry_set_user(nativeUser);
            }
            catch (Exception)
            {
            }
        }

        public void SetEnvironment(string environment)
        {
            try
            {
                if (environment != null)
                    GHAvaloniaSentryNative.sentry_set_environment(environment);
            }
            catch (Exception)
            {
            }
        }

        /* Trace linking is not needed for crashes */
        public void SetTrace(SentryId traceId, SpanId parentSpanId)
        {
        }

        /* Managed attachments are files that a native crash cannot read safely */
        public void AddAttachment(SentryAttachment attachment)
        {
        }

        public void ClearAttachments()
        {
        }

        private static void SetIfNotNull(GHAvaloniaSentryNative.SentryValue obj, string key, string value)
        {
            if (value != null)
                GHAvaloniaSentryNative.sentry_value_set_by_key(obj, key, GHAvaloniaSentryNative.sentry_value_new_string(value));
        }
    }
}
#endif
