# Steam Input Configuration

The Steam target of `Build-LinuxPackages.ps1` copies the `.vdf` files of this folder into
the Steam content root as `steam_input/`.

| File | Content |
|------|---------|
| `gnollhack_input_manifest.vdf` | The Action Manifest. Steamworks points at it as `steam_input/gnollhack_input_manifest.vdf` when the Steam Input template is "Custom Configuration (Bundled with game)". |
| `controller_neptune.vdf` | GnollHack's Steam Deck layout, which the manifest names. **Not written by hand**: it is exported from the Steam client and committed here with its `creator` and `url` lines removed, which identify the author. Without it, the packaging script warns and ships the manifest alone. |

The layout is Steam's "Gamepad with Mouse Trackpad" template with the back grips bound to
F2-F5 and gyro off. How to author, export and publish it, and why the Steamworks setting
waits until both files are in every live Linux depot, is in
[DEVEL/gamepad_controls.md](../../../../../../../../../DEVEL/gamepad_controls.md).
