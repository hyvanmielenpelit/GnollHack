---
name: client-framework-sources
description: >-
  Where the source code of GnollHack's frameworks is and how to read it at the version
  GnollHack ships: .NET MAUI, the .NET runtime, .NET for Android, .NET for iOS, WinUI,
  CsWinRT, SkiaSharp, Skia, and HarfBuzz. Covers finding the local clones, which may sit
  in a different folder on each computer, mapping GnollHack's package and workload
  versions to a tag or commit, reading that version without changing the clone, and
  deciding whether a bug is GnollHack's or the framework's. Read when a stack trace,
  exception, or behavior leads into framework code, before working around a framework,
  and before drafting an upstream issue.
---

The full skill lives in this repository's tool-neutral agent directory (`.agents/`),
which is shared with other AI coding agents. This file is only a pointer.

Read `.agents/skills/client_framework_sources/SKILL.md` (path relative to the repository root) in full
before proceeding, and follow it. Any `references/` files it links are relative to that
same directory.