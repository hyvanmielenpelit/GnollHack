# Steam Workarounds

- Component: Steam client, Steam Input
- Upstream: https://github.com/ValveSoftware/steam-for-linux
- ID prefix: WA-STEAM

Entries follow the schema in [README.md](README.md). Append workarounds before the
`<!-- known-issues -->` line if there is one, otherwise at the end of the file, and known
issues at the end of the file. Never renumber or reuse an ID.

<!-- entries -->

## WA-STEAM-0001: Steam Input's virtual gamepad has no back grips, so the grips reach the game as F2-F5

- Status: active
- Added: 2026-10-28
- GnollHack version: 3.32 (432)
- Component: Steam client, Steam Input
- Package: Steam client
- Defective version: Steam client 1792618285 (SteamOS 3.9.5 stable)
- Platforms: Linux
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-28
- Code: `win/win32/xpl/GnollHackX/GnollHackX/Gamepad/GHGamepadKeyMap.cs` (`GHGamepadKeyMap.Primary`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/GHAvaloniaGamepad.cs` (`GHAvaloniaGamepad.TryHandleKey`)
- Remove when: Steam Input can send the back grips to the game as gamepad buttons (a virtual pad with paddle outputs, or a Steam Input API backend in GnollHack), and Verify removal passes.

**Symptom.** A game that reads Steam Input's virtual gamepad cannot see the Steam Deck's
back grips L4, L5, R4 and R5. Bound to anything other than a key, they never reach the game.

**Cause.** The virtual pad ("Microsoft X-Box 360 pad", `28DE:11FF`) is an Xbox 360 device
with 11 buttons, and Steam Input offers no paddle output for it. The keys F13-F24, which no
real keyboard has, cannot be bound in Steam Input, so a layout can only send keys that also
exist on keyboards. A missing feature, not a defect.

**Workaround.** GnollHack's Steam Input layout binds the grips to F2, F3, F4 and F5, which
GnollHack otherwise ignores. `GHGamepadKeyMap.Primary` maps the four keys back to the grips,
and `GHAvaloniaGamepad.TryHandleKey` passes them to the back-grip key bridge, an auxiliary
controller that delivers them as grip buttons of the active pad and marks the keys handled.
See `DEVEL/gamepad_controls.md`.

**Verify removal.** If Steam Input gains paddle outputs on the virtual pad, or GnollHack
gets a Steam Input API backend:
- Rebind the grips in the GnollHack layout to the paddles instead of F2-F5.
- On the Deck, run with `GNOLLHACK_GAMEPAD_LOG=raw` and find the paddle codes in the raw
  event dump.
- Map them in `GHEvdevLayout`, and delete the four entries of `GHGamepadKeyMap.Primary`
  together with the key bridge.
- Build and deploy the Linux flavor. Each grip must act once per press, coming from the evdev
  backend only, with no F2-F5 key events in the `GNOLLHACK_KEY_LOG=1` key log.

Checks:
- 2026-10-28 | Steam client 1792618285, SteamOS 3.9.5 | still-needed | Grips reach the game only as keys (Step 1b G3); no upstream issue found.
