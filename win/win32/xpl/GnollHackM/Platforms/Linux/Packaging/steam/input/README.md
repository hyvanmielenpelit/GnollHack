# Steam Input Configuration

The Steam target of `Build-LinuxPackages.ps1` copies the `.vdf` files of this folder into
the Steam content root as `steam_input/`.

| File | Content |
|------|---------|
| `gnollhack_input_manifest.vdf` | The Action Manifest. Inert unless Steamworks points at it as `steam_input/gnollhack_input_manifest.vdf` with the Steam Input template "Custom Configuration (Bundled with game)". |
| `controller_neptune.vdf` | GnollHack's Steam Deck layout, which the manifest names. **Not written by hand**: it is exported from the Steam client and committed here with its `creator` line removed. Without it, the packaging script warns and ships the manifest alone. |

The layout is Steam's "Gamepad with Mouse Trackpad" template with the back grips bound to
F2-F5 and gyro off. Players get it as an official configuration in Steamworks, so the copy here
is its record. How to author, export and publish it, and what the manifest route would need
first, is in
[DEVEL/gamepad_controls.md](../../../../../../../../../DEVEL/gamepad_controls.md).
