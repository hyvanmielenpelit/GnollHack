using System;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;

namespace GnollHackX
{
    /* What the file dialog control shows and does, with no view types. State changes on the caller's context. */
    public abstract class GHFileDialogBehavior
    {
        private readonly TaskCompletionSource<IList<string>> _result =
            new TaskCompletionSource<IList<string>>(TaskCreationOptions.RunContinuationsAsynchronously);
        private IList<GHFileRow> _rows = new List<GHFileRow>();
        private bool _isStatusFinal;
        private IList<string> _finalResult;
        private bool _isHostGone;

        public abstract string Title { get; }
        public abstract string ConfirmText { get; }

        public virtual string Subtitle
        {
            get { return null; }
        }

        public IList<GHFileRow> Rows
        {
            get { return _rows; }
        }

        public string Notice { get; protected set; }
        public string StatusMessage { get; protected set; }
        public bool IsBusy { get; protected set; }

        public virtual bool CanGoUp
        {
            get { return false; }
        }

        public virtual bool CanConfirm
        {
            get { return false; }
        }

        /* False while activating a row would be ignored */
        public virtual bool CanActivate
        {
            get { return !IsBusy && StatusMessage == null; }
        }

        public event Action Changed;
        public Action<string> Log { get; set; }

        /* Completes once. Null means cancelled. */
        public Task<IList<string>> Result
        {
            get { return _result.Task; }
        }

        protected bool IsHostGone
        {
            get { return _isHostGone; }
        }

        public abstract Task LoadAsync();
        public abstract Task ActivateRowAsync(GHFileRow row);

        public virtual Task GoUpAsync()
        {
            return Task.CompletedTask;
        }

        public virtual void Confirm()
        {
        }

        public void Cancel()
        {
            if (!IsBusy)
            {
                Complete(null);
            }
        }

        public void AcknowledgeStatus()
        {
            if (StatusMessage == null)
            {
                return;
            }

            if (_isStatusFinal)
            {
                Complete(_finalResult);
            }
            else
            {
                StatusMessage = null;
                RaiseChanged();
            }
        }

        public void HostGone()
        {
            _isHostGone = true;
            if (!IsBusy)
            {
                Complete(null);
            }
        }

        protected void Complete(IList<string> result)
        {
            _result.TrySetResult(result);
        }

        protected void RaiseChanged()
        {
            Changed?.Invoke();
        }

        protected void SetRows(IList<GHFileRow> rows)
        {
            _rows = rows ?? new List<GHFileRow>();
        }

        /* A status whose acknowledgment completes the dialog with the result */
        protected void SetFinalStatus(string message, IList<string> result)
        {
            StatusMessage = message;
            _isStatusFinal = true;
            _finalResult = result;
        }

        /* A status whose acknowledgment returns to the rows */
        protected void SetErrorStatus(string message)
        {
            StatusMessage = message;
            _isStatusFinal = false;
            _finalResult = null;
        }

        protected void WriteLog(string message)
        {
            Log?.Invoke(message);
        }

        protected static GHFileRow CreatePlaceRow(GHFilePlace place)
        {
            return new GHFileRow { Kind = GHFileRowKind.Place, Name = place.Name, Detail = place.Path, Path = place.Path, Place = place };
        }

        protected static GHFileRow CreateMessageRow(string message)
        {
            return new GHFileRow { Kind = GHFileRowKind.Message, Name = message };
        }
    }

    /* Browses the given places and returns the chosen paths. Writes nothing. */
    public sealed class GHFileOpenBehavior : GHFileDialogBehavior
    {
        public const int MaxRows = 300;

        private const string PathDivider = " › ";

        private readonly string _title;
        private readonly string _confirmText;
        private readonly bool _multiple;
        private readonly int _maxFiles;
        private readonly IList<string> _extensions;
        private readonly IList<GHFilePlace> _places;
        private readonly List<GHFileRow> _folders = new List<GHFileRow>();
        private readonly List<GHFileRow> _marks = new List<GHFileRow>();
        private GHFilePlace _place;
        private string _folderNotice;
        private int _generation;
        private bool _isReading;

        public GHFileOpenBehavior(string title, string confirmText, bool multiple, int maxFiles, IEnumerable<string> extensions,
            IList<GHFilePlace> places)
        {
            _title = title;
            _confirmText = confirmText;
            _multiple = multiple;
            _maxFiles = maxFiles;
            _extensions = GHFileDialogLogic.NormalizeExtensions(extensions);
            _places = places ?? new List<GHFilePlace>();
        }

