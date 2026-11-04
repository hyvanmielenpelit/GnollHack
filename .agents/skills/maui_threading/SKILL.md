---
name: maui_threading
description: Multi-threaded programming patterns in GnollHack's .NET MAUI frontend. Covers thread architecture, lock strategies (Monitor.TryEnter, lock, Interlocked), the IThreadSafeView pattern, ConcurrentQueue-based inter-thread communication, and the guard every async void entry point needs.
---

# MAUI Threading

## Critical Rules
- **Never perform game logic on the UI thread**.
- **Never perform UI updates on the Game thread**.
- **Use `lock(syncObject)`** for writing shared state.
- **Use `Monitor.TryEnter`** for reading shared state on the render thread to prevent dropped frames.

## Thread Architecture
1. **Main UI Thread**: Handles MAUI layout, button taps, gestures.
2. **Game Thread**: Runs the C engine `moveloop()`. Blocks waiting for input.
3. **Render Thread (SkiaSharp)**: Draws the map canvas at 60 FPS.
4. **Audio Thread (FMOD)**: Handles sound mixing independently.

## Communication Patterns
- **Input**: The UI thread enqueues onto `ConcurrentQueue<GHRequest>`; the game thread dequeues. Replies travel back on `ConcurrentQueue<GHResponse>`, and `ConcurrentQueue<GHPost>` carries posts. (`GHApp.AchievementQueue` is a `ConcurrentQueue<int>`, and `FmodService` queues sound work on its own `ConcurrentQueue<GHSoundTask>`.)
- **State Updates**: Game thread modifies shared `MapData` under a `lock`.
- **Rendering**: Render thread attempts `Monitor.TryEnter(syncObject, 0)`. If it gets the lock, it copies `MapData`. If not, it re-renders the old frame to avoid stuttering.

## `async void` Entry Points

An exception that escapes an `async void` method, or an async lambda passed to `BeginInvokeOnMainThread`,
`+=` or an `Action`, is re-thrown on the UI thread and crashes the app on every platform.

- Give every such entry point a top-level `try`/`catch (Exception ex)` that first calls
  `GHApp.ReportCaughtException(ex)`, then undoes what the normal path undoes on its way out (re-enabled
  controls, hidden wait overlays). Where no state is safe to restore, rethrow after the report.
- Put the guard inside the lambda: a `try` around `BeginInvokeOnMainThread` catches nothing the lambda throws.
- Keep handlers thin; long bodies go in a `private async Task <HandlerName>Async` the handler awaits.
- A helper that only fires and forgets stays `async void` with the guard; `async Task` with a discarded
  result would hide the exception instead.

## `IThreadSafeView`
- UI components implement this interface so they can be updated safely from a
  non-UI thread. See `Controls/LabeledImageButton.xaml.cs` for the reference
  implementation.
- A view holds its parent as `WeakReference<IThreadSafeView>` (weak, to avoid
  leaking the visual tree), and the reference itself is swapped through
  `Interlocked.Exchange` / `Interlocked.CompareExchange` so reads and writes
  never tear across threads.
- There is **no** `EventAggregator` or message-bus type in this codebase; updates
  flow through the concurrent queues above and through direct thread-safe
  property access.

## Related Skills

- **`client_framework_sources`** — How MAUI and the platforms dispatch threads, read at the shipped version
