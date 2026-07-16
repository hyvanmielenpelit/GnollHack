using System;
using System.Collections.Generic;
using System.Reflection;
using System.Text;

namespace GnollHackX
{
    public class UserSecrets
    {
        public string EncodedDefaultGamePostAddress = "";
        public string EncodedDefaultDiagnosticDataPostAddress = "";
        public string EncodedDefaultXlogRegisterationAddress = "";
        public string EncodedDefaultXlogPostAddress = "";
        public string EncodedDefaultXlogAntiForgeryToken = "";
        public string EncodedDefaultAzureBlobStorageConnectionString = "";
        public string EncodedDefaultSentryDSN = "";

        public UserSecrets()
        {

        }

        public string DefaultGamePostAddress => GHUtils.DecryptString(GHUtils.StrToKey(GetType()?.GetTypeInfo()?.Assembly?.FullName), EncodedDefaultGamePostAddress);

        public string DefaultDiagnosticDataPostAddress => GHUtils.DecryptString(GHUtils.StrToKey(GetType()?.GetTypeInfo()?.Assembly?.FullName), EncodedDefaultDiagnosticDataPostAddress);

        public string DefaultXlogAccountLink => GHUtils.DecryptString(GHUtils.StrToKey(GetType()?.GetTypeInfo()?.Assembly?.FullName), EncodedDefaultXlogRegisterationAddress);

        public string DefaultXlogPostAddress => GHUtils.DecryptString(GHUtils.StrToKey(GetType()?.GetTypeInfo()?.Assembly?.FullName), EncodedDefaultXlogPostAddress);

        public string DefaultXlogAntiForgeryToken => GHUtils.DecryptString(GHUtils.StrToKey(GetType()?.GetTypeInfo()?.Assembly?.FullName), EncodedDefaultXlogAntiForgeryToken);

        public string DefaultAzureBlobStorageConnectionString => GHUtils.DecryptString(GHUtils.StrToKey(GetType()?.GetTypeInfo()?.Assembly?.FullName), EncodedDefaultAzureBlobStorageConnectionString);
        public string DefaultSentryDNS => GHUtils.DecryptString(GHUtils.StrToKey(GetType()?.GetTypeInfo()?.Assembly?.FullName), EncodedDefaultSentryDSN);
    }
}
