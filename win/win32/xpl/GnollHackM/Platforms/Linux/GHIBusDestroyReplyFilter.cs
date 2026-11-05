using System;

namespace GnollHackM
{
    // WORKAROUND(WA-AVALONIA-0023): IBus's Destroy reply reaches Sentry as an unobserved exception
    /* Recognizes the reply the IBus portal gives to every Destroy call, once the input context is already destroyed.
       The type is compared by name, as Tmds.DBus.Protocol is only a transitive dependency of the Linux build. Pure
       .NET, so the unit tests can link it. */
    internal static class GHIBusDestroyReplyFilter
    {
        private const string ReplyTypeName = "Tmds.DBus.Protocol.DBusErrorReplyException";
        private const string ReplyMessage = "org.freedesktop.DBus.Error.UnknownMethod: "
            + "Method Destroy is not implemented on interface org.freedesktop.IBus.Service";

        public static bool IsIBusDestroyReply(Exception exception)
        {
            return exception != null
                && exception.GetType().FullName == ReplyTypeName
                && exception.Message == ReplyMessage;
        }
    }
}
