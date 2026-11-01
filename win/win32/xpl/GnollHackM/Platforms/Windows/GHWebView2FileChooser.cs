using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using GnollHackX;
using Microsoft.Maui.Storage;
using Microsoft.Web.WebView2.Core;
using Newtonsoft.Json.Linq;

namespace GnollHackM
{
    // WORKAROUND(WA-WINUI-0009): WebView2 has no public API to replace its file dialog
    /* Attach and downloads on a page while the controller is the active input. PrepareAsync, awaited before each A press's
       activation, turns on the DevTools protocol's file chooser interception and grants a user gesture, so that a press on an
       attach button opens FileDialogView instead of WebView2's dialog, which a controller cannot drive. Interception goes off
       when the controller stops being active, so that a mouse click gets Windows' own dialog, and on Dispose, because the next
       instance on the same CoreWebView2 starts knowing nothing of it. Downloads go through the in-app save dialog while it
       is in use, and are deleted from the cache afterward. One instance per CoreWebView2. UI thread only. */
    internal sealed class GHWebView2FileChooser : IDisposable
    {
        private const string InterceptOn = "{\"enabled\":true}";
        private const string InterceptOff = "{\"enabled\":false}";
        private const string GestureScript = "{\"expression\":\"0\",\"userGesture\":true}";
        private const int MaxAttachedFiles = 5;
        private const int PrepareBoundMilliseconds = 1000;

        private readonly CoreWebView2 _core;
        private readonly CoreWebView2DevToolsProtocolEventReceiver _receiver;
        private bool _intercepting;
        private bool _disposed;

        public GHWebView2FileChooser(CoreWebView2 core)
        {
            _core = core;
            _receiver = core.GetDevToolsProtocolEventReceiver("Page.fileChooserOpened");
            _receiver.DevToolsProtocolEventReceived += Receiver_DevToolsProtocolEventReceived;
            _core.DownloadStarting += Core_DownloadStarting;
            GHGamepadService.StateChanged += GamepadService_StateChanged;
        }

        /* All three calls go out on every press: they cost little and survive any reset of the page's session. Never
           faults, and returns within 1 s. */
        public async Task PrepareAsync()
        {
            if (_disposed || !GHGamepadService.IsActive)
                return;
            await Task.WhenAny(PrepareCoreAsync(), Task.Delay(PrepareBoundMilliseconds));
        }

        public void Dispose()
        {
            if (_disposed)
                return;
            _disposed = true;
            GHGamepadService.StateChanged -= GamepadService_StateChanged;
            try
            {
                _receiver.DevToolsProtocolEventReceived -= Receiver_DevToolsProtocolEventReceived;
                _core.DownloadStarting -= Core_DownloadStarting;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHWebView2FileChooser: releasing the web view failed: " + ex.Message);
            }
            if (_intercepting)
                _ = TurnInterceptionOffAsync();
        }

        /* Never faults */
        private async Task PrepareCoreAsync()
        {
            try
            {
                await _core.CallDevToolsProtocolMethodAsync("Page.enable", "{}");
                await _core.CallDevToolsProtocolMethodAsync("Page.setInterceptFileChooserDialog", InterceptOn);
                _intercepting = true;
                await _core.CallDevToolsProtocolMethodAsync("Runtime.evaluate", GestureScript);
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHWebView2FileChooser: preparing failed: " + ex.Message);
            }
        }

        private void GamepadService_StateChanged()
        {
            if (_intercepting && !GHGamepadService.IsActive && !_disposed)
                _ = TurnInterceptionOffAsync();
        }

        /* Never faults */
        private async Task TurnInterceptionOffAsync()
        {
            _intercepting = false;
            try
            {
                await _core.CallDevToolsProtocolMethodAsync("Page.setInterceptFileChooserDialog", InterceptOff);
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHWebView2FileChooser: turning interception off failed: " + ex.Message);
            }
        }

