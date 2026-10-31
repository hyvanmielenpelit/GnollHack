using System;
using GnollHackM;
using Xunit;

namespace Tmds.DBus.Protocol
{
    /* Same full name as the type the filter looks for; the test project does not reference Tmds.DBus */
    internal sealed class DBusErrorReplyException : Exception
    {
        public DBusErrorReplyException(string message)
            : base(message)
        {
        }
    }
}

namespace GnollHackX.UnitTests
{
    /* Covers GHIBusDestroyReplyFilter: only the exact type and the exact message match */
    public class GHIBusDestroyReplyFilterTests
    {
        private const string DestroyReply =
            "org.freedesktop.DBus.Error.UnknownMethod: Method Destroy is not implemented on interface org.freedesktop.IBus.Service";

        private sealed class OtherException : Exception
        {
            public OtherException(string message)
                : base(message)
            {
            }
        }

        [Fact]
        public void ExactReply_Matches()
        {
            Assert.True(GHIBusDestroyReplyFilter.IsIBusDestroyReply(new Tmds.DBus.Protocol.DBusErrorReplyException(DestroyReply)));
        }

        [Fact]
        public void OtherMethod_DoesNotMatch()
        {
            string message = "org.freedesktop.DBus.Error.UnknownMethod: Method Reset is not implemented on interface org.freedesktop.IBus.InputContext";
            Assert.False(GHIBusDestroyReplyFilter.IsIBusDestroyReply(new Tmds.DBus.Protocol.DBusErrorReplyException(message)));
        }

        [Fact]
        public void OtherErrorName_DoesNotMatch()
        {
            string message = "org.freedesktop.DBus.Error.Failed: Method Destroy is not implemented on interface org.freedesktop.IBus.Service";
            Assert.False(GHIBusDestroyReplyFilter.IsIBusDestroyReply(new Tmds.DBus.Protocol.DBusErrorReplyException(message)));
        }

        [Fact]
        public void OtherType_DoesNotMatch()
        {
            Assert.False(GHIBusDestroyReplyFilter.IsIBusDestroyReply(new OtherException(DestroyReply)));
        }

        [Fact]
        public void Null_DoesNotMatch()
        {
            Assert.False(GHIBusDestroyReplyFilter.IsIBusDestroyReply(null));
        }
    }
}
