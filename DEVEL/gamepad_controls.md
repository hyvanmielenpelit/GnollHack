# Gamepad Controls

GnollHack's native gamepad controls are built for the **Steam Deck**: the Avalonia Linux
build, launched through Steam. They also work with an **Xbox controller** in the Avalonia
Windows build, for development. The MAUI builds (Android, iOS, WinUI) have no gamepad
support yet.

Contents:

- [Control scheme](#control-scheme)
- [Steam Input](#steam-input)
- [The GnollHack Steam Input layout](#the-gnollhack-steam-input-layout)
- [Eligibility and environment variables](#eligibility-and-environment-variables)
- [Diagnostics](#diagnostics)
- [Development](#development)

## Control Scheme

Button names follow the Steam Deck. On an Xbox controller L1/R1 are LB/RB, L2/R2 are LT/RT,
View is Back and Menu is Start. The on-screen hints draw the glyphs of the controller in use
(Steam Deck, Xbox, PlayStation or Nintendo), which Steam's `SteamVirtualGamepadInfo` file names.

A long press is 0.5 s. The D-pad, L1/R1 and stick directions repeat while held (0.4 s, then
every 0.09 s) where they navigate.

**Pages outside the game.** The main menu and the About, Reset, Import/Export, Vault and Version
pages work like the game menu: the D-pad or left stick moves a focus ring over the buttons, A
presses the focused button and B goes back (on the main menu B does nothing; its in-page popups
take A and B as well). The main menu starts on Play Game, and the Reset page starts on Close.
On the main menu the focus also reaches the Classic, Casual and Wizard Mode switches, between
Exit and Play Game, and a popup's "do not show again" check box: their row is tinted while
focused, and A flips them as a tap does. Right on Play Game goes to Exit. The main menu's
shortcuts are X Play Game, Y About, Menu Settings and View Vault, each when its button is
available. On every page, L2/R2 go to the first and last entry of the scrolling list (or of the
page when it has none).

On every page with a scrolling list, the right stick scrolls it, faster the further it is pushed.
The other pages work the same way, with these additions:

- **Settings:** the focus walks the rows in order, and the focused row is tinted. A flips a
  switch, steps a picker to its next value (wrapping), or edits a text field. Left/Right step a
  picker or a volume slider (5 %, held to repeat), or move between two buttons in one row. L1/R1
  jump to the previous/next section. X shows the row's explanation, as tapping its label does.
  B saves and closes, as Escape does; if a field is invalid, the focus goes to its row.
- **Library, Oracle, Music, Snapshots, Achievements, Save Transfer:** the focus moves over the
  rows and the buttons below them; A opens a row.
- **Top Scores and Replays:** Up/Down move the list's selection, and A opens the dumplog or plays
  the replay; past the list's ends the focus moves on to the buttons. Replays in multiple
  selection mode leave the list to touch.
- **Text viewer:** the sticks and D-pad Up/Down scroll the text, L1/R1 page.
- **Wiki:** Close and the back/forward arrows; the page itself stays touch.
- **Name page:** the focus starts on the name field.

**Text fields and the Steam keyboard.** On a Steam Deck in Game Mode, A on a text field focuses
it and opens Steam's on-screen keyboard, placed away from the field; the keyboard's Enter closes
it and confirms, and B ends the edit. A getline prompt (wish, engrave, name) opens the keyboard
as soon as it appears, and Y reopens it. In Desktop Mode, or with `GNOLLHACK_STEAM_KEYBOARD=off`,
A only focuses the field; Steam + X opens the keyboard by hand. `GNOLLHACK_STEAM_KEYBOARD=on`
allows it on any Linux session, and `GNOLLHACK_STEAM_KEYBOARD_LAUNCH` (`xdg-open`, the default,
`steam` or `launcher`) picks how the `steam://open/keyboard` link is opened.

Pages that keep the keyboard adapter (the Overseer, and the developer-only editor and performance
suite): A is Enter, B is Escape (closes the page), directions are arrow keys, L1/R1 are Page Up
and Page Down.

### On the Map

Both shoulder buttons are **hold modifiers** on the map, each for the controls the other hand
uses: **L1** changes the right-side controls (A, B, X, Y, R2, Menu) and **R1** the left-side
controls (D-pad, left stick, View). Neither does anything on its own. A layer is sampled when
the other button is pressed. The hint strip shows the layer while a modifier is held, with the
held modifier's badge and its outlined up arrow (see Hint badges) in gold.

| Control | Alone | L1 held | R1 held | L1 + R1 held |
|---------|-------|---------|---------|--------------|
| A | **Context command**: nothing when there is none, runs it when there is exactly one, opens the Actions radial when there are several | Open the **commands radial** | -- | -- |
| B | Cancel: close an overlay, else Escape; interrupts running and other occupations | Wait | -- | Rest |
| X | Pick up | Pick and Stash | -- | -- |
| Y | **Repeat** | Search | -- | Search 20 times |
| R2 | R2's **assigned action** (default Fire) | **Right Quick** radial (R2's quick actions) | **Set R2** radial | R2's assigned action |
| L2 | L2's **assigned action** (default Cast) | **Set L2** radial | **Left Quick** radial (L2's quick actions) | L2's assigned action |
| Menu | Open the game menu (Menu again closes it) | Abilities | -- | -- |
| View | Inventory | -- | Equipment (all equipment in use) | -- |
| D-pad | Walk one step per press, repeating while held; two directions together walk diagonally | -- | **Zoom**: Up/Down zoom in and out, Left toggles the minimap, Right returns to normal zoom | -- |
| Left stick | Walk (8-way) | -- | Run in that direction | -- |
| Right stick | Pan the map | | | |
| L3 | Center the map on the hero | | | |
| R3 | Open the Toggles radial | | | |
| L4, L5, R4, R5 (back grips) | Configurable; defaults Apply, Cast, Quick Cast, Quick Zap | | | |
The grips are set in **Settings, Controller**. Their pickers appear only when the controller
has grips, and they work only with the GnollHack Steam Input layout (see below); with any other
layout they do nothing, which the game cannot detect.

**Trigger quick actions.** L2 and R2 each have eight quick actions, set in **Settings, Trigger
Quick Actions** (any command the bar buttons can hold, or None):

| Trigger | Defaults, slots 1-8 | Assigned by default |
|---|---|---|
| R2 | Fire, Swap, Kick, Cast, Throw, Search, Search 20, Wait | Fire |
| L2 | Apply, Cast, Quick Cast, Zap, Quick Zap, Eat, Pick-axe, Polearm | Cast |

The modifier on the trigger's own side (R1 + R2, L1 + L2) opens the teal **Set** radial: choosing
a slot makes it the trigger's action when pressed alone (the current one is marked NOW). The
modifier on the other side (L1 + R2, R1 + L2) opens the gold **Right Quick** or **Left Quick**
radial, which runs the chosen action at once. Pressing the trigger again closes either radial.

**Map overlays.** While More Messages or the Status Screen (the extended status bar) is open
(both from the Toggles radial), the right stick and D-pad Up/Down scroll it, B closes it, and the left stick
still walks.

**Choosing a position** (getpos): the left stick moves the cursor, with 8-square jumps at full
push; the right stick pans. A selects, B cancels, L1/R1 go to the previous/next interesting
spot, L2/R2 to the previous/next monster, and Y puts the cursor on the hero.

**Choosing a direction** (getdir): a D-pad press picks the direction at once (two directions
pressed together pick the diagonal); or aim with the left stick and confirm with A. B cancels.
L1 is up (`<`) and R1 is down (`>`).

**Hint badges.** The hints on the map, in menus and dialogs, and in the radials draw button
badges: A, B, X and Y as white circles with a black letter (drawn symbols on PlayStation pads),
L1/R1 as shoulder bumpers labeled with the pad's name for them (L1, LB or L), L2/R2 as triggers,
L3/R3 as stick caps, the back grips as paddles, Menu as three lines, View as two overlapping
squares, and the D-pad and sticks as a plus and a ring. An outlined up arrow after a badge means
**hold** the button. A long hint strip wraps onto two centered lines; with no modifier held,
the map strip starts its second line at L1.

**Command bar.** While the controller is the active input (outside replays), the bottom command
bar's buttons and its dimmed background are hidden but keep their space, so the map, the messages
and the context buttons stay where they are, and the hint strip (with its own backing) is
centered vertically in the bar's area. Menus likewise hide their OK and Cancel buttons, keeping
the space; the hints show X OK and B Cancel. Touch, the mouse or a keyboard key brings the
buttons back; the first touch on the bar only does that.

**Replays:** A pauses, L1/R1 change the speed, B quits.

### Radials

The radials stay open until an item is chosen or they are closed.

- **Select** with whichever stick is pushed further, or step with D-pad Left/Right; D-pad
  Up/Down jump to the top and bottom slots. The center hub describes the highlighted item.
- **A** activates it, **B** closes the radial (R3 also closes the Toggles radial), **L1/R1** change page.
  **Menu** closes the radial and opens the game menu.
- **Toggles radial (R3):** More Messages, Status Screen, Auto-Center, Look Mode, Auto-Dig,
  Ignore Stopping, Max Hit Points and Max Mana (the maximum in the health and mana orbs, as
  tapping an orb does; unavailable while the orbs are hidden). The two orb toggles show the orbs
  themselves, with the current and maximum values. Toggling an option leaves the radial open.
- **Actions radial** (A with several context commands, or L1 + A for the commands pages):
  the context buttons plus Skills, Polearm and Wield Last/Unwield (eight slots per page, overflowing onto further pages), then the **commands pages**:
  Rest, Search 20 and Extended first, then every other command of the single commands page in
  alphabetical order, eight per page with no gaps. The hub shows the page number above the page
  name.
  **X** switches between the context page and the first commands page; L1/R1 page.

After a radial closes, both sticks stay disarmed until they return to center.

### Menus, Dialogs and Text Windows

Menus show a gold focus selector, and the y/n buttons, message boxes, text windows and the game
menu follow the same rules:

| Control | Action |
|---------|--------|
| Left stick, D-pad | Move the focus; in menus D-pad Left/Right page |
| Right stick | Scroll |
| A | Confirm, OK, or select the focused item; hold A on a menu row for its count or actions |
| B | Cancel, close, back |
| X | Menus and the count dialog: OK (accept the selection). getline: the list |
| Y | Menus: All / None (select or unselect every item, in menus that select several). Count dialog: All. getline: reopen the Steam keyboard |
| L1 / R1 | Menus: jump between sections |
| L2 / R2 | Menus: first or last row |

A y/n prompt starts with the focus on its default answer, or on the leftmost button when it
has none; Y answers Yes and X answers No when the prompt offers them. In an inventory or
equipment menu, Menu flips between the item list and the equipment view, like the flip button;
on the equipment view B closes the menu. Touch, moving the mouse or pressing a keyboard key leaves controller mode until the
controller is used again; tapping a menu row moves the focus there.

## Steam Input

GnollHack reads **Steam Input's virtual gamepad**, not the Deck's own controller. Steam turns
the Deck's controls into a virtual Xbox 360 pad ("Microsoft X-Box 360 pad N", USB ID
`28DE:11FF`) according to the game's Steam Input layout, and GnollHack reads that pad through
evdev (`/dev/input/event*`). Two consequences:

- **The layout must output a gamepad.** Steam's default for a game without declared controller
  support sends keyboard and mouse only. With that default **no virtual pad exists, and the
  controller does nothing in GnollHack** (touch still works). If the controls do nothing, open
  the Steam button menu in game, Controller Settings, and select the template **"Gamepad with
  Mouse Trackpad"**, or the GnollHack layout once it is published. Making the GnollHack layout
  the default in Steamworks is therefore a release prerequisite.
- **The back grips need key bindings.** The virtual pad has no paddle buttons, so the GnollHack
  layout sends the grips as **F2, F3, F4 and F5** (L4, L5, R4, R5), which the game turns back
  into grips (`WA-STEAM-0001` in `DEVEL/workarounds/steam.md`). With Steam's plain Gamepad
  template everything except the grips works.

If Steam Input is turned off for the game, only the Deck's physical controller is visible
(`28DE:1205`, the hid-steam trackpad-mouse and keyboard devices). GnollHack does not read it,
and the device log says so (see [Diagnostics](#diagnostics)). Turn Steam Input on in the game's
Properties, Controller, in Steam.

**Steam recreates the virtual pad.** In Game Mode, Steam may recreate the virtual gamepad when
any app is launched ([steam-for-linux#13665](https://github.com/ValveSoftware/steam-for-linux/issues/13665)).
GnollHack watches `/dev/input` with inotify and opens the new pad automatically, so nothing
special is needed; the device log shows the old pad closed and the new one opened.

**Focus.** evdev delivers events whatever window has focus, so GnollHack ignores the pad while
its window is inactive. In Game Mode and Desktop Mode, Steam silences the pad while the Quick
Access Menu or the Steam overlay is open. In Desktop Mode the window raises no Deactivated event
for them; the silent pad is what keeps the game from reacting.

## The GnollHack Steam Input Layout

### Content

- Steam's **"Gamepad with Mouse Trackpad"** template, unchanged: sticks as joysticks, analog
  triggers, D-pad, every button to the virtual pad, right trackpad as mouse.
- **L4 → F2, L5 → F3, R4 → F4, R5 → F5**: Regular Press, no extra activators, no turbo.
- Gyro off.
- Title "GnollHack (Steam Deck)".

The analog sticks reach the game raw; GnollHack applies its own deadzone.

### Authoring and Exporting

Official configurations need the real app ID, so the layout is authored as a **personal layout
on the Non-Steam development shortcut** (see [Development](#development)):

1. Launch the shortcut, open Controller Settings, and pick the "Gamepad with Mouse Trackpad"
   template.
2. Bind the four grips as above, turn gyro off, and save the layout with the title above.
3. In Desktop Mode, export it with
   `xdg-open "steam://dumpcontrollerconfig?appid=<id>"`, which writes the `.vdf` to `/tmp`.
   For the shortcut, `<id>` is its `SteamGameId`, which `run-gnollhack.sh` writes at the top of
   each log.
4. Copy the file over SSH to
   `win/win32/xpl/GnollHackM/Platforms/Linux/Packaging/steam/input/controller_neptune.vdf` and
   commit it. It is both the record of the layout and the file the depot ships.

A later developer can apply the exported layout by placing it in the shortcut's personal
configuration folder, or rebind it by hand from the list above.

### Publishing

**Primary route: the bundled Action Manifest.**

- `Packaging/steam/input/gnollhack_input_manifest.vdf` is the manifest; it names
  `controller_neptune.vdf` as the Steam Deck configuration.
- The Steam target of `Build-LinuxPackages.ps1` copies the `.vdf` files of that folder into the
  content root as `steam_input/`, and warns when `controller_neptune.vdf` is missing.
- In Steamworks: Application, Steam Input, "Steam Input Template" = **"Custom Configuration
  (Bundled with game)"**, path `steam_input/gnollhack_input_manifest.vdf`. Steam reads the
  manifest from the depots, so the layout ships together with the game code that reads it.
- **Select it only after every live branch's depot contains both files.** A branch without
  them would get a missing configuration.

**Fallback route: an official configuration**, if Steam rejects a manifest for a game that does
not use the Steam Input API:

1. Export the layout with a developer account ("Steam Input Layout Dev Mode", gear, Export
   layout).
2. Make it the official configuration.
3. Select it for the Steam Deck controller type in Steamworks.

A player who picks another template or their own layout keeps it; Steam remembers the choice
per user. A keyboard template produces no virtual pad, so keys behave as they always did.

## Eligibility and Environment Variables

| Backend | Platform | Used when |
|---------|----------|-----------|
| evdev | Avalonia Linux | `GNOLLHACK_GAMEPAD_EVDEV` allows it (below) |
| Back-grip key bridge | Avalonia, any OS | `GNOLLHACK_GAMEPAD_KEYS` allows it (below) |
| XInput | Avalonia Windows | Always; Xbox controllers through `xinput1_4.dll` |

A **Steam launch** is any of `SteamGameId`, `SteamAppId` (non-empty and not `0`),
`SteamClientLaunch=1`, or `SDL_GAMECONTROLLER_ALLOW_STEAM_VIRTUAL_GAMEPAD`. Non-Steam shortcuts
count too: Steam gives them a non-zero `SteamAppId`.

| Variable | Values | Effect |
|----------|--------|--------|
| `GNOLLHACK_GAMEPAD_EVDEV` | `auto` (default) | evdev runs only on a Steam Deck launched by Steam, and reads only Steam's virtual pads. A terminal launch never reads the pad |
| | `off` | evdev does not run |
| | `any` | Drops the Steam Deck and Steam-launch requirements, and also accepts physical Xbox-layout pads (never Valve's physical devices); for desktop Linux testing |
| `GNOLLHACK_GAMEPAD_KEYS` | `auto` (default) | The key bridge runs on a Steam Deck launched by Steam |
| | `on` | Always: F2-F5 act as the grips, so a PC developer can press them on a keyboard |
| | `off` | Never; F2-F5 are ordinary keys |
| `GNOLLHACK_GAMEPAD_LOG` | `raw` | Writes every evdev event to standard error, capped at 5000 lines per run; stick jitter inside the deadzone is skipped |
| `GNOLLHACK_KEY_LOG` | `1` | Logs every KeyDown, KeyUp, TextInput and window activation, one line each, to standard error and the app log |

The evdev backend honors `SDL_GAMECONTROLLER_IGNORE_DEVICES` and
`SDL_GAMECONTROLLER_IGNORE_DEVICES_EXCEPT`, which Steam sets to hide the controllers it wraps.
It never grabs a device.

The bridge only takes F2-F5 without modifiers. **Settings, Controller, Use Controller** turns
the whole feature off: evdev and XInput input is discarded, and F2-F5 pass through to the game
as keys. The Controller section is shown only when a backend is available.

## Diagnostics

The **device log** is always on. It goes to standard error and the app log at startup and on
every hot-plug, with lines starting `GAMEPAD`:

| Line | Meaning |
|------|---------|
| `GAMEPAD env mode=... steamLaunch=... deck=... eligible=...` | What the evdev backend saw in the environment, and whether it runs |
| `GAMEPAD dev eventN +open ... verdict=accept:steam-virtual ... style=...` | A pad was opened, with its axes, deadzone and glyph style |
| `GAMEPAD dev eventN ... verdict=skip` | A device was skipped, with the reason; most are filtered through sysfs without being opened |
| `GAMEPAD keys bridge on (...)` / `off (...)` | Whether the back-grip bridge runs, and its keys |
| `GAMEPAD hint: no Steam virtual gamepad (GnollHack's Steam Input layout is not a gamepad layout?)` | A Steam launch with no virtual pad: the layout outputs keyboard and mouse. Select "Gamepad with Mouse Trackpad" or the GnollHack layout |
| `GAMEPAD hint: only valve-physical device present (Steam Input off?)` | Only the Deck's physical controller is visible. Turn Steam Input on for the game |

The hints are written once, 10 s after the backend starts. A frame that throws is
logged at most once every 10 s, and 30 failed frames in a row stop the gamepad for the session.

On the Deck's development shortcut, standard error goes to
`~/gnollhack-dev/logs/latest.log`.

## Development

### Steam Deck

`Deploy-SteamDeck.ps1` (in `win/win32/xpl/GnollHackM/Platforms/Linux/Packaging/`) copies a
staged Linux build to `~/gnollhack-dev/` on the Deck and installs `run-gnollhack.sh`, the target
of a **Non-Steam Game shortcut** that is added once in Desktop Mode. The launcher logs each run
to `~/gnollhack-dev/logs/` with the Steam environment variables at the top, and sources
`~/gnollhack-dev/dev.env` first when it exists, exporting what it sets. For example:

```sh
GNOLLHACK_KEY_LOG=1
GNOLLHACK_GAMEPAD_LOG=raw
```

The shortcut is a Steam launch on a Steam Deck, so `auto` applies; it needs the personal layout
described above.

**Game Mode and Desktop Mode differ:**

- Game Mode (gamescope) is the real target. Test controls there.
- In Desktop Mode, Steam's desktop layout does not double the input: face buttons and sticks
  produce no key events while the game has focus. The Quick Access Menu and the overlay raise
  no Deactivated event; the pad goes silent instead.

The build itself is described in the `build_pipeline` skill (`.agents/skills/build_pipeline/SKILL.md`).

### Windows

The Avalonia Windows flavor reads Xbox controllers through XInput, so the analog paths that ship
on the Deck can be exercised on a PC. Connected controllers are polled every frame, empty slots
every 2 s. A PlayStation controller works through Steam's XInput wrapping only.

For the grips, set `GNOLLHACK_GAMEPAD_KEYS=on` and press F2-F5 on the keyboard.

### Linux Desktop

WSL has no gamepad drivers and is not a test platform. On a Linux desktop with an Xbox pad, use
`GNOLLHACK_GAMEPAD_EVDEV=any`, or launch through desktop Steam as a Non-Steam game to read its
virtual pad. The evdev decoding is covered by unit tests whose fixtures are the Deck's real
capability bitmaps and raw dumps.
