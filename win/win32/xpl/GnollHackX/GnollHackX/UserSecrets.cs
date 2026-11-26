using System;
using System.Collections.Generic;
using System.Reflection;
using System.Text;

namespace GnollHackX
{
    public class UserSecrets
    {
        public string EncodedDefaultXlogRegisterationAddress = "";
        public string EncodedDefaultXlogPostAddress = "";
        public string EncodedDefaultXlogAntiForgeryToken = "";
        public string EncodedDefaultSentryDSN = "";

        public UserSecrets()
        {

        }

        public string DefaultXlogAccountLink
        {
            get
            {
                return GHUtils.DecryptString(GHUtils.StrToKey(GetType()?.GetTypeInfo()?.Assembly?.FullName), EncodedDefaultXlogRegisterationAddress);
            }
        }

        public string DefaultXlogPostAddress
        {
            get
            {
                return GHUtils.DecryptString(GHUtils.StrToKey(GetType()?.GetTypeInfo()?.Assembly?.FullName), EncodedDefaultXlogPostAddress);
            }
        }

        public string DefaultXlogAntiForgeryToken
        {
            get
            {
                return GHUtils.DecryptString(GHUtils.StrToKey(GetType()?.GetTypeInfo()?.Assembly?.FullName), EncodedDefaultXlogAntiForgeryToken);
            }
        }

        public string DefaultSentryDNS
        {
            get
            {
                return GHUtils.DecryptString(GHUtils.StrToKey(GetType()?.GetTypeInfo()?.Assembly?.FullName), EncodedDefaultSentryDSN);
            }
        }
    }
}
