---
name: client-workarounds
description: >-
  How to record, re-check, and remove GnollHack's workarounds for defects and limitations
  in third-party components -- .NET MAUI, SkiaSharp, the .NET runtime, .NET for Android
  and iOS, WinUI, Avalonia, CEF, FMOD, Sentry, and the operating systems -- in the
  workaround register in DEVEL/workarounds. Covers what counts as a workaround, choosing
  the component file and the next WA ID, filling every field, where each version number
  comes from, establishing upstream status, the WORKAROUND(WA-...) code tag, the lint
  script, re-checks after a package, SDK or workload upgrade, and removal. Read whenever
  you add, change or remove a workaround for a framework or OS bug, upgrade a NuGet
  package, SDK or workload, file or update an upstream issue, meet a WORKAROUND(WA-...)
  tag, or are asked to re-check the workaround register.
---

The full skill lives in this repository's tool-neutral agent directory (`.agents/`),
which is shared with other AI coding agents. This file is only a pointer.

Read `.agents/skills/client_workarounds/SKILL.md` (path relative to the repository root) in full
before proceeding, and follow it. Any `references/` files it links are relative to that
same directory.