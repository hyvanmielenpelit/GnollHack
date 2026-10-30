using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using GnollHackX;
using Microsoft.Maui.Storage;

using AvDispatcher = Avalonia.Threading.Dispatcher;

namespace GnollHackM
{
    /* IFilePicker for the Avalonia target: GHFileDialogHost's control when the system's dialog cannot be used with a
       controller, else the implementation ACM installed. Results from the control are plain FileResults, so read them
       with GHApp.OpenPickedFileAsync. */
    public sealed class GHAvaloniaFilePicker : IFilePicker
    {
        private const string DefaultTitle = "Choose a File";
        private const string ConfirmText = "Choose";
        private const int MaxMultipleFiles = 100;

        private readonly IFilePicker _systemPicker;

        public GHAvaloniaFilePicker(IFilePicker systemPicker)
        {
            _systemPicker = systemPicker;
        }

        public Task<FileResult> PickAsync(PickOptions options = null)
        {
            return AvDispatcher.UIThread.InvokeAsync(() => PickOnUIThreadAsync(options));
        }

        public Task<IEnumerable<FileResult>> PickMultipleAsync(PickOptions options = null)
        {
            return AvDispatcher.UIThread.InvokeAsync(() => PickMultipleOnUIThreadAsync(options));
        }

        private async Task<FileResult> PickOnUIThreadAsync(PickOptions options)
        {
            if (!GHFileDialogHost.UseCustomDialog)
                return await _systemPicker.PickAsync(options);

            IList<string> paths = await GHFileDialogHost.PickFilesAsync(TitleOf(options), ConfirmText, false, 1, ExtensionsOf(options), true);
            return paths != null && paths.Count > 0 ? new FileResult(paths[0]) : null;
        }

        private async Task<IEnumerable<FileResult>> PickMultipleOnUIThreadAsync(PickOptions options)
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
                GHApp.MaybeWriteGHLog("GHAvaloniaFilePicker: reading the file types failed: " + ex.Message);
                return null;
            }
        }
    }
}
