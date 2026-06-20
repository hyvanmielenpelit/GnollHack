---
name: maui_threading
description: Multi-threaded programming patterns in GnollHack's .NET MAUI frontend. Covers thread architecture, lock strategies (Monitor.TryEnter, lock, Interlocked), the IThreadSafeView pattern, ConcurrentQueue-based inter-thread communication, and platform-specific GPU rendering thread behavior.
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
- **Input**: UI Thread adds commands to a `ConcurrentQueue<string>`. Game thread dequeues.
- **State Updates**: Game thread modifies shared `MapData` under a `lock`.
- **Rendering**: Render thread attempts `Monitor.TryEnter(syncObject, 0)`. If it gets the lock, it copies `MapData`. If not, it re-renders the old frame to avoid stuttering.

## `IThreadSafeView`
- UI components implement this interface to receive thread-safe updates from the Game Thread via the `EventAggregator` pattern.
