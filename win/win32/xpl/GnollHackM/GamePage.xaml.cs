using GnollHackX;
using SkiaSharp;
using System.Diagnostics;

namespace GnollHackM;

public partial class GamePage : ContentPage
{
	public GamePage()
	{
		InitializeComponent();
	}

    public GamePage(MainPage mainPage) : base()
    {

    }

    public void StartGame()
    {

    }

    public ClientGame ClientGame { get; set; }
    public bool EnableWizardMode { get; set; }
    public bool EnableCasualMode { get; set; }
    public bool EnableModernMode { get; set; }
    public bool DrawWallEnds { get; set; }


    public bool MapNoClipMode { get; set; }
    public bool MapLookMode { get; set; }
    public bool ZoomMiniMode { get; set; }
    public bool ForceAscii { get; set; }
    public List<string> ExtendedCommands { get; set; }

    public void ClearConditionTexts()
    {

    }

    public void ClearFloatingTexts()
    {

    }

    public void ClearGuiEffects()
    {

    }

    public void StopWaitAndResumeSavedGame()
    {

    }

    public void SaveGameAndWaitForResume()
    {

    }

    public void ClearMap()
    {

    }

    public void SetMapCursor(int x, int y)
    {

    }

    public void SetMapSymbol(int x, int y, int glyph, int bkglyph, int c, int color, uint special, LayerInfo layers)
    {
    }

    public float GetTextScale()
    {
        return 1.0f;
    }

    public IGnollHackService GnollHackService { get; set; }

    public SKBitmap[] TileMap { get; set; }

    public readonly object AnimationTimerLock = new object();
    public GHAnimationTimerList AnimationTimers = new GHAnimationTimerList();
    public readonly object StatusFieldLock = new object();
    public readonly GHStatusField[] StatusFields = new GHStatusField[(int)statusfields.MAXBLSTATS];

    public MapRefreshRateStyle MapRefreshRate;

    public double CurrentPageWidth;
    public double CurrentPageHeight;

    private readonly object _mainCounterLock = new object();
    private long _mainCounterValue = 0;
    public long MainCounterValue { get { lock (_mainCounterLock) { return _mainCounterValue; } } }

    public readonly object _screenTextLock = new object();
    public GHScreenText _screenText = null;

    public void UpdateCursor(int style, int force_paint, int show_on_u)
    {

    }

    public void AddObjectData(int x, int y, obj otmp, int cmdtype, int where, objclassdata otypdata, ulong oflags)
    {
    }

    public void ClearPetData()
    {
    }

    public void AddPetData(monst_info monster_data)
    {
    }

    public void ClearAllObjectData(int x, int y)
    {

    }

    public void SetTargetClip(int x, int y, bool force)
    {

    }

    public void Suspend()
    {

    }

    public void Resume()
    {

    }

    public void DrawAutoDraw(int autodraw, SKCanvas canvas, SKPaint paint, ObjectDataItem otmp_round,
    int layer_idx, int mapx, int mapy,
    bool tileflag_halfsize, bool tileflag_normalobjmissile, bool tileflag_fullsizeditem,
    float tx, float ty, float width, float height,
    float scale, float targetscale, float scaled_x_padding, float scaled_y_padding, float scaled_tile_height,
    bool is_inventory, bool drawwallends)
    {

    }
    public void UpdateMainCanvas()
    {

    }

    public void UpdateCommandCanvas()
    {

    }

    public void UpdateMenuCanvas()
    {

    }

    public void UpdateTextCanvas()
    {

    }

    public void IncrementCounters()
    {

    }

    private void ContentPage_Appearing(object sender, EventArgs e)
    {

    }

    private void ContentPage_Disappearing(object sender, EventArgs e)
    {

    }


    public TTYCursorStyle CursorStyle { get; set; }
    public GHGraphicsStyle GraphicsStyle { get; set; }

    public bool ShowMemoryUsage { get; set; }
    public bool UseMainGLCanvas
    {
        get { return false; }
        set
        {
        }
    }
    private bool _useSimpleCmdLayout = true;
    public bool UseSimpleCmdLayout
    {
        get { return _useSimpleCmdLayout; }
        set
        {
        }
    }

    public bool ShowFPS { get; set; }
    private double _fps;
    private long _counterValueDiff;
    private long _previousMainFPSCounterValue = 0L;
    private long _previousCommandFPSCounterValue = 0L;
    private readonly object _fpslock = new object();
    private Stopwatch _stopWatch = new Stopwatch();
    private Stopwatch _mapUpdateStopWatch = new Stopwatch();

    private Stopwatch _animationStopwatch = new Stopwatch();
    private TimeSpan _previousTimeSpan;

    private readonly object _mapGridLock = new object();
    private bool _mapGrid = false;
    public bool MapGrid { get { lock (_mapGridLock) { return _mapGrid; } } set { lock (_mapGridLock) { _mapGrid = value; } } }

    private readonly object _hitPointBarLock = new object();
    private bool _hitPointBars = false;
    public bool HitPointBars { get { lock (_hitPointBarLock) { return _hitPointBars; } } set { lock (_hitPointBarLock) { _hitPointBars = value; } } }

