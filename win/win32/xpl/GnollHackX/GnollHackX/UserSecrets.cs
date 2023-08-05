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

        public UserSecrets()
        {

        }

        public string DefaultGamePostAddress
        {
            get
            {
                return GHUtils.DecryptString(GHUtils.StrToKey(GetType()?.GetTypeInfo()?.Assembly?.FullName), EncodedDefaultGamePostAddress);
            }
        }

        public string DefaultDiagnosticDataPostAddress
        {
            get
            {
                return GHUtils.DecryptString(GHUtils.StrToKey(GetType()?.GetTypeInfo()?.Assembly?.FullName), EncodedDefaultDiagnosticDataPostAddress);
            }
        }
    }
}