        /* An unanswered chooser blocks nothing, so cancelling sends nothing */
        private async void Receiver_DevToolsProtocolEventReceived(object sender, CoreWebView2DevToolsProtocolEventReceivedEventArgs args)
        {
            try
            {
                JObject opened = JObject.Parse(args.ParameterObjectAsJson);
                int backendNodeId = (int)opened["backendNodeId"];
                bool multiple = (string)opened["mode"] == "selectMultiple";
                IEnumerable<string> extensions = await ReadAcceptExtensionsAsync(backendNodeId);
                IList<string> picked = await GHFileDialogHost.PickFilesAsync("Attach Files", "Attach", multiple, multiple ? MaxAttachedFiles : 1, extensions, false);
                if (_disposed || picked == null || picked.Count == 0)
                    return;
                JObject parameters = new JObject();
                parameters["files"] = new JArray(picked.ToArray());
                parameters["backendNodeId"] = backendNodeId;
                await _core.CallDevToolsProtocolMethodAsync("DOM.setFileInputFiles", parameters.ToString(Newtonsoft.Json.Formatting.None));
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHWebView2FileChooser: the file dialog failed: " + ex.Message);
            }
        }

        /* The extensions in the input's accept attribute; MIME types are left out, since a file's name cannot be matched
           against them. Null when there are none. */
        private async Task<IEnumerable<string>> ReadAcceptExtensionsAsync(int backendNodeId)
        {
            JObject parameters = new JObject();
            parameters["backendNodeId"] = backendNodeId;
            string json = await _core.CallDevToolsProtocolMethodAsync("DOM.describeNode", parameters.ToString(Newtonsoft.Json.Formatting.None));
            JArray attributes = JObject.Parse(json)["node"]?["attributes"] as JArray;
            if (attributes == null)
                return null;
            int i;
            for (i = 0; i + 1 < attributes.Count; i += 2)
            {
                if ((string)attributes[i] == "accept")
                {
                    List<string> extensions = new List<string>();
                    foreach (string token in ((string)attributes[i + 1]).Split(','))
                    {
                        string trimmed = token.Trim();
                        if (trimmed.Length > 0 && !trimmed.Contains('/'))
                            extensions.Add(trimmed);
                    }
                    return extensions.Count > 0 ? extensions : null;
                }
            }
            return null;
        }

        private void Core_DownloadStarting(object sender, CoreWebView2DownloadStartingEventArgs args)
        {
            try
            {
                if (!GHFileDialogHost.UseCustomDialog)
                    return;
                string folder = Path.Combine(FileSystem.CacheDirectory, "overseer_share", Guid.NewGuid().ToString("N"));
                Directory.CreateDirectory(folder);
                string path = Path.Combine(folder, GHFileDialogLogic.SanitizeFileName(Path.GetFileName(args.ResultFilePath)));
                args.ResultFilePath = path;
                args.Handled = true;
                CoreWebView2DownloadOperation operation = args.DownloadOperation;
                operation.StateChanged += (s, e) => Operation_StateChanged(operation, path);
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHWebView2FileChooser: DownloadStarting failed: " + ex.Message);
            }
        }

        private async void Operation_StateChanged(CoreWebView2DownloadOperation operation, string path)
        {
            if (operation.State == CoreWebView2DownloadState.InProgress)
                return;
            try
            {
                if (operation.State == CoreWebView2DownloadState.Completed)
                    await GHFileDialogHost.SaveCopyAsync("Save a Copy", path, Path.GetFileName(path));
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHWebView2FileChooser: saving a download failed: " + ex.Message);
            }
            finally
            {
                try
                {
                    Directory.Delete(Path.GetDirectoryName(path), true);
                }
                catch (Exception ex)
                {
                    GHApp.MaybeWriteGHLog("GHWebView2FileChooser: deleting a download failed: " + ex.Message);
                }
            }
        }
    }
}
