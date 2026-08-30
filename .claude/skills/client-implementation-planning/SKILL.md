---
name: client-implementation-planning
description: >-
  Implementation planning overlay for the GnollHack client repository. Covers when a plan
  is required across the C core, native bridge, MAUI frontend, build utilities, and game
  data; the makedefs, levcomp, dgncomp, dlb, and makedefsdroid regeneration boundaries a
  plan step must never straddle; the XAML generation pipeline from GnollHackX to
  GnollHackM; and C-to-C# interop triggers. Read before any multi-file or cross-subsystem
  GnollHack change.
---

The full skill lives in this repository's tool-neutral agent directory (`.agents/`),
which is shared with other AI coding agents. This file is only a pointer.

Read `.agents/skills/client_implementation_planning/SKILL.md` (path relative to the repository root) in full
before proceeding, and follow it. Any `references/` files it links are relative to that
same directory.