---
name: client-workarounds
description: >-
  How to record, re-check, and remove GnollHack's workarounds for defects and limitations
  in third-party components -- .NET MAUI, SkiaSharp, the .NET runtime, .NET for Android
  and iOS, WinUI, Avalonia, CEF, FMOD, Sentry, and the operating systems -- and the known
  issues it cannot work around, in the register in DEVEL/workarounds. Covers WA and KI
  entries, IDs, every field, version sources, upstream status, WORKAROUND(WA-...) tags,
  the lint, linking a known issue to its Sentry issue, re-checks after a package, SDK or
  workload upgrade, and removal or resolution. Read whenever you add, change or remove a
  workaround, conclude that a crash or defect is upstream's with no workaround, upgrade a
  NuGet package, SDK or workload, file or update an upstream issue, meet a
  WORKAROUND(WA-...) tag or a KI- ID, or are asked to re-check the register.
---

The full skill lives in this repository's tool-neutral agent directory (`.agents/`),
which is shared with other AI coding agents. This file is only a pointer.

Read `.agents/skills/client_workarounds/SKILL.md` (path relative to the repository root) in full
before proceeding, and follow it. Any `references/` files it links are relative to that
same directory.