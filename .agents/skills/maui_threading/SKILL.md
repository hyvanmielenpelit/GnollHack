---
name: maui_threading
description: Multi-threaded programming patterns in GnollHack's .NET MAUI frontend. Covers thread architecture, lock strategies (Monitor.TryEnter, lock, Interlocked), the IThreadSafeView pattern, ConcurrentQueue-based inter-thread communication, and platform-specific GPU rendering thread behavior.
---

# Multi-Threaded Programming in GnollHack MAUI

GnollHack uses multiple threads to run the C game engine, render graphics, process audio, and handle UI events simultaneously. Thread safety is achieved through a deliberate mix of lock-free atomic operations, non-blocking try-locks, and blocking locks — each chosen for its specific thread context.

## Thread Architecture

```
┌──────────────────────────────────────────────────────────┐
│  GAME THREAD (new Thread → GNHThreadProc)                │
│  - Runs native C game loop via P/Invoke                  │
│  - Receives callbacks from C core (GHGame.cs)            │
│  - Enqueues GHRequest → RequestQueue                     │
│  - Polls ResponseQueue for UI answers                    │
│  - Uses lock() for data writes (blocking OK here)        │
│  - Uses Interlocked for simple flag/counter updates      │
└─────────────┬───────────────────────────┬────────────────┘
              │ ConcurrentQueue           │ ConcurrentQueue
              │ <GHRequest>               │ <GHResponse>
              ▼                           ▲
┌──────────────────────────────────────────────────────────┐
│  UI THREAD (Main Thread / Dispatcher)                    │
│  - _pollingTimer: 60Hz IDispatcherTimer polls requests   │
│  - Dispatches UI actions from dequeued GHRequests        │
│  - Enqueues GHResponse back to game thread               │
│  - Creates/destroys XAML controls, updates Preferences   │
│  - Triggers canvas InvalidateSurface() for repaints      │
│  - _updateTimer: 0.5s for cursor blink, FPS, memory      │
└─────────────┬────────────────────────────────────────────┘
              │ PaintSurface event
              ▼
┌──────────────────────────────────────────────────────────┐
│  RENDER THREAD (PaintSurface handler)                    │
│  - CPU mode (SKCanvasView): runs on UI thread            │
│  - GPU mode (SKGLView): runs on separate GPU thread      │
│  - Uses Monitor.TryEnter for ALL data reads (non-block)  │
│  - Uses Interlocked for simple property reads            │
│  - NEVER calls lock() — would block rendering            │
│  - Reads IThreadSafeView for layout dimensions           │
└──────────────────────────────────────────────────────────┘

┌──────────────────────────────────────────────────────────┐
│  FMOD AUDIO THREAD (managed by FMOD Studio SDK)          │
│  - Separate thread for audio mixing and playback         │
│  - Callbacks invoked from GnollHack's sound functions    │
└──────────────────────────────────────────────────────────┘
```

### Thread Count by Platform and GPU Mode

| Platform | Game Thread | UI Thread | GPU Render Thread | FMOD Thread | Total |
|----------|-------------|-----------|-------------------|-------------|-------|
| Android (GPU on) | 1 | 1 | 1 (per SKGLView) | 1+ | 4+ |
| Android (GPU off) | 1 | 1 | 0 (PaintSurface on UI) | 1+ | 3+ |
| iOS (GPU on) | 1 | 1 | 1 (per SKGLView) | 1+ | 4+ |
| iOS (GPU off) | 1 | 1 | 0 | 1+ | 3+ |
| Windows (GPU on) | 1 | 1 | 1 (SwapChain panel) | 1+ | 4+ |
| Windows (GPU off) | 1 | 1 | 0 | 1+ | 3+ |

GPU mode is controlled by `GHApp.IsGPUDefault` (true by default in MAUI builds). Low-memory Android devices may disable auxiliary GPU canvases via `IsDisableAuxGPUDefault`. The `SwitchableCanvasView` toggles between `SKCanvasView` (CPU) and `SKGLView` (GPU) at runtime.

---

## Synchronization Primitives Used

The codebase uses exactly **four** synchronization mechanisms:

| Primitive | Usage | When to Use |
|-----------|-------|-------------|
| `Monitor.TryEnter` / `Monitor.Exit` | 40+ lock objects in GamePage.xaml.cs | **Render thread** — non-blocking reads of shared data |
| `lock (obj)` | GHGame.cs, controls, app logic | **Game thread / UI thread** — blocking writes to shared data |
| `Interlocked.CompareExchange` / `Exchange` | 450+ usages across controls | **Any thread** — lock-free reads/writes of simple values |
| `ConcurrentQueue<T>` | RequestQueue, ResponseQueue, AchievementQueue | **Inter-thread messaging** — producer/consumer queues |

**Not used**: `SemaphoreSlim`, `Mutex`, `ManualResetEvent`, `AutoResetEvent`, `ReaderWriterLockSlim`.

---

## Pattern 1: Monitor.TryEnter (Non-Blocking Lock for Rendering)

**Purpose**: The render thread (`PaintSurface` handler) must never block — a blocked render thread causes frame drops. `Monitor.TryEnter` attempts to acquire the lock instantly; if it fails, the code skips that section and continues rendering.

**Pattern** (from [GamePage.xaml.cs:7304-7318](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/Pages/Game/GamePage.xaml.cs#L7304)):

```csharp
bool lockTaken = false;
try
{
    Monitor.TryEnter(_savedCanvasLock, ref lockTaken);
    if (lockTaken)
    {
        // Read shared data safely — only executes if lock was acquired
        _savedCanvasWidth = canvaswidth;
        _savedCanvasHeight = canvasheight;
    }
    // If !lockTaken: skip this section, use stale data, render continues
}
finally
{
    if (lockTaken)
        Monitor.Exit(_savedCanvasLock);
}
lockTaken = false;  // Reset for next lock in the same method
```

**Lock objects in GamePage.xaml.cs** (declared as `private readonly object`):

| Lock Object | Protects |
|-------------|----------|
| `_savedCanvasLock` | Canvas dimensions |
| `_clipLock` | Clip region coordinates |
| `_mapOffsetLock` | Map scroll offset |
| `_statusOffsetLock` | Status bar offset |
| `_floatingTextLock` | Floating damage text list |
| `_screenTextLock` | Screen text overlays |
| `_conditionTextLock` | Condition status text |
| `_screenFilterLock` | Screen filter effects |
| `_guiEffectLock` | GUI visual effects |
| `_canvasPointerLock` | Mouse/touch pointer state |
| `_tileSizeLock` | Tile rendering dimensions |
| `_uiRectLock` | UI element rectangles |
| `_savedMenuCanvasLock` | Menu canvas state |
| `_savedTextCanvasLock` | Text canvas state |
| `_savedCommandCanvasLock` | Command canvas state |
| `_uLock` | Player position for rendering |

**Lock objects in GHGame.cs** (used with both `lock()` and `Monitor.TryEnter`):

| Lock Object | Protects |
|-------------|----------|
| `_mapDataBufferLock` | Double-buffered map data (MapData, ObjectData arrays) |
| `StatusFieldLock` | Status line fields |
| `AnimationTimerLock` | Animation timer counters |
| `_weaponStyleObjDataItemLock` | Weapon style display data |
| `_petDataLock` | Pet information overlay data |
| `_contextMenuDataLock` | Context menu entries |
| `_quickLock` | Quick action display data |

**Rule**: The **game thread** side writes with `lock()` (blocking OK), while the **render thread** side reads with `Monitor.TryEnter` (non-blocking). This avoids deadlocks and rendering stalls.

---

## Pattern 2: lock() Statement (Blocking Lock)

**Purpose**: Used on the game thread and UI thread where blocking is acceptable. The game thread can wait because it doesn't affect frame rate.

**Pattern** (from [GHGame.cs:863-868](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/GHGame.cs#L863)):

```csharp
lock (_mapDataBufferLock)
{
    _mapCursorX = x;
    _mapCursorY = y;
}
```

**Key rule**: Game thread uses `lock()` to write → Render thread uses `Monitor.TryEnter` to read the **same lock object**. This is the fundamental reader/writer asymmetry in GnollHack.

---

## Pattern 3: Interlocked (Lock-Free Atomics)

**Purpose**: Lock-free thread-safe access to simple values (int, double, bool, object references). Avoids lock overhead entirely for frequently accessed properties.

### Thread-Safe Boolean Property

```csharp
private int _useGL = 0;  // int backing field, not bool

public bool UseGL
{
    get { return Interlocked.CompareExchange(ref _useGL, 0, 0) != 0; }
    set { Interlocked.Exchange(ref _useGL, value ? 1 : 0); }
}
```

`Interlocked` only works with `int`, `long`, `float`, `double`, and `object` — so `bool` values use an `int` backing field with 0/1 encoding.

### Thread-Safe Double Property

```csharp
private double _threadSafeWidth;

public double ThreadSafeWidth
{
    get { return Interlocked.CompareExchange(ref _threadSafeWidth, 0.0, 0.0); }
    private set { Interlocked.Exchange(ref _threadSafeWidth, value); }
}
```

`CompareExchange(ref field, 0.0, 0.0)` is an idiom that atomically reads the value without modifying it (it only changes if the current value equals the comparand 0.0, replacing it with 0.0 — a no-op).

### Thread-Safe Object Reference

```csharp
private GHWindow _gHWindow;

public GHWindow GHWindow
{
    get { return Interlocked.CompareExchange(ref _gHWindow, null, null); }
    set { Interlocked.Exchange(ref _gHWindow, value); }
}
```

### Atomic Flag (Compare-and-Swap)

```csharp
// Set _shutDown to 1 only if it is currently 0 (returns old value)
if (Interlocked.CompareExchange(ref _shutDown, 1, 0) == 0)
{
    // First call — perform shutdown work
}
```

---

## Pattern 4: IThreadSafeView Interface

**Purpose**: MAUI UI properties (`Width`, `Height`, `X`, `Y`, `IsVisible`, `Margin`) are only safe to read from the UI thread. The render thread needs these values for layout calculations. `IThreadSafeView` provides `Interlocked`-backed copies that any thread can safely read.

**Interface** ([IThreadSafeView.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/IThreadSafeView.cs)):

```csharp
public interface IThreadSafeView
{
    double ThreadSafeWidth { get; }
    double ThreadSafeHeight { get; }
    double ThreadSafeX { get; }
    double ThreadSafeY { get; }
    bool ThreadSafeIsVisible { get; }
    Thickness ThreadSafeMargin { get; }
    WeakReference<IThreadSafeView> ThreadSafeParent { get; }
}
```

**Implementations**: `SwitchableCanvasView`, `LabeledImageButton`, `SimpleImageButton`, `GHCachedImage`, `MeasurableGrid`, `MeasurableStackLayout`.

**Rule**: On the render/GPU thread, always use `control.ThreadSafeWidth` instead of `control.Width`.

**Note on Thickness**: `Thickness` is a struct that cannot be atomically exchanged via `Interlocked`. Implementations use a small `_propertyLock` object with `lock()` for `ThreadSafeMargin`:

```csharp
private readonly object _propertyLock = new object();
public Thickness ThreadSafeMargin
{
    get { lock (_propertyLock) { return _threadSafeMargin; } }
    private set { lock (_propertyLock) { _threadSafeMargin = value; } }
}
```

---

## Pattern 5: ConcurrentQueue Inter-Thread Communication

**Purpose**: The game thread and UI thread communicate via producer/consumer queues. This avoids shared mutable state for complex operations.

**Declarations** ([GHGame.cs:74-75](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/GHGame.cs#L74)):

```csharp
public readonly ConcurrentQueue<GHRequest> RequestQueue = new ConcurrentQueue<GHRequest>();
public readonly ConcurrentQueue<GHResponse> ResponseQueue = new ConcurrentQueue<GHResponse>();
```

**Data flow**:
1. **Game thread → UI thread**: Native C callbacks in GHGame enqueue `GHRequest` objects
2. **UI polling**: `_pollingTimer` (60Hz) → `DoPolling()` → `PollRequestQueue()` dequeues and dispatches
3. **UI thread → Game thread**: UI enqueues `GHResponse` when user provides input
4. **Game thread polling**: `PollResponseQueue()` in blocking wait loops for GetChar, PosKey, GetLine, SelectMenu

**Game thread blocking wait** ([GHGame.cs:1149-1158](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/GHGame.cs#L1149)):

```csharp
RequestQueue.Enqueue(new GHRequest(this, GHRequestType.GetChar));
while (_inputBufferLocation < 0)
{
    Thread.Sleep(GHConstants.PollingInterval);  // 15ms
    PollResponseQueue();
}
```

The game thread uses **`Thread.Sleep` polling loops** — not `ManualResetEvent` or `AutoResetEvent`. This is intentional: it keeps FMOD sound polling alive during waits (`GHApp.FmodService?.PollTasks()` is called alongside `PollResponseQueue`).

**Key constants** (from [GHConstants.cs](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/GHConstants.cs)):

| Constant | Value | Purpose |
|----------|-------|----------|
| `PollingFrequency` | 60 | UI `_pollingTimer` fires 60 times/second (~16.67ms interval) |
| `PollingInterval` | 15 | Game thread `Thread.Sleep(15)` between response polls |

**Additional queues**:
- `GHApp.AchievementQueue` (`ConcurrentQueue<int>`) — queued from game thread, processed on UI thread
- `FmodService._soundTasks` (`ConcurrentQueue<GHSoundTask>`) — sound tasks polled by `PollTasks()` on game thread

---

## Pattern 6: MainThread.BeginInvokeOnMainThread

**Purpose**: Marshal work from the game thread or render thread to the MAUI UI thread. Required for XAML control manipulation and `Preferences` API access.

```csharp
// From game thread callback — save a preference (Preferences API requires main thread)
MainThread.BeginInvokeOnMainThread(() =>
{
    Preferences.Set("LastUsedPlayerName", playerName);
});
```

**When required**: Creating/modifying XAML controls, accessing `Preferences`, updating navigation, dismissing pages. Used 119+ times across the codebase.

---

## Double-Buffering for Map Data

The map rendering uses a double-buffer to avoid contention between the game thread writing map updates and the render thread reading map data:

```
Game Thread:                    Render Thread:
┌────────────────┐              ┌────────────────────┐
│ _mapDataMaster │ ─── copy ──→ │ _mapDataBuffer1    │
│ (writes here)  │     under    │ or _mapDataBuffer2 │
└────────────────┘     lock()   └────────────────────┘
                                          │ Monitor.TryEnter
                                          │ reads _mapDataCurrent
                                          ▼
                                   Canvas rendering
```

The game thread copies `_mapDataMaster` into one of two buffers under `lock(_mapDataBufferLock)`, then swaps `_mapDataCurrent` to point to the updated buffer. The render thread reads `_mapDataCurrent` using `Monitor.TryEnter` — if the lock is held, it simply renders the previous frame's data.

---

## GPU Mode and the Render Thread

The [SwitchableCanvasView](file:///c:/hmp/GnollHack/win/win32/xpl/GnollHackX/GnollHackX/Controls/SwitchableCanvasView.xaml.cs) toggles between:

| Mode | View | Render Thread | Notes |
|------|------|---------------|-------|
| CPU | `SKCanvasView` | `PaintSurface` runs on **UI thread** | Simpler, fewer threading issues |
| GPU | `SKGLView` | `PaintSurface` runs on **separate GPU thread** | Better performance, requires full thread safety |

**In GPU mode**, the `PaintSurface` handler runs on a thread separate from the UI thread. This is why all data accessed in `PaintSurface` must use `Monitor.TryEnter` or `Interlocked` — never bare field access or `lock()`.

On Android, GPU rendering uses OpenGL ES on a dedicated GL thread. Low-memory devices (`TotalMemory < DisableAuxGPUbyDefaultThresholdInBytes`) may disable GPU for auxiliary canvases to save resources.

---

## Rules for Thread-Safe Code

### On the Game Thread (GHGame callbacks)
- ✅ Use `lock(lockObj)` for writing shared data
- ✅ Use `Interlocked.Exchange` for updating simple values
- ✅ Enqueue `GHRequest` to `RequestQueue` for UI actions
- ✅ Use `MainThread.BeginInvokeOnMainThread()` for Preferences or XAML access
- ❌ Never directly modify XAML controls

### On the UI Thread (polling, event handlers)
- ✅ Access XAML controls directly
- ✅ Use `lock(lockObj)` for brief critical sections
- ✅ Dequeue `RequestQueue`, enqueue `ResponseQueue`
- ✅ Call `InvalidateSurface()` to trigger repaints

### On the Render Thread (PaintSurface handler)
- ✅ Use `Monitor.TryEnter` for all shared data reads — skip if lock not available
- ✅ Use `Interlocked.CompareExchange` for simple value reads
- ✅ Use `IThreadSafeView` properties for control layout dimensions
- ✅ Always reset `lockTaken = false` between successive lock attempts
- ❌ Never use `lock()` — will block rendering and cause frame drops
- ❌ Never access MAUI UI properties directly (use `ThreadSafe*` variants)
- ❌ Never enqueue to `RequestQueue` or `ResponseQueue`
