using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.Threading.Tasks;
#if GNH_MAUI
using Microsoft.Maui.Controls;
using GnollHackX;
#else
using Xamarin.Forms;
#endif

#if GNH_MAUI
namespace GnollHackM
#else
namespace GnollHackX.Controls
#endif
{
    /* A web view's script evaluation for GHWebGamepadNavigator. UI thread only. */
    public interface IGHWebScriptHost
    {
        /* A platform web view is attached */
        bool IsReady { get; }

        /* Device pixels per DIP of the web view's window; 1 when unknown */
        double RenderScaling { get; }

        /* The page script that defines window.__gnhPad; null when it cannot be read */
        string PageScript { get; }

        /* The script's result as the platform returns it, or null on timeout, failure or without a view */
        Task<string> EvaluateAsync(string script);

        /* Gives the web view native keyboard focus */
        void FocusNative();

        /* A new document: its first evaluation failure is logged again */
        void ResetFailureLog();

        /* The platform web view was attached, replaced or detached */
        event Action ViewChanged;
    }

    public enum GHWebNavigatorMode
    {
        Page,
        Edit,
        Toolbar
    }

    /* Gamepad navigation of a web page through the page script (gamepad_web.js), with the page's button row as a
       toolbar mode. Page mode moves a focus ring between the page's links and controls (GHWebSpatialNavigation),
       scrolls, activates, and jumps between sections; View cycles the article, the sidebar and the toolbar. Edit mode
       is a text field being edited with the on-screen keyboard; Toolbar mode hands the buttons, except B and Menu, to
       the toolbar's GHGamepadPageNavigator. Without a
       ready script host Toolbar is the only mode. Page interaction runs as operations on a serial queue, each
       stopping once Detach, OnNavigating or a host change has passed it by; the right stick scrolls outside the
       queue. UI thread only. */
    public sealed class GHWebGamepadNavigator
    {
        /* DIP per second at full right-stick push; the push is raised to RightStickScrollExponent first */
        private const double RightStickScrollSpeed = 1200.0;
        private const double RightStickScrollExponent = 1.5;
        /* How long the page keeps scrolling at a sent velocity, and how often a held velocity is sent again */
        private const int StickVelocityLifetimeMilliseconds = 250;
        private const double StickResendSeconds = 0.1;
        /* A velocity change larger than this share of RightStickScrollSpeed is sent at once */
        private const double StickChangeThreshold = 0.05;

        private const string ScriptPrefix = "(function(){var p=window.__gnhPad;if(!p||p.v!==2)return 'M';return p.";
        private const string ScriptSuffix = ";})()";
        private const string MissingResult = "M";
        private const string ErrorPrefix = "E~";

        private sealed class PendingOperation
        {
            public Func<int, Task> Run;
            public int Generation;
            /* The button whose Repeat events are dropped while this is pending, or -1 */
            public int Button;
        }

        private readonly GHGamepadPageNavigator _toolbar;
        private readonly WebView _webView;
        private IGHWebScriptHost _host;
        private GHWebGamepadProfile _profile = GHWebGamepadProfile.CreateDefault();
        private GHWebNavigatorMode _mode = GHWebNavigatorMode.Page;
        private bool _attached = false;
        private bool _toolbarAttached = false;
        /* The press that entered Toolbar mode; its Repeat events are ignored there */
        private int _toolbarEntryPressId = -1;

        private int _generation = 0;
        private readonly Queue<PendingOperation> _queue = new Queue<PendingOperation>();
        private bool _running = false;
        private readonly int[] _pendingRepeatable = new int[(int)GHGamepadButton.Count];

        /* A call in this document found the page script */
        private bool _scriptPresent = false;
        private bool _hasSearch = false;
        /* The page has a sidebar area; the area of the last usable focus */
        private bool _hasSide = false;
        private char _lastArea = GHWebCandidate.MainArea;
        private bool _searchQueried = false;
        private bool _failureLogged = false;
        private bool _aArmed = false;
        private int _aPressId = 0;
        /* The kind of the focus A was armed on */
        private char _aKind = '\0';
        /* This navigator opened the on-screen keyboard and has not closed it */
        private bool _keyboardOpened = false;
        private bool _editMultipleLines = false;
        /* The edited field's x,y,w,h in CSS px from the viewport's top-left when the edit began, or null */
        private string _editRect;

        /* From the last snapshot */
        private double _devicePixelRatio = 1.0;
        private double _viewportHeight = 0.0;

        /* Right-stick velocity last sent, in DIP/s */
        private bool _stickScrolling = false;
        private bool _stickInFlight = false;
        /* Identifies the scroll() call in flight; a reset makes its completion leave _stickInFlight alone */
        private int _stickCallId = 0;
        private double _stickSentX = 0.0;
        private double _stickSentY = 0.0;
        private long _stickSentTicks = 0;

        private List<GHGamepadHint> _hints = new List<GHGamepadHint>();

        public GHWebGamepadNavigator(GHGamepadPageNavigator toolbar, WebView webView)
        {
            _toolbar = toolbar;
            _webView = webView;
            _hints = BuildHints();
        }

        /* The page's script host; null without one (non-Avalonia flavors) */
        public IGHWebScriptHost Host
        {
            get { return _host; }
            set
            {
                if (ReferenceEquals(value, _host))
                    return;
                if (_host != null)
                    _host.ViewChanged -= Host_ViewChanged;
                _host = value;
                if (_host != null)
                    _host.ViewChanged += Host_ViewChanged;
                _generation++;
                _scriptPresent = false;
                ClearQueue();
                HideKeyboard();
                if (_mode == GHWebNavigatorMode.Edit)
                    _mode = GHWebNavigatorMode.Page;
                _aArmed = false;
                ResetStick();
                _failureLogged = false;
                RefreshHints();
            }
        }

        public Func<bool> CanGoBack { get; set; }
        public Action GoBack { get; set; }
        /* Plays the click sound itself */
        public Action ClosePage { get; set; }
        public Func<bool> CanGoForward { get; set; }
        public Action GoForward { get; set; }

        /* The page's labels and actions; never null, null sets the default profile */
        public GHWebGamepadProfile Profile
        {
            get { return _profile; }
            set
            {
                _profile = value ?? GHWebGamepadProfile.CreateDefault();
                RefreshHints();
            }
        }

        /* Raised when CurrentHints changes and when the gamepad state changes */
        public event Action HintsChanged;

        /* The current mode's hints in priority order; replaced, never changed in place */
        public IList<GHGamepadHint> CurrentHints { get { return _hints; } }

        public GHWebNavigatorMode Mode { get { return EffectiveMode; } }

        private bool HasPage
        {
            get
            {
                IGHWebScriptHost host = _host;
                return host != null && host.IsReady;
            }
        }

        private GHWebNavigatorMode EffectiveMode
        {
            get { return HasPage ? _mode : GHWebNavigatorMode.Toolbar; }
        }

        /* From the page's Appearing */
        public void Attach()
        {
            if (!_attached)
            {
                GHGamepadService.StateChanged += GamepadService_StateChanged;
                _attached = true;
            }
            _aArmed = false;
            UpdateToolbarAttachment();
            RefreshHints();
        }

        /* From the page's Disappearing */
        public void Detach()
        {
            if (_attached)
            {
                GHGamepadService.StateChanged -= GamepadService_StateChanged;
                _attached = false;
            }
            _generation++;
            ClearQueue();
            HideKeyboard();
            if (_mode == GHWebNavigatorMode.Edit)
                _mode = GHWebNavigatorMode.Page;
            _aArmed = false;
            ResetStick();
            UpdateToolbarAttachment();
        }

        /* From the web view's Navigating */
        public void OnNavigating()
        {
            _generation++;
            ClearQueue();
            HideKeyboard();
            if (_mode == GHWebNavigatorMode.Edit)
                _mode = GHWebNavigatorMode.Page;
            _aArmed = false;
            _scriptPresent = false;
            _searchQueried = false;
            _hasSearch = false;
            _hasSide = false;
            _lastArea = GHWebCandidate.MainArea;
            _failureLogged = false;
            ResetStick();
            IGHWebScriptHost host = _host;
            if (host != null)
                host.ResetFailureLog();
            RefreshHints();
        }

        /* From the web view's Navigated; prepares the page when the controller is in use */
        public void OnNavigated()
        {
            if (HasPage && GHGamepadService.IsAvailable && GHGamepadService.IsEnabled)
                Enqueue(PreparePageAsync, -1);
            RefreshHints();
        }

        /* From an in-page route change, which raises neither Navigating nor Navigated: re-places the ring and asks
           for a search field and sidebar again, in the same document */
        public void OnDocumentChanged()
        {
            if (HasPage && (_scriptPresent || (GHGamepadService.IsAvailable && GHGamepadService.IsEnabled)))
                Enqueue(DocumentChangedAsync, -1);
        }

        public void RefreshHints()
        {
            try
            {
                UpdateToolbarAttachment();
                List<GHGamepadHint> hints = BuildHints();
                if (SameHints(hints, _hints))
                    return;
                _hints = hints;
                RaiseHintsChanged();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
        }

        /* A new platform view holds a new document */
        private void Host_ViewChanged()
        {
            try
            {
                _generation++;
                _scriptPresent = false;
                ClearQueue();
                ResetStick();
                RefreshHints();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
        }

        private void GamepadService_StateChanged()
        {
            try
            {
                UpdateToolbarAttachment();
                if (EffectiveMode == GHWebNavigatorMode.Page && (_scriptPresent || GHGamepadService.IsActive))
                {
                    bool visible = GHGamepadService.IsActive;
                    Enqueue(gen => SetVisibleAsync(gen, visible), -1);
                }
                _hints = BuildHints();
                RaiseHintsChanged();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
        }

        /* Buttons */

        /* Always consumes the event */
        public bool HandleButton(GHGamepadButtonEvent e)
        {
            /* Ignore input when opening a page or using a system browser */
            if (GHApp.PushingModalPage || GHApp.IsSystemBrowserOpen)
                return true;

            try
            {
                UpdateToolbarAttachment();
                if (e.RevealsUi)
                    _aArmed = false;
                switch (EffectiveMode)
                {
                case GHWebNavigatorMode.Toolbar:
                    HandleToolbarButton(e);
                    break;
                case GHWebNavigatorMode.Edit:
                    HandleEditButton(e);
                    break;
                default:
                    HandlePageButton(e);
                    break;
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
            return true;
        }

        private void HandlePageButton(GHGamepadButtonEvent e)
        {
            if (e.RevealsUi)
            {
                Enqueue(gen => RevealAsync(gen, null), -1);
                return;
            }

            bool pressed = e.Kind == GHGamepadButtonEventKind.Pressed;
            bool pressedOrRepeat = pressed || e.Kind == GHGamepadButtonEventKind.Repeat;
            switch (e.Button)
            {
            case GHGamepadButton.B:
                if (pressed)
                    Enqueue(BackAsync, -1);
                return;
            case GHGamepadButton.View:
                if (pressed)
                {
                    int viewPressId = e.PressId;
                    Enqueue(gen => ViewAsync(gen, viewPressId), -1);
                }
                return;
            case GHGamepadButton.Menu:
                if (pressed)
                    RunClosePage();
                return;
            case GHGamepadButton.X:
                if (pressed)
                {
                    string composeKey = _profile.ComposeKey;
                    if (composeKey != null)
                        Enqueue(gen => ComposeAsync(gen, composeKey), -1);
                    else
                        Enqueue(ForwardAsync, -1);
                }
                return;
            default:
                break;
            }

            GHWebDirection dir;
            if (TryGetDirection(e.Button, out dir))
            {
                if (pressedOrRepeat)
                    EnqueueRepeatable(e, gen => DirectionAsync(gen, dir, e.PressId));
                return;
            }

            int pressId = e.PressId;
            switch (e.Button)
            {
            case GHGamepadButton.A:
                if (pressed)
                    Enqueue(gen => ArmAsync(gen, pressId), -1);
                else if (e.Kind == GHGamepadButtonEventKind.Released && e.IsCancelled)
                    Enqueue(gen => DisarmAsync(pressId), -1);
                else if (e.Kind == GHGamepadButtonEventKind.ShortPressed)
                    Enqueue(gen => ActivateAsync(gen, pressId), -1);
                break;
            case GHGamepadButton.Y:
                if (pressed)
                    Enqueue(SearchAsync, -1);
                break;
            case GHGamepadButton.LB:
            case GHGamepadButton.RB:
                if (pressedOrRepeat)
                {
                    int sectionDir = e.Button == GHGamepadButton.LB ? -1 : 1;
                    EnqueueRepeatable(e, gen => SimpleCallAsync(gen, "section(" + Int(sectionDir) + ")"));
                }
                break;
            case GHGamepadButton.LT:
            case GHGamepadButton.RT:
                if (pressed)
                {
                    int pageDir = e.Button == GHGamepadButton.LT ? -1 : 1;
                    Enqueue(gen => SimpleCallAsync(gen, "page(" + Int(pageDir) + ")"), -1);
                }
                break;
            case GHGamepadButton.L3:
                if (pressed)
                {
                    string l3Call = _profile.L3Call;
                    if (!string.IsNullOrEmpty(l3Call))
                        Enqueue(gen => SimpleCallAsync(gen, l3Call), -1);
                }
                break;
            default:
                break;
            }
        }

        /* A and B act even on a RevealsUi press, since typing makes the keyboard the active input */
        private void HandleEditButton(GHGamepadButtonEvent e)
        {
            bool pressed = e.Kind == GHGamepadButtonEventKind.Pressed;
            switch (e.Button)
            {
            case GHGamepadButton.B:
                if (pressed)
                    Enqueue(EndEditOperationAsync, -1);
                return;
            case GHGamepadButton.A:
                if (e.Kind == GHGamepadButtonEventKind.ShortPressed)
                    Enqueue(EditKeyboardAsync, -1);
                return;
            default:
                break;
            }

            if (e.RevealsUi)
            {
                Enqueue(gen => RevealAsync(gen, null), -1);
                return;
            }

            if (e.Button == GHGamepadButton.Menu)
            {
                if (pressed)
                    RunClosePage();
                return;
            }

            GHWebDirection dir;
            if (TryGetDirection(e.Button, out dir) && (pressed || e.Kind == GHGamepadButtonEventKind.Repeat))
            {
                EnqueueRepeatable(e, gen => DirectionAsync(gen, dir, e.PressId));
            }
        }

        private void HandleToolbarButton(GHGamepadButtonEvent e)
        {
            if (e.RevealsUi)
            {
                _toolbar.HandleButton(e);
                return;
            }
            if (e.Kind == GHGamepadButtonEventKind.Repeat && e.PressId == _toolbarEntryPressId)
                return;

            bool pressed = e.Kind == GHGamepadButtonEventKind.Pressed;
            switch (e.Button)
            {
            case GHGamepadButton.B:
                if (pressed)
                    Enqueue(BackAsync, -1);
                return;
            case GHGamepadButton.X:
                if (pressed)
                {
                    string composeKey = _profile.ComposeKey;
                    if (composeKey == null)
                    {
                        Enqueue(ForwardAsync, -1);
                    }
                    else if (HasPage)
                    {
                        SetMode(GHWebNavigatorMode.Page, e.PressId);
                        Enqueue(gen => ComposeAsync(gen, composeKey), -1);
                    }
                }
                return;
            case GHGamepadButton.Menu:
                if (pressed)
                    RunClosePage();
                return;
            default:
                break;
            }

            if (HasPage)
            {
                switch (e.Button)
                {
                case GHGamepadButton.DPadDown:
                case GHGamepadButton.LeftStickDown:
                    if (pressed)
                        SetMode(GHWebNavigatorMode.Page, e.PressId);
                    return;
                case GHGamepadButton.View:
                    if (pressed)
                    {
                        SetMode(GHWebNavigatorMode.Page, e.PressId);
                        Enqueue(ArticleAsync, -1);
                    }
                    return;
                case GHGamepadButton.DPadUp:
                case GHGamepadButton.LeftStickUp:
                    return;
                default:
                    break;
                }
            }
            _toolbar.HandleButton(e);
        }

        private static bool TryGetDirection(GHGamepadButton button, out GHWebDirection dir)
        {
            switch (button)
            {
            case GHGamepadButton.DPadUp:
            case GHGamepadButton.LeftStickUp:
                dir = GHWebDirection.Up;
                return true;
            case GHGamepadButton.DPadDown:
            case GHGamepadButton.LeftStickDown:
                dir = GHWebDirection.Down;
                return true;
            case GHGamepadButton.DPadLeft:
            case GHGamepadButton.LeftStickLeft:
                dir = GHWebDirection.Left;
                return true;
            case GHGamepadButton.DPadRight:
            case GHGamepadButton.LeftStickRight:
                dir = GHWebDirection.Right;
                return true;
            default:
                dir = GHWebDirection.Up;
                return false;
            }
        }

        /* Modes */

        private void SetMode(GHWebNavigatorMode mode, int pressId)
        {
            if (mode == _mode)
                return;
            _mode = mode;
            if (mode == GHWebNavigatorMode.Toolbar)
                _toolbarEntryPressId = pressId;
            _aArmed = false;
            RefreshHints();
        }

        /* The toolbar navigator is attached exactly while this navigator is attached in Toolbar mode; the in-page ring
           is hidden while it is */
        private void UpdateToolbarAttachment()
        {
            bool attach = _attached && EffectiveMode == GHWebNavigatorMode.Toolbar;
            if (attach == _toolbarAttached)
                return;
            _toolbarAttached = attach;
            if (attach)
            {
                if (HasPage && _scriptPresent)
                    Enqueue(gen => SetVisibleAsync(gen, false), -1);
                _toolbar.Attach();
            }
            else
            {
                _toolbar.Detach();
                _toolbar.Main.Focus.ClearVisuals();
                if (_attached && HasPage && (_scriptPresent || GHGamepadService.IsActive))
                {
                    bool visible = GHGamepadService.IsActive;
                    Enqueue(gen => SetVisibleAsync(gen, visible), -1);
                }
            }
        }

        /* Operation queue */

        private void Enqueue(Func<int, Task> run, int button)
        {
            if (button >= 0 && button < _pendingRepeatable.Length)
                _pendingRepeatable[button]++;
            PendingOperation op = new PendingOperation();
            op.Run = run;
            op.Generation = _generation;
            op.Button = button;
            _queue.Enqueue(op);
            if (!_running)
                RunQueue();
        }

        /* Drops a Repeat while an operation of the same button is queued or running */
        private void EnqueueRepeatable(GHGamepadButtonEvent e, Func<int, Task> run)
        {
            int button = (int)e.Button;
            if (e.Kind == GHGamepadButtonEventKind.Repeat && _pendingRepeatable[button] > 0)
                return;
            Enqueue(run, button);
        }

        private async void RunQueue()
        {
            if (_running)
                return;
            _running = true;
            try
            {
                while (_queue.Count > 0)
                {
                    PendingOperation op = _queue.Dequeue();
                    try
                    {
                        if (op.Generation == _generation)
                            await op.Run(op.Generation);
                    }
                    catch (Exception ex)
                    {
                        Debug.WriteLine(ex);
                    }
                    finally
                    {
                        ReleaseRepeatable(op.Button);
                    }
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
            finally
            {
                _running = false;
            }
        }

        private void ClearQueue()
        {
            while (_queue.Count > 0)
            {
                PendingOperation op = _queue.Dequeue();
                ReleaseRepeatable(op.Button);
            }
        }

        private void ReleaseRepeatable(int button)
        {
            if (button >= 0 && button < _pendingRepeatable.Length && _pendingRepeatable[button] > 0)
                _pendingRepeatable[button]--;
        }

        /* Script calls */

        /* One call: the normalized result, with the page script injected and the call retried once when it is
           missing; null when the call failed, returned an error, or generation is no longer current */
        private async Task<string> CallAsync(int generation, string call)
        {
            IGHWebScriptHost host = _host;
            if (host == null || !host.IsReady || generation != _generation)
                return null;
            string script = ScriptPrefix + call + ScriptSuffix;
            string result = GHWebSpatialNavigation.NormalizeScriptResult(await host.EvaluateAsync(script));
            if (generation != _generation)
                return null;
            if (result == MissingResult)
            {
                if (!await InjectAsync(generation))
                    return null;
                result = GHWebSpatialNavigation.NormalizeScriptResult(await host.EvaluateAsync(script));
                if (generation != _generation)
                    return null;
                if (result != null && result != MissingResult && !_searchQueried)
                    Enqueue(QuerySearchAsync, -1);
            }
            if (result == null || result == MissingResult || result.StartsWith(ErrorPrefix, StringComparison.Ordinal))
            {
                LogFailure(call, result);
                return null;
            }
            _scriptPresent = true;
            return result;
        }

        /* Evaluates the page script; false when there is none or generation is no longer current */
        private async Task<bool> InjectAsync(int generation)
        {
            IGHWebScriptHost host = _host;
            string pageScript = host != null ? host.PageScript : null;
            if (string.IsNullOrEmpty(pageScript))
            {
                LogFailure("inject", "no page script");
                return false;
            }
            await host.EvaluateAsync(pageScript);
            return generation == _generation;
        }

        private void LogFailure(string call, string result)
        {
            if (_failureLogged)
                return;
            _failureLogged = true;
            int paren = call != null ? call.IndexOf('(') : -1;
            string name = paren > 0 ? call.Substring(0, paren) : call;
            GHApp.MaybeWriteGHLog("GHWebGamepadNavigator: " + name + " failed: " + (result ?? "no result"));
        }

        private async Task<GHWebSnapshot> SnapshotAsync(int generation)
        {
            double reach = GHWebSpatialNavigation.ScrollStep(_viewportHeight);
            string result = await CallAsync(generation, "snapshot(" + Num(reach) + ")");
            if (result == null)
                return null;
            GHWebSnapshot snapshot;
            if (!GHWebSnapshot.TryParse(result, out snapshot))
            {
                LogFailure("snapshot", "unparsable result");
                return null;
            }
            _devicePixelRatio = snapshot.DevicePixelRatio;
            _viewportHeight = snapshot.ViewportHeight;
            if (GHWebSpatialNavigation.HasUsableFocus(snapshot))
                NoteArea(snapshot.Candidates[snapshot.FocusPosition].Area);
            return snapshot;
        }

        /* The snapshot an operation starts with; null when the call failed, or when the ring was hidden and the
           operation has revealed it instead */
        private async Task<GHWebSnapshot> BeginAsync(int generation)
        {
            GHWebSnapshot snapshot = await SnapshotAsync(generation);
            if (snapshot == null)
                return null;
            if (snapshot.RingHidden)
            {
                await RevealAsync(generation, snapshot);
                return null;
            }
            return snapshot;
        }

        private Task<string> SetVisibleAsync(int generation, bool visible)
        {
            return CallAsync(generation, visible ? "setVisible(true)" : "setVisible(false)");
        }

        private Task<string> FocusAsync(int generation, GHWebSnapshot snapshot, int position)
        {
            return CallAsync(generation, "focus(" + Int(snapshot.Seq) + "," + Int(position) + ")");
        }

        /* Operations */

        /* Injects the page script, sets the initial focus, shows or hides the ring and asks for a search field */
        private async Task PreparePageAsync(int generation)
        {
            if (!await InjectAsync(generation))
                return;
            if (await CallAsync(generation, "initial()") == null)
                return;
            bool visible = GHGamepadService.IsActive && EffectiveMode == GHWebNavigatorMode.Page;
            if (await SetVisibleAsync(generation, visible) == null)
                return;
            await QuerySearchAsync(generation);
        }

        /* The page's search field and sidebar, for the hints and View */
        private async Task QuerySearchAsync(int generation)
        {
            string result = await CallAsync(generation, "hasSearch()");
            if (result == null)
                return;
            _searchQueried = true;
            bool hasSearch = result == "D~1";
            string side = await CallAsync(generation, "hasArea('s')");
            if (generation != _generation)
                return;
            bool hasSide = side == "D~1";
            if (hasSearch != _hasSearch || hasSide != _hasSide)
            {
                _hasSearch = hasSearch;
                _hasSide = hasSide;
                RefreshHints();
            }
        }

        /* The area of the focus; the View hint depends on it */
        private void NoteArea(char area)
        {
            if (area == _lastArea)
                return;
            _lastArea = area;
            RefreshHints();
        }

        /* View in Page mode: the sidebar from the article or another area, when the page has one; else the toolbar */
        private async Task ViewAsync(int generation, int pressId)
        {
            if (_hasSide && _lastArea != GHWebCandidate.SideArea)
            {
                string result = await CallAsync(generation, "area('s')");
                if (generation != _generation)
                    return;
                if (result != null && result.StartsWith("F~", StringComparison.Ordinal) && result != "F~n")
                {
                    NoteArea(GHWebCandidate.SideArea);
                    if (GHGamepadService.IsActive)
                        await SetVisibleAsync(generation, true);
                    return;
                }
            }
            SetMode(GHWebNavigatorMode.Toolbar, pressId);
        }

        /* View from the toolbar: the article */
        private async Task ArticleAsync(int generation)
        {
            string result = await CallAsync(generation, "area('m')");
            if (result != null && result.StartsWith("F~", StringComparison.Ordinal) && result != "F~n")
                NoteArea(GHWebCandidate.MainArea);
        }

        /* Shows the ring, first moving an unset or lost focus to the top visible candidate of the last focus's area;
           a pending initial focus is scrolled into view by setVisible instead */
        private async Task RevealAsync(int generation, GHWebSnapshot snapshot)
        {
            if (snapshot == null)
            {
                snapshot = await SnapshotAsync(generation);
                if (snapshot == null)
                    return;
            }
            if (!GHWebSpatialNavigation.HasUsableFocus(snapshot) && !snapshot.InitialPending)
            {
                int top = GHWebSpatialNavigation.FindTopVisible(snapshot, _lastArea);
                if (top >= 0 && await FocusAsync(generation, snapshot, top) == null)
                    return;
            }
            await SetVisibleAsync(generation, true);
        }

        private async Task SimpleCallAsync(int generation, string call)
        {
            if (await BeginAsync(generation) == null)
                return;
            await CallAsync(generation, call);
        }

        private async Task DirectionAsync(int generation, GHWebDirection dir, int pressId)
        {
            if (_mode == GHWebNavigatorMode.Edit)
            {
                await EndEditAsync(generation);
                if (generation != _generation)
                    return;
            }

            GHWebSnapshot s = await BeginAsync(generation);
            if (s == null)
                return;
            if (s.Editing)
            {
                /* A field focused by other means: the direction ends its edit first */
                await EndEditAsync(generation);
                s = await SnapshotAsync(generation);
                if (s == null)
                    return;
            }

            if (!GHWebSpatialNavigation.HasUsableFocus(s))
            {
                int top = GHWebSpatialNavigation.FindTopVisible(s, _lastArea);
                if (top >= 0)
                {
                    await FocusAsync(generation, s, top);
                    return;
                }
            }
            else if (s.FocusKind == 's' && (dir == GHWebDirection.Left || dir == GHWebDirection.Right))
            {
                await CallAsync(generation, dir == GHWebDirection.Left ? "adjust(-1)" : "adjust(1)");
                return;
            }

            int target = GHWebSpatialNavigation.FindTarget(s, dir);
            if (target >= 0)
            {
                await FocusAsync(generation, s, target);
                return;
            }

            double step = GHWebSpatialNavigation.ScrollStep(s.ViewportHeight);
            switch (dir)
            {
            case GHWebDirection.Down:
                await CallAsync(generation, "step(0," + Num(step) + ")");
                break;
            case GHWebDirection.Up:
                /* Pages may scroll an inner container while the window stays at the top: the toolbar only when
                   nothing scrolled */
                string scrolled = await CallAsync(generation, "step(0," + Num(-step) + ")");
                if (scrolled == "D~0" && generation == _generation)
                    SetMode(GHWebNavigatorMode.Toolbar, pressId);
                break;
            case GHWebDirection.Left:
                await CallAsync(generation, "step(" + Num(-step) + ",0)");
                break;
            default:
                await CallAsync(generation, "step(" + Num(step) + ",0)");
                break;
            }
        }

        /* A Pressed: arms A for the press when the ring shows a focus, else reveals the ring */
        private async Task ArmAsync(int generation, int pressId)
        {
            GHWebSnapshot s = await SnapshotAsync(generation);
            if (s == null)
                return;
            if (s.RingHidden || !GHWebSpatialNavigation.HasUsableFocus(s))
            {
                _aArmed = false;
                await RevealAsync(generation, s);
                return;
            }
            _aArmed = true;
            _aPressId = pressId;
            _aKind = s.FocusKind;
        }

        private Task DisarmAsync(int pressId)
        {
            if (_aPressId == pressId)
                _aArmed = false;
            return Task.CompletedTask;
        }

        /* A ShortPressed of the armed press. A followed link usually starts the navigation before the call returns,
           which leaves the call without a result. */
        private async Task ActivateAsync(int generation, int pressId)
        {
            if (!_aArmed || _aPressId != pressId)
                return;
            _aArmed = false;
            char kind = _aKind;
            string result = await CallAsync(generation, "activate()");
            if (result != null)
                HandleActivation(result);
            else if (generation != _generation && kind == 'a' && _attached)
                GHApp.PlayButtonClickedSound();
        }

        /* A~kind[~x,y,w,h]: t or m starts editing, c, g and s click */
        private void HandleActivation(string result)
        {
            string[] parts = result.Split('~');
            if (parts.Length < 2 || parts[0] != "A" || parts[1].Length != 1)
                return;
            char kind = parts[1][0];
            switch (kind)
            {
            case 't':
            case 'm':
                EnterEdit(kind == 'm', parts.Length > 2 ? parts[2] : null);
                break;
            case 'c':
            case 'g':
            case 's':
                GHApp.PlayButtonClickedSound();
                break;
            default:
                break;
            }
        }

        private async Task SearchAsync(int generation)
        {
            if (await BeginAsync(generation) == null)
                return;
            string result = await CallAsync(generation, "search()");
            if (result != null)
                HandleActivation(result);
        }

        /* X with a compose key: focuses the element marked with the key and activates it, which starts editing a text
           field */
        private async Task ComposeAsync(int generation, string key)
        {
            if (await BeginAsync(generation) == null)
                return;
            string focused = await CallAsync(generation, "focusMarked(" + Str(key) + ")");
            if (focused == null || focused == "F~n" || generation != _generation)
                return;
            string result = await CallAsync(generation, "activate()");
            if (result != null)
                HandleActivation(result);
        }

        /* Re-places the ring after an in-page route change and asks for a search field and sidebar again */
        private async Task DocumentChangedAsync(int generation)
        {
            await CallAsync(generation, "refresh()");
            if (generation != _generation)
                return;
            await QuerySearchAsync(generation);
        }

        /* X: forward in history, as the toolbar's forward button; nothing when there is no page to go forward to */
        private Task ForwardAsync(int generation)
        {
            try
            {
                if (generation == _generation && CanGoForward != null && CanGoForward() && GoForward != null)
                {
                    GHApp.PlayButtonClickedSound();
                    GoForward();
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
            return Task.CompletedTask;
        }

        /* B: ends an edit, else closes the details around the focus, else goes back in history, else runs the
           profile's BackFallback or closes the page. The page is asked only in Page or Edit mode and once a call has
           found its script, so a failing or script-less page does not hold up going back or closing. */
        private async Task BackAsync(int generation)
        {
            if (HasPage && _scriptPresent && EffectiveMode != GHWebNavigatorMode.Toolbar)
            {
                bool editing = _mode == GHWebNavigatorMode.Edit;
                string ended = await CallAsync(generation, "endEdit()");
                if (generation != _generation)
                    return;
                if (editing || ended == "D~1")
                {
                    HideKeyboard();
                    if (_mode == GHWebNavigatorMode.Edit)
                        SetMode(GHWebNavigatorMode.Page, -1);
                    return;
                }
                string closed = ended != null ? await CallAsync(generation, "closeLayer()") : null;
                if (generation != _generation)
                    return;
                if (closed == "D~1")
                    return;
            }

            bool canGoBack = false;
            try
            {
                canGoBack = CanGoBack != null && CanGoBack();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
            if (canGoBack && GoBack != null)
            {
                GHApp.PlayButtonClickedSound();
                GoBack();
            }
            else
            {
                Action fallback = _profile.BackFallback;
                if (fallback != null)
                    RunBackFallback(fallback);
                else
                    RunClosePage();
            }
        }

        /* Edit mode B */
        private async Task EndEditOperationAsync(int generation)
        {
            await EndEditAsync(generation);
        }

        /* Blurs the field being edited, closes the keyboard and returns to Page mode */
        private async Task EndEditAsync(int generation)
        {
            await CallAsync(generation, "endEdit()");
            if (generation != _generation)
                return;
            HideKeyboard();
            if (_mode == GHWebNavigatorMode.Edit)
                SetMode(GHWebNavigatorMode.Page, -1);
        }

        /* Edit mode A: reopens the keyboard for the field; a field no longer being edited is activated again */
        private async Task EditKeyboardAsync(int generation)
        {
            GHWebSnapshot s = await SnapshotAsync(generation);
            if (s == null)
                return;
            if (!s.Editing)
            {
                HideKeyboard();
                SetMode(GHWebNavigatorMode.Page, -1);
                string result = await CallAsync(generation, "activate()");
                if (result != null)
                    HandleActivation(result);
                return;
            }
            if (s.FocusPosition >= 0 && s.FocusPosition < s.Candidates.Count)
            {
                foreach (GHWebRect f in s.Candidates[s.FocusPosition].Fragments)
                {
                    if (f.IsEmpty)
                        continue;
                    FocusNativeView();
                    ShowKeyboard(f.X, f.Y, f.Width, f.Height);
                    return;
                }
            }
            /* The field is out of reach or inside another candidate: the rectangle from when the edit began */
            double x, y, width, height;
            if (TryParseRect(_editRect, out x, out y, out width, out height))
            {
                FocusNativeView();
                ShowKeyboard(x, y, width, height);
            }
        }

        /* Editing */

        /* rect: x,y,w,h of the field in CSS px from the viewport's top-left, or null */
        private void EnterEdit(bool multipleLines, string rect)
        {
            _editMultipleLines = multipleLines;
            _editRect = rect;
            SetMode(GHWebNavigatorMode.Edit, -1);
            FocusNativeView();
            double x, y, width, height;
            if (TryParseRect(rect, out x, out y, out width, out height))
                ShowKeyboard(x, y, width, height);
        }

        private void FocusNativeView()
        {
            try
            {
                IGHWebScriptHost host = _host;
                if (host != null)
                    host.FocusNative();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
        }

        /* Opens the on-screen keyboard away from a CSS px area of the viewport, converted to DIPs of the web view */
        private void ShowKeyboard(double x, double y, double width, double height)
        {
            double f = _devicePixelRatio / HostRenderScaling();
            GHScreenKeyboardMode mode = _editMultipleLines ? GHScreenKeyboardMode.MultipleLines
                : GHScreenKeyboardMode.SingleLine;
            if (GHScreenKeyboard.ShowForArea(_webView, x * f, y * f, width * f, height * f, mode))
                _keyboardOpened = true;
        }

        private void HideKeyboard()
        {
            if (!_keyboardOpened)
                return;
            _keyboardOpened = false;
            GHScreenKeyboard.Hide();
        }

        private static bool TryParseRect(string text, out double x, out double y, out double width, out double height)
        {
            x = 0.0;
            y = 0.0;
            width = 0.0;
            height = 0.0;
            if (string.IsNullOrEmpty(text))
                return false;
            string[] parts = text.Split(',');
            if (parts.Length != 4)
                return false;
            NumberStyles styles = NumberStyles.AllowLeadingSign | NumberStyles.AllowDecimalPoint;
            return double.TryParse(parts[0], styles, CultureInfo.InvariantCulture, out x)
                && double.TryParse(parts[1], styles, CultureInfo.InvariantCulture, out y)
                && double.TryParse(parts[2], styles, CultureInfo.InvariantCulture, out width)
                && double.TryParse(parts[3], styles, CultureInfo.InvariantCulture, out height);
        }

        private double HostRenderScaling()
        {
            IGHWebScriptHost host = _host;
            double scale = host != null ? host.RenderScaling : 1.0;
            return scale > 0.0 && !double.IsNaN(scale) && !double.IsInfinity(scale) ? scale : 1.0;
        }

        /* Sticks */

        /* Right stick, every gamepad tick: scrolls the page at a velocity in proportion to the push, in every mode,
           through scroll() calls that bypass the queue; a call is not sent while another is in flight */
        public void UpdateSticks(GHGamepadSticks sticks, double dtSeconds)
        {
            try
            {
                bool usable = _attached && HasPage;
                double vx = 0.0;
                double vy = 0.0;
                if (usable && !sticks.IsNeutral && GHGamepadService.IsActive
                    && !GHApp.PushingModalPage && !GHApp.IsSystemBrowserOpen)
                {
                    vx = StickVelocity(sticks.RightX);
                    vy = StickVelocity(sticks.RightY);
                }

                if (vx == 0.0 && vy == 0.0)
                {
                    if (_stickScrolling && usable && !_stickInFlight)
                        SendStickVelocity(0.0, 0.0);
                    return;
                }

                if (_stickInFlight)
                    return;
                long now = Stopwatch.GetTimestamp();
                double threshold = StickChangeThreshold * RightStickScrollSpeed;
                bool changed = !_stickScrolling || Math.Abs(vx - _stickSentX) > threshold
                    || Math.Abs(vy - _stickSentY) > threshold;
                bool due = (now - _stickSentTicks) >= (long)(StickResendSeconds * Stopwatch.Frequency);
                if (changed || due)
                    SendStickVelocity(vx, vy);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
        }

        private static double StickVelocity(float axis)
        {
            if (axis == 0f)
                return 0.0;
            double speed = RightStickScrollSpeed * Math.Pow(Math.Abs(axis), RightStickScrollExponent);
            return axis < 0f ? -speed : speed;
        }

        /* Velocities in DIP/s; sent in CSS px/s */
        private async void SendStickVelocity(double vx, double vy)
        {
            int callId = ++_stickCallId;
            try
            {
                bool stop = vx == 0.0 && vy == 0.0;
                _stickInFlight = true;
                _stickScrolling = !stop;
                _stickSentX = vx;
                _stickSentY = vy;
                _stickSentTicks = Stopwatch.GetTimestamp();
                string call;
                if (stop)
                {
                    call = "scroll(0,0,0)";
                }
                else
                {
                    double f = HostRenderScaling() / _devicePixelRatio;
                    call = "scroll(" + Num(vx * f) + "," + Num(vy * f) + ","
                        + Int(StickVelocityLifetimeMilliseconds) + ")";
                }
                await CallAsync(_generation, call);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
            finally
            {
                if (callId == _stickCallId)
                    _stickInFlight = false;
            }
        }

        /* A call still in flight for an old document no longer holds up the stick */
        private void ResetStick()
        {
            _stickCallId++;
            _stickInFlight = false;
            _stickScrolling = false;
            _stickSentX = 0.0;
            _stickSentY = 0.0;
            _stickSentTicks = 0;
        }

        /* Hints */

        private List<GHGamepadHint> BuildHints()
        {
            List<GHGamepadHint> hints = new List<GHGamepadHint>();
            bool canGoBack = false;
            bool canGoForward = false;
            try
            {
                canGoBack = CanGoBack != null && CanGoBack();
                canGoForward = CanGoForward != null && CanGoForward();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
            GHWebGamepadProfile profile = _profile;
            string backLabel = canGoBack ? "Back" : profile.BackFallbackLabel;
            bool compose = profile.ComposeKey != null;

            switch (EffectiveMode)
            {
            case GHWebNavigatorMode.Page:
                hints.Add(new GHGamepadHint(GHGamepadButton.A, "Open", false));
                hints.Add(new GHGamepadHint(GHGamepadButton.B, backLabel, false));
                if (compose)
                {
                    if (profile.ComposeLabel != null)
                        hints.Add(new GHGamepadHint(GHGamepadButton.X, profile.ComposeLabel, false));
                }
                else if (canGoForward)
                {
                    hints.Add(new GHGamepadHint(GHGamepadButton.X, "Forward", false));
                }
                hints.Add(GHGamepadHint.ForSymbol(GHGamepadHintSymbol.DPad, "Move"));
                hints.Add(GHGamepadHint.ForSymbol(GHGamepadHintSymbol.RightStick, "Scroll"));
                hints.Add(new GHGamepadHint(GHGamepadButton.View,
                    _hasSide && _lastArea != GHWebCandidate.SideArea ? "Sidebar" : "Toolbar", false));
                if (_hasSearch)
                    hints.Add(new GHGamepadHint(GHGamepadButton.Y, profile.SearchLabel, false));
                hints.Add(GHGamepadHint.Pair(GHGamepadButton.LB, GHGamepadButton.RB, profile.SectionLabel));
                hints.Add(GHGamepadHint.Pair(GHGamepadButton.LT, GHGamepadButton.RT, "Page"));
                if (profile.L3Label != null && !string.IsNullOrEmpty(profile.L3Call))
                    hints.Add(new GHGamepadHint(GHGamepadButton.L3, profile.L3Label, false));
                hints.Add(new GHGamepadHint(GHGamepadButton.Menu, profile.CloseLabel, false));
                break;
            case GHWebNavigatorMode.Edit:
                hints.Add(new GHGamepadHint(GHGamepadButton.B, "Done", false));
                hints.Add(GHGamepadHint.ForSymbol(GHGamepadHintSymbol.DPad, "Leave"));
                if (GHScreenKeyboard.IsAvailable)
                    hints.Add(new GHGamepadHint(GHGamepadButton.A, "Keyboard", false));
                break;
            default:
                hints.Add(new GHGamepadHint(GHGamepadButton.A, "Press", false));
                hints.Add(new GHGamepadHint(GHGamepadButton.B, backLabel, false));
                if (compose)
                {
                    if (HasPage && profile.ComposeLabel != null)
                        hints.Add(new GHGamepadHint(GHGamepadButton.X, profile.ComposeLabel, false));
                }
                else if (canGoForward)
                {
                    hints.Add(new GHGamepadHint(GHGamepadButton.X, "Forward", false));
                }
                hints.Add(GHGamepadHint.ForSymbol(GHGamepadHintSymbol.DPad, "Move"));
                if (HasPage)
                    hints.Add(new GHGamepadHint(GHGamepadButton.View, "Page", false));
                hints.Add(new GHGamepadHint(GHGamepadButton.Menu, profile.CloseLabel, false));
                break;
            }
            return hints;
        }

        private static bool SameHints(List<GHGamepadHint> a, List<GHGamepadHint> b)
        {
            if (a == null || b == null)
                return a == b;
            if (a.Count != b.Count)
                return false;
            int i;
            for (i = 0; i < a.Count; i++)
            {
                GHGamepadHint x = a[i];
                GHGamepadHint y = b[i];
                if (x.Button != y.Button || x.Label != y.Label || x.Hold != y.Hold || x.HasButton2 != y.HasButton2
                    || x.Button2 != y.Button2 || x.Symbol != y.Symbol || x.Highlight != y.Highlight
                    || x.LineBreakBefore != y.LineBreakBefore)
                    return false;
            }
            return true;
        }

        private void RaiseHintsChanged()
        {
            Action handler = HintsChanged;
            if (handler == null)
                return;
            try
            {
                handler();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
        }

        /* Page callbacks */

        private void RunClosePage()
        {
            try
            {
                if (ClosePage != null)
                    ClosePage();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
        }

        private static void RunBackFallback(Action fallback)
        {
            try
            {
                fallback();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex);
            }
        }

        /* Script arguments in invariant culture */

        private static string Num(double value)
        {
            return value.ToString("0.###", CultureInfo.InvariantCulture);
        }

        private static string Int(int value)
        {
            return value.ToString(CultureInfo.InvariantCulture);
        }

        /* A single-quoted JavaScript string literal */
        private static string Str(string value)
        {
            return "'" + (value ?? "").Replace("\\", "\\\\").Replace("'", "\\'") + "'";
        }
    }
}
