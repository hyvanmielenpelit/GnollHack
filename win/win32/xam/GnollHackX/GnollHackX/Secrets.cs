using System;
using System.Collections.Generic;
using System.Net.NetworkInformation;
using System.Text;

namespace GnollHackX
{
    public class Secrets
    {
        public SecretsDirectory[] directories = null;
        public SecretsFile[] files = null;
        public string EncodedDefaultGamePostAddress = "";
        public string EncodedDefaultDiagnosticDataPostAddress = "";

        public Secrets()
        {

        }

        public string DefaultGamePostAddress { get { return Encoding.UTF8.GetString(Convert.FromBase64String(EncodedDefaultGamePostAddress)); } }
        public string DefaultDiagnosticDataPostAddress { get { return Encoding.UTF8.GetString(Convert.FromBase64String(EncodedDefaultDiagnosticDataPostAddress)); } }

    }
}
