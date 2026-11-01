using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using GnollHackX;
using Microsoft.Maui.ApplicationModel;
using Microsoft.Maui.Storage;

namespace GnollHackM
{
    /* IFilePicker for the WinUI 3 build: GHFileDialogHost's control when the system's dialog cannot be used with a
       controller, else the implementation it wraps. Results from the control are plain FileResults. */
    public sealed class GHWinUIFilePicker : IFilePicker
    {
        private const string DefaultTitle = "Choose a File";
        private const string ConfirmText = "Choose";
        private const int MaxMultipleFiles = 100;

        private readonly IFilePicker _systemPicker;

        public GHWinUIFilePicker(IFilePicker systemPicker)
        {
            _systemPicker = systemPicker;
        }

        public Task<FileResult> PickAsync(PickOptions options = null)
        {
            return MainThread.InvokeOnMainThreadAsync(() => PickOnMainThreadAsync(options));
        }

        public Task<IEnumerable<FileResult>> PickMultipleAsync(PickOptions options = null)
        {
            return MainThread.InvokeOnMainThreadAsync(() => PickMultipleOnMainThreadAsync(options));
        }

        private async Task<FileResult> PickOnMainThreadAsync(PickOptions options)
        {
            if (!GHFileDialogHost.UseCustomDialog)
                return await _systemPicker.PickAsync(options);

            IList<string> paths = await GHFileDialogHost.PickFilesAsync(TitleOf(options), ConfirmText, false, 1, ExtensionsOf(options), true);
            return paths != null && paths.Count > 0 ? new FileResult(paths[0]) : null;
        }

        private async Task<IEnumerable<FileResult>> PickMultipleOnMainThreadAsync(PickOptions options)
        {
            if (!GHFileDialogHost.UseCustomDialog)
                return await _systemPicker.PickMultipleAsync(options);

            List<FileResult> results = new List<FileResult>();
            IList<string> paths = await GHFileDialogHost.PickFilesAsync(TitleOf(options), ConfirmText, true, MaxMultipleFiles, ExtensionsOf(options), true);
            if (paths != null)
            {
                foreach (string path in paths)
                    results.Add(new FileResult(path));
            }
            return results;
        }

        private static string TitleOf(PickOptions options)
        {
            return options != null && !string.IsNullOrWhiteSpace(options.PickerTitle) ? options.PickerTitle : DefaultTitle;
        }

        /* Extensions only; a FilePickerFileType may also name MIME types, which a file's name cannot be matched against */
        private static IEnumerable<string> ExtensionsOf(PickOptions options)
        {
            try
            {
                IEnumerable<string> values = options?.FileTypes?.Value;
                if (values == null)
                    return null;
                List<string> extensions = new List<string>();
                foreach (string value in values)
                {
                    if (!string.IsNullOrEmpty(value) && !value.Contains('/'))
                        extensions.Add(value);
                }
                return extensions.Count > 0 ? extensions : null;
            }
            catch (Exception ex)
            {
                GHApp.MaybeWriteGHLog("GHWinUIFilePicker: reading the file types failed: " + ex.Message);
                return null;
            }
        }
    }
}
