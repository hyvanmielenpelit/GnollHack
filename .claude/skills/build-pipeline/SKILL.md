---
name: build-pipeline
description: Guide on building GnollHack native C libraries, processing assets with makedefs/levcomp/dgncomp/dlb, packaging nhdat, and compiling the C# .NET MAUI frontend for Android, iOS, and Windows, plus the Avalonia port (GnhAvalonia=true) for Linux (AppImage, Steam depot, tar.gz), macOS (GnollHack.app with Developer ID signing, notarization, DMG and Steam depot, built on the Mac over SSH) and Windows. Also covers the Apple engine libraries built over SSH and deploying the Linux build to a Steam Deck.
---

The full skill lives in this repository's tool-neutral agent directory (`.agents/`),
which is shared with other AI coding agents. This file is only a pointer.

Read `.agents/skills/build_pipeline/SKILL.md` (path relative to the repository root) in full
before proceeding, and follow it. Any `references/` files it links are relative to that
same directory.
