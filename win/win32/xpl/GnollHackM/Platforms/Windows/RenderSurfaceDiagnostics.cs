#if WINDOWS && GNH_MAUI
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading;
using GnollHackX;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using SkiaSharp.Views.Windows;

namespace GnollHackM
{
    /* Lifecycle counters for the Windows render surfaces: SkiaSharp's swap-chain panels (SKSwapChainPanel)
       and bitmap canvases (SKXamlCanvas). A ViewMapper entry attaches static event handlers to every such
       platform view; counts are kept per phase (in game or not), family, kind and owner. Occurrences that
       should not happen on the main menu become rate-limited breadcrumbs, and the totals go into the
       "Render surfaces:" summary and the Sentry event context. The window-bitmap detection follows
       SKXamlCanvas.CreateBitmap and FreeBitmap in SkiaSharp 4.153.1. */
    internal static class RenderSurfaceDiagnostics
    {
        private enum SurfaceKind
        {
            Created = 0,
            Loaded,
            Reloaded,
            Unloaded,
            SizeChanged,
            ScaleChanged,
            Paint,
            WindowBitmap,
        }

        private const int PhaseNoGame = 0;
        private const int PhaseInGame = 1;
        private const int FamilyPanel = 0;
        private const int FamilyCanvas = 1;
        private const int KindCount = 8;
        private const int MaxOwners = 16;
        private const int BreadcrumbsPerCell = 3;
        private const int MaxBreadcrumbs = 20;
        private const int MaxStackFramesScanned = 80;
        private const int MaxStackFramesShown = 12;
        private const string SummaryPrefix = "Render surfaces: ";
        private const string OtherOwner = "Other";

        /* Indexed by SurfaceKind */
        private static readonly char[] KindCodes = { 'N', 'L', 'R', 'U', 'S', 'C', 'P', 'W' };
        private static readonly string[] KindTexts =
            { "Created", "Loaded", "Loaded again", "Unloaded", "SizeChanged", "ScaleChanged", "Paint", "window bitmap" };
        private static readonly SurfaceKind[] PanelKinds =
        {
            SurfaceKind.Loaded, SurfaceKind.Unloaded, SurfaceKind.Reloaded, SurfaceKind.SizeChanged,
            SurfaceKind.ScaleChanged, SurfaceKind.Paint, SurfaceKind.Created
        };
        private static readonly SurfaceKind[] CanvasKinds =
        {
            SurfaceKind.Loaded, SurfaceKind.Unloaded, SurfaceKind.SizeChanged, SurfaceKind.Paint,
            SurfaceKind.Created, SurfaceKind.WindowBitmap
        };

        private sealed class SurfaceState
        {
            public int Id;
            public int Family;
            public int Owner;
            /* UI thread only */
            public int IsLoaded;
            public int LoadCount;
            public bool BitmapFreed;
            public int LastRawWidth;
            public int LastRawHeight;
            /* Interlocked */
            public int IsShutDown;
        }

        /* ((phase * 2 + family) * KindCount + kind) * MaxOwners + owner */
        private static readonly long[] _counts = new long[2 * 2 * KindCount * MaxOwners];
        private static readonly int[] _breadcrumbBudget = new int[2 * 2 * KindCount * MaxOwners];
        private static int _breadcrumbsTotal = 0;
        /* No-game totals over owners, family * KindCount + kind, for the milestone breadcrumbs */
        private static readonly long[] _noGameTotals = new long[2 * KindCount];
        private static readonly ConditionalWeakTable<object, SurfaceState> _surfaces =
            new ConditionalWeakTable<object, SurfaceState>();
        private static int _nextId = 0;
        private static int _loadedPanels = 0;
        private static int _registered = 0;

        private static readonly object _ownerLock = new object();
        private static readonly Dictionary<string, int> _ownerIndices = new Dictionary<string, int>();
        private static readonly string[] _ownerNames = new string[MaxOwners];