    private readonly object _orbLock = new object();
    private bool _showOrbs = true;
    public bool ShowOrbs { get { lock (_orbLock) { return _showOrbs; } } set { lock (_orbLock) { _showOrbs = value; } } }
    private bool _showMaxHealthInOrb = false;
    public bool ShowMaxHealthInOrb { get { lock (_orbLock) { return _showMaxHealthInOrb; } } set { lock (_orbLock) { _showMaxHealthInOrb = value; } } }
    private bool _showMaxManaInOrb = false;
    public bool ShowMaxManaInOrb { get { lock (_orbLock) { return _showMaxManaInOrb; } } set { lock (_orbLock) { _showMaxManaInOrb = value; } } }

    private readonly object _playerMarkLock = new object();
    private bool _playerMark = false;
    public bool PlayerMark { get { lock (_playerMarkLock) { return _playerMark; } } set { lock (_playerMarkLock) { _playerMark = value; } } }

    private readonly object _monsterTargetingLock = new object();
    private bool _monsterTargeting = false;
    public bool MonsterTargeting { get { lock (_monsterTargetingLock) { return _monsterTargeting; } } set { lock (_monsterTargetingLock) { _monsterTargeting = value; } } }

    private readonly object _walkArrowLock = new object();
    private bool _walkArrows = true;
    public bool WalkArrows { get { lock (_walkArrowLock) { return _walkArrows; } } set { lock (_walkArrowLock) { _walkArrows = value; } } }

    private readonly object _classicStatusBarLock = new object();
    private bool _classicStatusBar = true;
    public bool ClassicStatusBar { get { lock (_classicStatusBarLock) { return _classicStatusBar; } } set { lock (_classicStatusBarLock) { _classicStatusBar = value; } } }

    private readonly object _showPetsLock = new object();
    private bool _showPets = false;
    public bool ShowPets { get { lock (_showPetsLock) { return _showPets; } } set { lock (_showPetsLock) { _showPets = value; } } }

    private bool _cursorIsOn;
    private bool _showDirections = false;
    private bool _showNumberPad = false;
    private bool ShowNumberPad { get { return _showNumberPad; } set { _showNumberPad = value; } }
    private bool _showWaitIcon = false;
    public bool ShowWaitIcon { get { return _showWaitIcon; } set { _showWaitIcon = value; } }

    private MainPage _mainPage;


    private game_cursor_types _cursorType;
    private bool _force_paint_at_cursor;
    private bool _show_cursor_on_u;

    private readonly object _objectDataLock = new object();

    private ObjectDataItem[] _weaponStyleObjDataItem = new ObjectDataItem[3];
    private readonly object _weaponStyleObjDataItemLock = new object();
    private bool _drawWeaponStyleAsGlyphs = true;

    private readonly object _petDataLock = new object();

    private int _shownMessageRows = GHConstants.DefaultMessageRows;
    public int NumDisplayedMessages { get { return _shownMessageRows; } set { _shownMessageRows = value; } }
    public int ActualDisplayedMessages { get { return ForceAllMessages ? GHConstants.AllMessageRows : NumDisplayedMessages; } }

    private int _shownPetRows = GHConstants.DefaultPetRows;
    public int NumDisplayedPetRows { get { return _shownPetRows; } set { _shownPetRows = value; } }
    private readonly object _isSizeAllocatedProcessedLock = new object();
    private bool _isSizeAllocatedProcessed = false;
    public bool IsSizeAllocatedProcessed { get { lock (_isSizeAllocatedProcessedLock) { return _isSizeAllocatedProcessed; } } set { lock (_isSizeAllocatedProcessedLock) { _isSizeAllocatedProcessed = value; } } }


    private readonly object _forceAllMessagesLock = new object();
    private bool _forceAllMessages = false;
    public bool ForceAllMessages { get { lock (_forceAllMessagesLock) { return _forceAllMessages; } } set { lock (_forceAllMessagesLock) { _forceAllMessages = value; } } }

    public bool HasAllMessagesTransparentBackground { get; set; } = true;

    private readonly object _showExtendedStatusBarLock = new object();
    private bool _showExtendedStatusBar = false;
    public bool ShowExtendedStatusBar { get { lock (_showExtendedStatusBarLock) { return _showExtendedStatusBar; } } set { lock (_showExtendedStatusBarLock) { _showExtendedStatusBar = value; } } }

    private readonly object _lighterDarkeningLock = new object();
    private bool _lighterDarkening = false;
    public bool LighterDarkening { get { lock (_lighterDarkeningLock) { return _lighterDarkening; } } set { lock (_lighterDarkeningLock) { _lighterDarkening = value; } } }

    private readonly object _accurateLayerDrawingLock = new object();
    private bool _accurateLayerDrawing = false;
    public bool AlternativeLayerDrawing { get { lock (_accurateLayerDrawingLock) { return _accurateLayerDrawing; } } set { lock (_accurateLayerDrawingLock) { _accurateLayerDrawing = value; } } }

    public readonly object RefreshScreenLock = new object();
    private bool _refreshScreen = true;
    public bool RefreshScreen
    {
        get { return _refreshScreen; }
        set { _refreshScreen = value; }
    }

    public int MoreCmdPage;
    public int ShownTip;
    public void ShowGUITips(bool param)
    {

    }

    public void GenericButton_Clicked(object sender, EventArgs e, int cmd)
    {

    }
}