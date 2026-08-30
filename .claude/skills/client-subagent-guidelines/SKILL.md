---
name: client-subagent-guidelines
description: >-
  Subagent constraints specific to the GnollHack client repository. Covers the three build
  dependency chains that must not be parallelized across -- makedefs to generated headers
  to C# interop, des files to levcomp to dlb, and GnollHackX XAML to makedefsdroid to
  GnollHackM -- and why cross-layer interop work in this repository escalates to the deep
  tier far more often than elsewhere. Read when writing a plan's Subagent Use section for
  GnollHack.
---

The full skill lives in this repository's tool-neutral agent directory (`.agents/`),
which is shared with other AI coding agents. This file is only a pointer.

Read `.agents/skills/client_subagent_guidelines/SKILL.md` (path relative to the repository root) in full
before proceeding, and follow it. Any `references/` files it links are relative to that
same directory.