        public override string Title
        {
            get { return _title; }
        }

        public override string ConfirmText
        {
            get { return _multiple ? _confirmText : null; }
        }

        public override string Subtitle
        {
            get
            {
                string text = "Choose a place";
                if (_place != null)
                {
                    text = _place.Name;
                    foreach (GHFileRow folder in _folders)
                    {
                        text += PathDivider + folder.Name;
                    }
                }

                return _marks.Count > 0 ? text + " (" + _marks.Count + " selected)" : text;
            }
        }

        public override bool CanGoUp
        {
            get { return _place != null; }
        }

        public override bool CanConfirm
        {
            get { return _multiple && _marks.Count > 0; }
        }

        public override Task LoadAsync()
        {
            _place = null;
            _folders.Clear();
            ShowPlaces();
            return Task.CompletedTask;
        }

        public override bool CanActivate
        {
            get { return !_isReading && base.CanActivate; }
        }

        public override Task ActivateRowAsync(GHFileRow row)
        {
            if (row == null || _isReading || Result.IsCompleted)
            {
                return Task.CompletedTask;
            }

            switch (row.Kind)
            {
            case GHFileRowKind.Place:
                _place = row.Place;
                _folders.Clear();
                return ShowFolderAsync();
            case GHFileRowKind.Folder:
                _folders.Add(row);
                return ShowFolderAsync();
            case GHFileRowKind.File:
                if (_multiple)
                {
                    ToggleMark(row);
                }
                else
                {
                    Complete(new List<string> { row.Path });
                }

                return Task.CompletedTask;
            default:
                return Task.CompletedTask;
            }
        }

        public override Task GoUpAsync()
        {
            if (_place == null || Result.IsCompleted)
            {
                return Task.CompletedTask;
            }

            if (_folders.Count > 0)
            {
                _folders.RemoveAt(_folders.Count - 1);
                return ShowFolderAsync();
            }

            _place = null;
            ShowPlaces();
            return Task.CompletedTask;
        }

        public override void Confirm()
        {
            if (!CanConfirm || Result.IsCompleted)
            {
                return;
            }

            List<string> paths = new List<string>();
            for (int i = _marks.Count - 1; i >= 0; i--)
            {
                if (IsStillAvailable(_marks[i]))
                {
                    paths.Insert(0, _marks[i].Path);
                }
                else
                {
                    WriteLog("A selected file is no longer available: " + _marks[i].Path);
                    _marks.RemoveAt(i);
                }
            }

            if (paths.Count > 0)
            {
                Complete(paths);
                return;
            }

            foreach (GHFileRow row in Rows)
            {
                row.IsMarked = false;
            }

            Notice = "The selected files are no longer available.";
            RaiseChanged();
        }

        private static bool IsStillAvailable(GHFileRow mark)
        {
            try
            {
                return File.Exists(mark.Path)
                    && GHFileDialogLogic.IsInside(GHFileDialogLogic.ResolvePath(mark.Place.Path), GHFileDialogLogic.ResolvePath(mark.Path));
            }
            catch (IOException)
            {
                return false;
            }
            catch (UnauthorizedAccessException)
            {
                return false;
            }
            catch (ArgumentException)
            {
                return false;
            }
        }

        private void ToggleMark(GHFileRow row)
        {
            int index = FindMark(row.Path);
            if (index >= 0)
            {
                _marks.RemoveAt(index);
                row.IsMarked = false;
                Notice = _folderNotice;
            }
            else if (_maxFiles > 0 && _marks.Count >= _maxFiles)
            {
                Notice = "You can select up to " + _maxFiles + " files.";
            }
            else
            {
                _marks.Add(row);
                row.IsMarked = true;
                Notice = _folderNotice;
            }

            RaiseChanged();
        }

        private int FindMark(string path)
        {
            for (int i = 0; i < _marks.Count; i++)
            {
                if (string.Equals(_marks[i].Path, path, StringComparison.Ordinal))
                {
                    return i;
                }
            }

            return -1;
        }

        private void ShowPlaces()
        {
            _generation++;
            _isReading = false;
            List<GHFileRow> rows = new List<GHFileRow>();
            foreach (GHFilePlace place in _places)
            {
                rows.Add(CreatePlaceRow(place));
            }

            if (rows.Count == 0)
            {
                rows.Add(CreateMessageRow("No folders are available."));
            }

            _folderNotice = null;
            Notice = null;
            SetRows(rows);
            RaiseChanged();
        }

