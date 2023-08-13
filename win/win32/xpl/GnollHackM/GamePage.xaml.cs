using GnollHackX;
using SkiaSharp;

namespace GnollHackM;

public partial class GamePage : ContentPage
{
	public GamePage()
	{
		InitializeComponent();
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

    private void ContentPage_Appearing(object sender, EventArgs e)
    {

    }

    private void ContentPage_Disappearing(object sender, EventArgs e)
    {

    }
}