        /* Called once from MauiProgram before builder.Build() */
        public static void Register()
        {
            try
            {
                if (Interlocked.Exchange(ref _registered, 1) != 0)
                    return;
                Microsoft.Maui.Handlers.ViewHandler.ViewMapper.AppendToMapping("GnhRenderSurfaceDiagnostics",
                    (handler, view) => Attach(handler, view));
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        /* UI thread. Marks a SwitchableCanvasView's platform view (panel or canvas) as shut down;
           marking a Main panel adds a BuildSummary() breadcrumb, once per game, outside the cap. */
        public static void MarkShutDown(object platformView)
        {
            try
            {
                SurfaceState state;
                if (platformView == null || !_surfaces.TryGetValue(platformView, out state))
                    return;
                if (Interlocked.Exchange(ref state.IsShutDown, 1) != 0)
                    return;
                if (state.Family == FamilyPanel && GetOwnerName(state.Owner) == "Main")
                    GHApp.AddSentryBreadcrumb(BuildSummary(), GHConstants.SentryGnollHackGeneralCategoryName);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        /* Any thread. "Render surfaces: ..." summary of the panel and window-bitmap counts. */
        public static string BuildSummary()
        {
            try
            {
                string[] names = GetOwnerNames();
                StringBuilder sb = new StringBuilder(512);
                sb.Append(SummaryPrefix);
                AppendPhaseSummary(sb, PhaseNoGame, "no game", names);
                sb.Append("; ");
                AppendPhaseSummary(sb, PhaseInGame, "in game", null);
                long created = Sum(PhaseNoGame, FamilyPanel, SurfaceKind.Created)
                    + Sum(PhaseInGame, FamilyPanel, SurfaceKind.Created);
                sb.Append("; ").Append(created).Append(" panels created, ")
                    .Append(CountLive(FamilyPanel)).Append(" live, ")
                    .Append(Interlocked.CompareExchange(ref _loadedPanels, 0, 0)).Append(" loaded");
                return sb.ToString();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
                return SummaryPrefix + "unavailable";
            }
        }

        /* Any thread. Adds the surface_* fields to an event context. */
        public static void AddToContext(Dictionary<string, object> context)
        {
            if (context == null)
                return;
            try
            {
                string[] names = GetOwnerNames();
                context["surface_panels_created"] = Sum(PhaseNoGame, FamilyPanel, SurfaceKind.Created)
                    + Sum(PhaseInGame, FamilyPanel, SurfaceKind.Created);
                context["surface_panels_live"] = CountLive(FamilyPanel);
                context["surface_panels_loaded"] = (long)Interlocked.CompareExchange(ref _loadedPanels, 0, 0);
                context["surface_canvases_created"] = Sum(PhaseNoGame, FamilyCanvas, SurfaceKind.Created)
                    + Sum(PhaseInGame, FamilyCanvas, SurfaceKind.Created);
                context["surface_no_game_panels"] = FamilyKindsText(PhaseNoGame, FamilyPanel);
                context["surface_in_game_panels"] = FamilyKindsText(PhaseInGame, FamilyPanel);
                context["surface_no_game_canvases"] = FamilyKindsText(PhaseNoGame, FamilyCanvas);
                context["surface_in_game_canvases"] = FamilyKindsText(PhaseInGame, FamilyCanvas);
                context["surface_no_game_by_owner"] = ByOwnerText(PhaseNoGame, names);
                context["surface_in_game_by_owner"] = ByOwnerText(PhaseInGame, names);
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        /* UI thread, once per handler connection of every view */
        private static void Attach(Microsoft.Maui.IViewHandler handler, Microsoft.Maui.IView view)
        {
            try
            {
                object platformView = handler?.PlatformView;
                SKSwapChainPanel panel = platformView as SKSwapChainPanel;
                SKXamlCanvas canvas = panel == null ? platformView as SKXamlCanvas : null;
                if (panel == null && canvas == null)
                    return;

                SurfaceState state;
                if (_surfaces.TryGetValue(platformView, out state))
                {
                    state.Owner = ResolveOwner(view);
                    return;
                }

                state = new SurfaceState();
                state.Id = Interlocked.Increment(ref _nextId);
                state.Owner = ResolveOwner(view);
                if (panel != null)
                {
                    state.Family = FamilyPanel;
                    state.IsLoaded = panel.IsLoaded ? 1 : 0;
                    if (state.IsLoaded == 1)
                        Interlocked.Increment(ref _loadedPanels);
                }
                else
                {
                    state.Family = FamilyCanvas;
                    state.LoadCount = canvas.IsLoaded ? 1 : 0;
                    state.BitmapFreed = true;
                }
                _surfaces.AddOrUpdate(platformView, state);

                FrameworkElement element = (FrameworkElement)platformView;
                int phase = CurrentPhase(state);
                Count(phase, state, SurfaceKind.Created);
                if (ShouldLog(phase, state, SurfaceKind.Created))
                {
                    string text = Describe(state, element, phase, SurfaceKind.Created,
                        PanelSizeText(panel, element.ActualWidth, element.ActualHeight));
                    if (panel != null && phase == PhaseNoGame)
                        text += " from " + CallerStack();
                    AddBreadcrumb(text);
                }

                element.Loaded += OnLoaded;
                element.Unloaded += OnUnloaded;
                element.SizeChanged += OnSizeChanged;
                if (panel != null)
                {
                    panel.PaintSurface += OnPanelPaintSurface;
                    panel.CompositionScaleChanged += OnCompositionScaleChanged;
                }
                else
                {
                    canvas.PaintSurface += OnCanvasPaintSurface;
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        private static void OnLoaded(object sender, RoutedEventArgs e)
        {
            try
            {
                SurfaceState state;
                if (sender == null || !_surfaces.TryGetValue(sender, out state))
                    return;
                int phase = CurrentPhase(state);
                Count(phase, state, SurfaceKind.Loaded);
                if (state.Family == FamilyPanel)
                {
                    SurfaceKind logged = SurfaceKind.Loaded;
                    if (state.IsLoaded == 1)
                    {
                        Count(phase, state, SurfaceKind.Reloaded);
                        logged = SurfaceKind.Reloaded;
                    }
                    else
                    {
                        state.IsLoaded = 1;
                        Interlocked.Increment(ref _loadedPanels);
                    }
                    if (ShouldLog(phase, state, logged))
                    {
                        FrameworkElement element = (FrameworkElement)sender;
                        AddBreadcrumb(Describe(state, element, phase, logged,
                            PanelSizeText(sender as SKSwapChainPanel, element.ActualWidth, element.ActualHeight)));
                    }
                }
                else
                {
                    state.LoadCount++;
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        private static void OnUnloaded(object sender, RoutedEventArgs e)
        {
            try
            {
                SurfaceState state;
                if (sender == null || !_surfaces.TryGetValue(sender, out state))
                    return;
                int phase = CurrentPhase(state);
                Count(phase, state, SurfaceKind.Unloaded);
                if (state.Family == FamilyPanel)
                {
                    if (state.IsLoaded == 1)
                    {
                        state.IsLoaded = 0;
                        Interlocked.Decrement(ref _loadedPanels);
                    }
                }
                else
                {
                    state.LoadCount--;
                    if (state.LoadCount == 0)
                        state.BitmapFreed = true;
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        private static void OnSizeChanged(object sender, Microsoft.UI.Xaml.SizeChangedEventArgs e)
        {
            try
            {
                SurfaceState state;
                if (sender == null || !_surfaces.TryGetValue(sender, out state))
                    return;
                int phase = CurrentPhase(state);
                Count(phase, state, SurfaceKind.SizeChanged);
                if (state.Family == FamilyCanvas)
                {
                    if (e.NewSize.Width <= 0 || e.NewSize.Height <= 0)
                        state.BitmapFreed = true;
                }
                else if (ShouldLog(phase, state, SurfaceKind.SizeChanged))
                {
                    AddBreadcrumb(Describe(state, (FrameworkElement)sender, phase, SurfaceKind.SizeChanged,
                        PanelSizeText(sender as SKSwapChainPanel, e.NewSize.Width, e.NewSize.Height)));
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        private static void OnCompositionScaleChanged(SwapChainPanel sender, object args)
        {
            try
            {
                SurfaceState state;
                if (sender == null || !_surfaces.TryGetValue(sender, out state))
                    return;
                int phase = CurrentPhase(state);
                Count(phase, state, SurfaceKind.ScaleChanged);
                if (ShouldLog(phase, state, SurfaceKind.ScaleChanged))
                {
                    AddBreadcrumb(Describe(state, sender, phase, SurfaceKind.ScaleChanged,
                        PanelSizeText(sender as SKSwapChainPanel, sender.ActualWidth, sender.ActualHeight)));
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        private static void OnPanelPaintSurface(object sender, SKPaintGLSurfaceEventArgs e)
        {
            try
            {
                SurfaceState state;
                if (sender == null || !_surfaces.TryGetValue(sender, out state))
                    return;
                int phase = CurrentPhase(state);
                Count(phase, state, SurfaceKind.Paint);
                if (ShouldLog(phase, state, SurfaceKind.Paint))
                {
                    FrameworkElement element = (FrameworkElement)sender;
                    AddBreadcrumb(Describe(state, element, phase, SurfaceKind.Paint,
                        PanelSizeText(sender as SKSwapChainPanel, element.ActualWidth, element.ActualHeight)));
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        /* A paint follows a newly allocated bitmap when SkiaSharp freed the last one (full unload or zero
           size) or the pixel size differs from the last paint's */
        private static void OnCanvasPaintSurface(object sender, SKPaintSurfaceEventArgs e)
        {
            try
            {
                SurfaceState state;
                if (sender == null || !_surfaces.TryGetValue(sender, out state))
                    return;
                int phase = CurrentPhase(state);
                Count(phase, state, SurfaceKind.Paint);

                int rawWidth = e.RawInfo.Width;
                int rawHeight = e.RawInfo.Height;
                bool newBitmap = state.BitmapFreed || rawWidth != state.LastRawWidth || rawHeight != state.LastRawHeight;
                state.BitmapFreed = false;
                state.LastRawWidth = rawWidth;
                state.LastRawHeight = rawHeight;
                if (!newBitmap)
                    return;

                FrameworkElement element = (FrameworkElement)sender;
                XamlRoot root = element.XamlRoot;
                if (root == null)
                    return;
                double scale = root.RasterizationScale;
                double windowWidth = root.Size.Width * scale;
                double windowHeight = root.Size.Height * scale;
                double windowArea = windowWidth * windowHeight;
                if (windowArea <= 0 || (double)rawWidth * rawHeight < 0.5 * windowArea)
                    return;

                Count(phase, state, SurfaceKind.WindowBitmap);
                if (ShouldLog(phase, state, SurfaceKind.WindowBitmap))
                {
                    string sizeText = rawWidth.ToString(CultureInfo.InvariantCulture) + "x"
                        + rawHeight.ToString(CultureInfo.InvariantCulture) + " of "
                        + RoundText(windowWidth) + "x" + RoundText(windowHeight);
                    AddBreadcrumb(Describe(state, element, phase, SurfaceKind.WindowBitmap, sizeText));
                }
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.Message);
            }
        }

        private static int CurrentPhase(SurfaceState state)
        {
            if (Interlocked.CompareExchange(ref state.IsShutDown, 0, 0) != 0)
                return PhaseNoGame;
            GamePage page = GHApp.CurrentGamePage;
            return page != null && !page.CanvasViewsShutDown ? PhaseInGame : PhaseNoGame;
        }

        private static int CellIndex(int phase, int family, SurfaceKind kind, int owner)
        {
            return ((phase * 2 + family) * KindCount + (int)kind) * MaxOwners + owner;
        }

        private static void Count(int phase, SurfaceState state, SurfaceKind kind)
        {
            Interlocked.Increment(ref _counts[CellIndex(phase, state.Family, kind, state.Owner)]);
            if (phase != PhaseNoGame || !IsMilestoneKind(state.Family, kind))
                return;
            long total = Interlocked.Increment(ref _noGameTotals[state.Family * KindCount + (int)kind]);
            if (total != 8 && total != 64 && total != 512 && total != 4096 && total != 32768)
                return;
            if (Interlocked.Increment(ref _breadcrumbsTotal) > MaxBreadcrumbs)
                return;
            string summary = BuildSummary();
            if (summary.StartsWith(SummaryPrefix, StringComparison.Ordinal))
                summary = summary.Substring(SummaryPrefix.Length);
            AddBreadcrumb(SummaryPrefix + "no game " + FamilyText(state.Family) + " " + KindTexts[(int)kind]
                + " reached " + total.ToString(CultureInfo.InvariantCulture) + "; " + summary);
        }

        private static bool IsMilestoneKind(int family, SurfaceKind kind)
        {
            if (family == FamilyPanel)
            {
                return kind == SurfaceKind.Reloaded || kind == SurfaceKind.Created || kind == SurfaceKind.Loaded
                    || kind == SurfaceKind.SizeChanged;
            }
            return kind == SurfaceKind.WindowBitmap;
        }

        /* Panel Reloaded in either phase, panel Created, Loaded, SizeChanged, ScaleChanged and Paint and
           canvas WindowBitmap with no game; at most 3 per cell and 20 in all */
        private static bool ShouldLog(int phase, SurfaceState state, SurfaceKind kind)
        {
            bool eligible;
            if (state.Family == FamilyPanel)
            {
                eligible = kind == SurfaceKind.Reloaded
                    || (phase == PhaseNoGame && (kind == SurfaceKind.Created || kind == SurfaceKind.Loaded
                        || kind == SurfaceKind.SizeChanged || kind == SurfaceKind.ScaleChanged
                        || kind == SurfaceKind.Paint));
            }
            else
            {
                eligible = phase == PhaseNoGame && kind == SurfaceKind.WindowBitmap;
            }
            if (!eligible)
                return false;
            int cell = CellIndex(phase, state.Family, kind, state.Owner);
            if (Volatile.Read(ref _breadcrumbBudget[cell]) >= BreadcrumbsPerCell)
                return false;
            return Interlocked.Increment(ref _breadcrumbBudget[cell]) <= BreadcrumbsPerCell
                && Interlocked.Increment(ref _breadcrumbsTotal) <= MaxBreadcrumbs;
        }

        private static void AddBreadcrumb(string text)
        {
            GHApp.AddSentryBreadcrumb(text, GHConstants.SentryGnollHackGeneralCategoryName);
        }

        private static string Describe(SurfaceState state, FrameworkElement element, int phase, SurfaceKind kind,
            string sizeText)
        {
            StringBuilder sb = new StringBuilder(160);
            sb.Append("Render surface #").Append(state.Id).Append(' ').Append(FamilyText(state.Family))
                .Append(' ').Append(GetOwnerName(state.Owner)).Append(' ').Append(KindTexts[(int)kind])
                .Append(" (").Append(phase == PhaseNoGame ? "no game" : "in game");
            if (Interlocked.CompareExchange(ref state.IsShutDown, 0, 0) != 0)
                sb.Append(", shut down");
            sb.Append("): ").Append(sizeText).Append(", ")
                .Append(element.Visibility == Microsoft.UI.Xaml.Visibility.Visible ? "visible" : "collapsed");
            return sb.ToString();
        }

        private static string PanelSizeText(SKSwapChainPanel panel, double width, double height)
        {
            string text = RoundText(width) + "x" + RoundText(height);
            if (panel != null)
                text += " at " + panel.CompositionScaleX.ToString("0.##", CultureInfo.InvariantCulture);
            return text;
        }

        private static string RoundText(double value)
        {
            if (double.IsNaN(value) || double.IsInfinity(value))
                return "?";
            return Math.Round(value).ToString("0", CultureInfo.InvariantCulture);
        }

        private static string FamilyText(int family)
        {
            return family == FamilyPanel ? "panel" : "canvas";
        }

        /* Managed callers outside MAUI, WinUI, WinRT, the BCL and this class, innermost first */
        private static string CallerStack()
        {
            StackTrace trace = new StackTrace(1, false);
            int frameCount = Math.Min(trace.FrameCount, MaxStackFramesScanned);
            StringBuilder sb = new StringBuilder(512);
            int shown = 0;
            for (int i = 0; i < frameCount && shown < MaxStackFramesShown; i++)
            {
                MethodBase method = trace.GetFrame(i)?.GetMethod();
                Type type = method?.DeclaringType;
                if (type == null)
                    continue;
                if (type == typeof(RenderSurfaceDiagnostics) || type.DeclaringType == typeof(RenderSurfaceDiagnostics))
                    continue;
                string ns = type.Namespace ?? "";
                if (ns.StartsWith("Microsoft.Maui", StringComparison.Ordinal)
                    || ns.StartsWith("Microsoft.UI", StringComparison.Ordinal)
                    || ns.StartsWith("WinRT", StringComparison.Ordinal)
                    || ns.StartsWith("ABI.", StringComparison.Ordinal)
                    || ns.StartsWith("System", StringComparison.Ordinal))
                    continue;
                if (shown > 0)
                    sb.Append(" < ");
                sb.Append(type.Name).Append('.').Append(method.Name);
                shown++;
            }
            return shown > 0 ? sb.ToString() : "unknown";
        }

        /* The CanvasType of an enclosing SwitchableCanvasView, or the view's own type name */
        private static int ResolveOwner(Microsoft.Maui.IView view)
        {
            Microsoft.Maui.Controls.Element element = (view as Microsoft.Maui.Controls.Element)?.Parent;
            for (int level = 0; level < 4 && element != null; level++)
            {
                SwitchableCanvasView switchable = element as SwitchableCanvasView;
                if (switchable != null)
                    return GetOwnerIndex(CanvasTypeName(switchable.CanvasType));
                element = element.Parent;
            }
            return GetOwnerIndex(view == null ? "Unknown" : view.GetType().Name);
        }

        private static string CanvasTypeName(CanvasTypes canvasType)
        {
            switch (canvasType)
            {
            case CanvasTypes.General:
                return "General";
            case CanvasTypes.MainCanvas:
                return "Main";
            case CanvasTypes.CommandCanvas:
                return "Command";
            case CanvasTypes.MenuCanvas:
                return "Menu";
            case CanvasTypes.TextCanvas:
                return "Text";
            case CanvasTypes.TipCanvas:
                return "Tip";
            default:
                return canvasType.ToString();
            }
        }

        /* Fifteen named owners; later names share the last entry, Other */
        private static int GetOwnerIndex(string name)
        {
            lock (_ownerLock)
            {
                int index;
                if (_ownerIndices.TryGetValue(name, out index))
                    return index;
                if (_ownerIndices.Count >= MaxOwners - 1)
                {
                    if (!_ownerIndices.TryGetValue(OtherOwner, out index))
                    {
                        index = _ownerIndices.Count;
                        _ownerIndices[OtherOwner] = index;
                        _ownerNames[index] = OtherOwner;
                    }
                    return index;
                }
                index = _ownerIndices.Count;
                _ownerIndices[name] = index;
                _ownerNames[index] = name;
                return index;
            }
        }

        private static string GetOwnerName(int owner)
        {
            lock (_ownerLock)
            {
                return _ownerNames[owner] ?? OtherOwner;
            }
        }

        private static string[] GetOwnerNames()
        {
            lock (_ownerLock)
            {
                return (string[])_ownerNames.Clone();
            }
        }

        private static long Read(int phase, int family, SurfaceKind kind, int owner)
        {
            return Interlocked.Read(ref _counts[CellIndex(phase, family, kind, owner)]);
        }

        private static long Sum(int phase, int family, SurfaceKind kind)
        {
            long total = 0;
            for (int owner = 0; owner < MaxOwners; owner++)
                total += Read(phase, family, kind, owner);
            return total;
        }

        private static long CountLive(int family)
        {
            long live = 0;
            foreach (KeyValuePair<object, SurfaceState> entry in _surfaces)
            {
                if (entry.Value.Family == family)
                    live++;
            }
            return live;
        }

        /* "panels L.. U.. R.. S.. C.. P.. N.. (Main L.. R..), window bitmaps .. (ImageCarousel ..)";
           the owner lists only when names is given */
        private static void AppendPhaseSummary(StringBuilder sb, int phase, string phaseText, string[] names)
        {
            sb.Append(phaseText).Append(": panels");
            for (int k = 0; k < PanelKinds.Length; k++)
            {
                sb.Append(' ').Append(KindCodes[(int)PanelKinds[k]]).Append(Sum(phase, FamilyPanel, PanelKinds[k]));
            }
            if (names != null)
            {
                bool first = true;
                for (int owner = 0; owner < MaxOwners; owner++)
                {
                    if (names[owner] == null)
                        continue;
                    StringBuilder ownerText = new StringBuilder();
                    for (int k = 0; k < PanelKinds.Length; k++)
                    {
                        long value = Read(phase, FamilyPanel, PanelKinds[k], owner);
                        if (value != 0)
                            ownerText.Append(' ').Append(KindCodes[(int)PanelKinds[k]]).Append(value);
                    }
                    if (ownerText.Length == 0)
                        continue;
                    sb.Append(first ? " (" : ", ").Append(names[owner]).Append(ownerText);
                    first = false;
                }
                if (!first)
                    sb.Append(')');
            }
            sb.Append(", window bitmaps ").Append(Sum(phase, FamilyCanvas, SurfaceKind.WindowBitmap));
            if (names != null)
            {
                bool first = true;
                for (int owner = 0; owner < MaxOwners; owner++)
                {
                    if (names[owner] == null)
                        continue;
                    long value = Read(phase, FamilyCanvas, SurfaceKind.WindowBitmap, owner);
                    if (value == 0)
                        continue;
                    sb.Append(first ? " (" : ", ").Append(names[owner]).Append(' ').Append(value);
                    first = false;
                }
                if (!first)
                    sb.Append(')');
            }
        }

        private static string FamilyKindsText(int phase, int family)
        {
            SurfaceKind[] kinds = family == FamilyPanel ? PanelKinds : CanvasKinds;
            StringBuilder sb = new StringBuilder(96);
            for (int k = 0; k < kinds.Length; k++)
            {
                if (k > 0)
                    sb.Append(' ');
                sb.Append(KindCodes[(int)kinds[k]]).Append(Sum(phase, family, kinds[k]));
            }
            return sb.ToString();
        }

        /* "panel Main L.. U.. ...; canvas ImageCarousel L.. ..." for every owner and family with a nonzero count */
        private static string ByOwnerText(int phase, string[] names)
        {
            StringBuilder sb = new StringBuilder(512);
            for (int family = FamilyPanel; family <= FamilyCanvas; family++)
            {
                SurfaceKind[] kinds = family == FamilyPanel ? PanelKinds : CanvasKinds;
                for (int owner = 0; owner < MaxOwners; owner++)
                {
                    if (names[owner] == null)
                        continue;
                    bool any = false;
                    for (int kind = 0; kind < KindCount; kind++)
                    {
                        if (Read(phase, family, (SurfaceKind)kind, owner) != 0)
                        {
                            any = true;
                            break;
                        }
                    }
                    if (!any)
                        continue;
                    if (sb.Length > 0)
                        sb.Append("; ");
                    sb.Append(FamilyText(family)).Append(' ').Append(names[owner]);
                    for (int k = 0; k < kinds.Length; k++)
                    {
                        sb.Append(' ').Append(KindCodes[(int)kinds[k]]).Append(Read(phase, family, kinds[k], owner));
                    }
                }
            }
            return sb.ToString();
        }
    }
}
#endif