        private async Task ShowFolderAsync()
        {
            int generation = ++_generation;
            _isReading = true;
            Notice = "Reading...";
            RaiseChanged();
            GHFilePlace place = _place;
            string folder = _folders.Count > 0 ? _folders[_folders.Count - 1].Path : place.Path;
            int omitted = 0;
            IList<GHFileRow> rows = await Task.Run(() =>
            {
                int count;
                IList<GHFileRow> listed = GHFileDialogLogic.ListFolder(folder, place, _extensions, MaxRows, out count);
                omitted = count;
                return listed;
            });

            /* A newer navigation or a finished dialog replaces this read */
            if (generation != _generation || Result.IsCompleted)
            {
                return;
            }

            _isReading = false;
            foreach (GHFileRow row in rows)
            {
                row.IsMarked = row.Kind == GHFileRowKind.File && FindMark(row.Path) >= 0;
            }

            if (rows.Count == 0)
            {
                rows.Add(CreateMessageRow("This folder is empty."));
            }

            _folderNotice = omitted > 0 ? "Showing the newest " + MaxRows + " items." : null;
            Notice = _folderNotice;
            SetRows(rows);
            RaiseChanged();
        }
    }

    /* Saves a copy of one file into a place the user picks. Never replaces an existing file. */
    public sealed class GHFileSaveBehavior : GHFileDialogBehavior
    {
        private readonly string _title;
        private readonly string _sourcePath;
        private readonly string _fileName;
        private readonly string _subtitle;
        private readonly IList<GHFilePlace> _places;
        private readonly IEnumerable<string> _protectedFolders;

        public GHFileSaveBehavior(string title, string sourcePath, string suggestedName, IList<GHFilePlace> places,
            IEnumerable<string> protectedFolders)
        {
            _title = title;
            _sourcePath = sourcePath;
            _fileName = GHFileDialogLogic.SanitizeFileName(string.IsNullOrEmpty(suggestedName) ? Path.GetFileName(sourcePath) : suggestedName);
            _places = places ?? new List<GHFilePlace>();
            _protectedFolders = protectedFolders;
            _subtitle = _fileName + GetSizeText(sourcePath);
        }

        public string SourcePath
        {
            get { return _sourcePath; }
        }

        public string SuggestedName
        {
            get { return _fileName; }
        }

        public override string Title
        {
            get { return _title; }
        }

        public override string ConfirmText
        {
            get { return null; }
        }

        public override string Subtitle
        {
            get { return _subtitle; }
        }

        public override Task LoadAsync()
        {
            List<GHFileRow> rows = new List<GHFileRow>();
            foreach (GHFilePlace place in _places)
            {
                rows.Add(CreatePlaceRow(place));
            }

            if (rows.Count == 0)
            {
                rows.Add(CreateMessageRow("No folders are available."));
            }

            SetRows(rows);
            RaiseChanged();
            return Task.CompletedTask;
        }

        public override async Task ActivateRowAsync(GHFileRow row)
        {
            if (IsBusy || StatusMessage != null || Result.IsCompleted || row == null || row.Kind != GHFileRowKind.Place || row.Place == null)
            {
                return;
            }

            GHFilePlace place = row.Place;
            if (GHFileDialogLogic.IsProtectedTarget(place.Path, _protectedFolders, _sourcePath))
            {
                SetErrorStatus("GnollHack does not save copies there.");
                RaiseChanged();
                return;
            }

            IsBusy = true;
            RaiseChanged();

            string finalPath = null;
            string error = null;
            try
            {
                finalPath = await Task.Run(() => GHFileDialogLogic.CopyNoClobber(_sourcePath, place.Path, _fileName));
            }
            catch (Exception ex)
            {
                error = ex.Message;
            }

            IsBusy = false;
            if (IsHostGone)
            {
                WriteLog(finalPath != null
                    ? "The file dialog's page closed during a copy, which saved " + finalPath
                    : "The file dialog's page closed during a copy, which failed: " + error);
                Complete(finalPath != null ? new List<string> { finalPath } : null);
                return;
            }

            if (finalPath != null)
            {
                SetFinalStatus("Saved to " + place.Name + "/" + Path.GetFileName(finalPath), new List<string> { finalPath });
            }
            else
            {
                SetErrorStatus("Could not save the file: " + error);
            }

            RaiseChanged();
        }

        private static string GetSizeText(string path)
        {
            try
            {
                return " · " + GHFileDialogLogic.FormatSize(new FileInfo(path).Length);
            }
            catch (Exception ex) when (ex is IOException || ex is UnauthorizedAccessException || ex is ArgumentException || ex is NotSupportedException)
            {
                return "";
            }
        }
    }
}
