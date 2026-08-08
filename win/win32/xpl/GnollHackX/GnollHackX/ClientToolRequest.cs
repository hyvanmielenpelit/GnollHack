using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace GnollHackX
{
    /// <summary>
    /// Model class for deserializing tool request JSON messages from the Angular SPA
    /// via the JS bridge. The server sends these as SignalR "tool_client_request" messages,
    /// which the Angular SPA forwards to the native MAUI client.
    /// </summary>
    public class ClientToolRequest
    {
        [JsonProperty("type")]
        public string Type { get; set; }

        [JsonProperty("requestId")]
        public string RequestId { get; set; }

        [JsonProperty("toolName")]
        public string ToolName { get; set; }

        [JsonProperty("parameters")]
        public JObject Parameters { get; set; }
    }
}
