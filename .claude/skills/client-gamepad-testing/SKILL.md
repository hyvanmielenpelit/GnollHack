---
name: client-gamepad-testing
description: >-
  How gamepad and game controller work in GnollHack is tested on real devices: which device a change
  must be tested on, decided by whether it is shared controller code, tested on the Steam Deck as the
  primary device through the Avalonia Linux build, or specific to one platform, tested there; the
  Xbox controller on the Avalonia Windows build as a backup; the packaging and Deploy-SteamDeck.ps1
  sequence, what to do when the Deck cannot be reached, what the user must test, and how to read the
  Deck's logs afterward. Read whenever you plan, change or review anything that alters what a
  controller does or shows: GHGamepad* or GHEvdev* code, GHXInput, GHWinUI or GHGameController code, radial menus, hint strips, focus navigation
  on a page, controller settings, Steam Input layouts, or the Steam keyboard. Read before reporting
  such work as finished.
---

The full skill lives in this repository's tool-neutral agent directory (`.agents/`),
which is shared with other AI coding agents. This file is only a pointer.

Read `.agents/skills/client_gamepad_testing/SKILL.md` (path relative to the repository root) in full
before proceeding, and follow it. Any `references/` files it links are relative to that
same directory.
