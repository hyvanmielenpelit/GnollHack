---
name: client_gamepad_testing
description: >-
  How gamepad and game controller work in GnollHack is tested on real devices: which device a change
  must be tested on, decided by whether it is shared controller code, tested on the Steam Deck as the
  primary device through the Avalonia Linux build, or specific to one platform, tested there; the
  Xbox controller on the Avalonia Windows build as a backup; the packaging and Deploy-SteamDeck.ps1
  sequence, what to do when the Deck cannot be reached, what the user must test, and how to read the
  Deck's logs afterward. Read whenever you plan, change or review anything that alters what a
  controller does or shows: GHGamepad* or GHEvdev* code, GHXInput or GHWinUI code, radial menus, hint strips, focus navigation
  on a page, controller settings, Steam Input layouts, or the Steam keyboard. Read before reporting
  such work as finished.
---

# Testing Gamepad Work on a Device

## The Rule

Work that changes what a game controller does or shows is **tested on a real device** before it counts as verified,
however green everything else is. Which device depends on where the change can behave differently:

| The change is | Test on |
|---------------|---------|
| **Shared controller code** | The **primary device**, the Steam Deck, and nothing else |
| **Specific to one platform** | **That platform's device**, and nothing else |
| **Both**, as when a shared change also edits a platform's branch | The primary device for the whole change, and the platform's device for what is specific to it |

The devices:

| Device and controller | Build | Ships | Role | How it is run |
|-----------------------|-------|-------|------|---------------|
| Steam Deck | Avalonia Linux | Yes | **Primary** | Through Steam, in Gaming Mode, from the Non-Steam Game shortcut to `run-gnollhack.sh` |
| Windows PC, Xbox controller | WinUI 3 | Yes | **Platform** | From Visual Studio, Debug x64; see [Xbox on WinUI 3](#xbox-on-winui-3) |
| Linux PC, Xbox controller | Avalonia Linux | Yes | **Platform** | From the Linux package; see [Xbox on Desktop Linux](#xbox-on-desktop-linux) |
| Windows PC, Xbox controller | Avalonia Windows | No | **Platform** for its own code; otherwise a backup and extra check | Directly, from the build output; see [Xbox on Avalonia Windows](#xbox-on-avalonia-windows) |

Android and iOS have no controller support yet. When a build gains it, add a row with the role **Platform**, and
write its procedure in this skill. Its first implementation gets a full pass there; after that this rule applies.

### Shared or Platform-Specific

**Shared:** code that compiles for several platforms and does not depend on what differs between them. This is most of it:
`GnollHackX/Gamepad/`, `Radial/`, `GamePage.Gamepad.cs`, `GamePage.GamepadDialogs.cs`, `GamePage.Radial.cs`, page
navigation, hint strips, in-page popups, `FileDialogView`, controller settings, and `gamepad_web.js`.

**Platform-specific:** code that runs only there, or a branch for that platform inside shared code:

| Platform | Its code |
|----------|----------|
| Steam Deck | `GHEvdev*`, `GHAvaloniaSteamKeyboard`, `Platforms/Avalonia/Cef/`, `Platforms/Linux/Packaging/steam/input/`, `steamdeck/run-gnollhack.sh`, `#if LINUX` branches, the grip bridge |
| Avalonia Windows | The Windows branches of Avalonia code (`#else` of `#if LINUX`, `#elif GNH_AVALONIA` after `GNH_AVALONIA && LINUX`), and the `NativeWebView` paths (`NativeWebViewHost`, `_nativeView` in `GHAvaloniaWebScriptHost`), which the Deck does not run because it uses CEF |
| WinUI 3 | `Platforms/Windows/GHWinUI*`, `GHWebView2FileChooser`, `KeyboardHook`'s key note, `#if WINDOWS` branches in controller code |
| Desktop Linux | The `!IsSteamDeck` branches of the evdev backend and `GHAvaloniaGamepad`, and physical pads in `GHEvdev*` |
| Both Windows builds | `GnollHackM/Gamepad/GHXInputGamepadBackend.cs`. Test on WinUI 3 |

Other code in `Platforms/Avalonia/`, such as `GHAvaloniaGamepad`, runs on the Deck too, so the Deck covers it.
`Platforms/Avalonia/Web/gamepad_web.js` is shared, whatever its folder.

**Ask one question of a shared change: does it alter how the code uses something a platform implements itself?** Compiling
from the same source does not settle it. Such things are a native control's focus, `ScrollView`, a modal or native dialog,
the frame loop or painting order, and a web view's API rather than the page script. Code that merely runs on them, as all
page navigation does, stays shared: adding a row to a navigated page is shared, while a new way of scrolling to it is
not. Name the platform and say why in the plan. When in doubt, ask the user rather than test everywhere.

A change confined to one platform does not need the primary device. A Deck-only change needs no Xbox test, and an XInput
change needs no Deck.

### Before a Release

Shared code is tested on the primary device only, so before releasing a build with a **Platform** row that ships, the user
runs its smoke list, about 10 minutes, against the release candidate. This applies to WinUI 3, and also to the Avalonia
Windows build if it starts to ship.

1. No controller connected: Settings has no Controller section, and mouse and keyboard play is unchanged.
2. Connect a controller and wait 3 s: A shows the focus on the main menu.
3. Walk Settings to the bottom.
4. Start a game: walk, open a radial, open the inventory and pick an item, answer a y/n prompt.
5. Alt+Tab away, press buttons, return: nothing happened, and nothing is stuck.
6. Move the mouse: focus and hints go. Press the pad: they return.
7. Open the wiki: the ring moves, and A follows a link.
8. Open the Overseer: Menu leaves.

A failure there is a bug to fix before release, whichever code it is in.

## When This Applies

Anything that changes controller behavior, in any of these:

- `win/win32/xpl/GnollHackX/GnollHackX/Gamepad/` and `Radial/`
- `Pages/Game/GamePage.Gamepad.cs`, `GamePage.GamepadDialogs.cs`, `GamePage.Radial.cs`
- `win/win32/xpl/GnollHackM/Platforms/Avalonia/`: the gamepad and evdev backends
  (`GHAvaloniaGamepad*.cs`, `GHEvdev*.cs`), the Steam keyboard, key handling for the grip bridge,
  and controller input in the in-app browser (`Cef/GHCefInput.cs`, `GHAvaloniaWebScriptHost.cs`)
- `win/win32/xpl/GnollHackM/Gamepad/` and the WinUI 3 glue: `Platforms/Windows/GHWinUI*.cs`,
  `GHWebView2FileChooser.cs`, and `KeyboardHook`'s key note in `MauiProgram.cs`
- `Platforms/Linux/Packaging/steam/input/` and `steamdeck/run-gnollhack.sh`
- Any page with controller navigation, when a change adds, moves or removes a focusable control,
  changes the focus order, or changes a hint strip, an in-page popup or a list's selection
- Settings: the Controller and Quick Radials sections
- C core or bridge changes made for a controller command

It does not apply to comment-only changes, changes that only touch unit tests, or documentation-only changes.

## What Does Not Replace the Device

Run these too, but none of them satisfies the rule:

- **Unit tests.** They cover decoding and routing, not what happens on the device.
- **WSL.** It has no gamepad drivers.
- **A build without controller support.** Android and iOS only prove that shared code compiles, so the usual verification
  builds still apply.
- **The Deck's Desktop Mode.** Game Mode is the real target; `DEVEL/gamepad_controls.md` lists the differences.
- **Another platform's device, for that platform's code.** Each row tests its own.

## Procedure

Decide first, using the rule, which devices the change needs, and say so in the plan or the handoff. The steps below are the
Deck's; a platform with its own row has its own section.

Only the orchestrating agent does this, after every subagent has returned.

1. **Finish the edits first.** The deploy's `VERSION` file records the repository state at deploy
   time, not the state of the staged build, so an edit made after packaging is invisible in it.
2. **Run the unit tests**, from `win\win32\xpl\GnollHackTests`: `dotnet test GnollHackTests.sln`.
3. **Rebuild what packaging does not rebuild:**

   | You changed | Before packaging |
   |-------------|------------------|
   | C# only | Nothing |
   | GnollHackX page or control XAML | Build `makedefsdroid` (see `build_pipeline`) |
   | C core, `libshare`, `dat/` | The `Linux` solution platform in **Release** (see "Native prerequisites of the Linux flavor" in `build_pipeline`) |

4. **Package**, from `win\win32\xpl\GnollHackM\Platforms\Linux\Packaging`. This takes a few minutes,
   so run it in the background:

   ```powershell
   .\Build-LinuxPackages.ps1 -Targets TarGz -NoSentryUpload
   ```

   `-NoSentryUpload` is required: without it a Release publish on a machine with `SENTRY_AUTH_TOKEN`
   uploads test symbols to production. Never pass `-SkipPublish` after a code change; it would
   re-stage the previous publish. This step runs whether or not the Deck is reachable, because it
   also proves that the Linux flavor compiles, packages and meets the glibc floor.
5. **Deploy:**

   ```powershell
   .\Deploy-SteamDeck.ps1
   ```

   The script does its own connection check and stops with a clear message. See below if it fails.
6. **Confirm** that the output names the version you just packaged.
7. **Hand over the tests.** They are a `TEST` session in the task's Developer Runbook
   (`client_developer_runbook`): numbered, concrete checks for this change (press this, in this
   place, expect that), run in **Gaming Mode**, and as few as that skill's limit allows.
   Write the walkthrough with its `## Runbook status` table first, so that the tests can be listed.
   Then report the state as "deployed to the Deck, tests M1-Mn pending".
8. **After the user has tested,** read the log (below) if anything went wrong, and record the results
   in the walkthrough's `## Runbook status` table. Only the user's result makes the work verified.

## When the Deploy Fails

| Message from the script | Meaning | What to do |
|-------------------------|---------|------------|
| `Cannot resolve` or `Cannot reach` | Asleep, off, or on another network | Ask the user once to wake the Deck, retry once, then treat it as unreachable. Never retry in a loop |
| `no SSH server is running` | sshd is off | The user enables it on the Deck; the script prints the command |
| `The one-time SSH setup ... is missing` | No trusted key | The user runs the printed `ssh-copy-id`. It needs the deck password, which an agent never handles |
| `is not the one trusted as` | A different device, or a reinstall | Stop and tell the user. Never edit `known_hosts` |
| `GnollHackM is running on the Deck` | The game is open, maybe with a game in progress | Ask the user to quit it. Pass `-Force` only if the user says to |
| `No staged build` | Packaging did not run for this version | Run step 4 |

**An unreachable Deck does not end the task, and it does not waive the rule.** Report plainly: the
package was built, it was **not** deployed, and the Deck tests are outstanding. The runbook
already has the deploy command and the test list: point to it, and offer the Xbox backup below. Record the same in the plan's `task.md`, and leave the `DEPLOY` step and its checks `blocked`
in the status table. Never describe the work as verified.

## Xbox on Desktop Linux

Build the package from `win\win32\xpl\GnollHackM\Platforms\Linux\Packaging`, then the user runs the tar.gz from
`GnollHackM\bin\LinuxPackages\<version>\` on the PC with the controller connected. There is no deploy step. The `GAMEPAD` lines
in its log name each pad and verdict.

```powershell
.\Build-LinuxPackages.ps1 -Targets TarGz -NoSentryUpload
```

## Xbox on WinUI 3

Build from `win\win32\xpl\GnollHackM`, then the user runs it from Visual Studio, Debug x64, with the controller connected.
There is no deploy step.

```powershell
dotnet build GnollHackM.csproj -c Debug -f net10.0-windows10.0.19041.0
```

The numbered checks for the change are a `TEST` session in the task's Developer Runbook, as for the
Deck. Read `GAMEPAD` lines in the app log if anything goes wrong.

## Xbox on Avalonia Windows

The Avalonia Windows build does not ship. It has two uses:

- **For its own code:** a change confined to its row in the platform table is tested here and needs no Deck.
- **As a backup or an extra check for shared code**, when the Deck is unreachable or something is unclear. For example, a
  bug that also appears here is probably in the shared logic, and one that appears only on the Deck points at evdev, Steam
  Input or Gaming Mode.

Build from `win\win32\xpl\GnollHackM\Platforms\Avalonia`, then have the user run the result with the
controller connected. `gnollhackwin.dll` must be current if the C core or bridge changed; see "Windows
flavor" in `build_pipeline`.

```powershell
dotnet build ..\..\GnollHackM.csproj -p:GnhAvalonia=true
```

The executable's path is in "Windows flavor" in `build_pipeline`. `GNOLLHACK_GAMEPAD_KEYS=on` makes
F2-F5 on the keyboard act as the back grips.

**What it covers:** focus navigation, radial menus, hint strips, routing, and settings.
**What it cannot cover:** evdev and device filtering, Steam Input and its layouts, the real grips, the
Steam keyboard, Gaming Mode, the Deck's screen size, WinUI's own glue and WebView2 file chooser, and attaching
with a controller, where A on `+` opens Windows' own dialog. If the change depends on any of these, say that
the backup cannot speak for it.

**Report it as what it is:** "tested with the Xbox backup on Avalonia Windows, Deck tests M1-Mn
outstanding". The Deck test stays outstanding, and only the user can decide that the backup is enough
for a particular change. If they make it, the checks become `waived` in the status table, and the note
says so.

## First-Time Setup

These are one-time steps for a person, documented in `Deploy-SteamDeck.ps1` and
`DEVEL/gamepad_controls.md`: the SSH key, the Non-Steam Game shortcut, and the shortcut's Steam
Input layout. Without a gamepad layout there is no virtual pad and the controller does nothing, which
looks like a code bug. The `GAMEPAD hint:` lines in the log say so.

## Reading the Deck

Resolve the address on Windows, because WSL cannot resolve `.local` names, and use the trusted alias.
Call `wsl.exe` from PowerShell, not Git Bash:

```powershell
$ip = @([System.Net.Dns]::GetHostAddresses('steamdeck') | Where-Object { $_.AddressFamily -eq 'InterNetwork' })[0].ToString()
wsl.exe -e ssh -o BatchMode=yes -o HostKeyAlias=steamdeck "deck@$ip" "tail -n 100 ~/gnollhack-dev/logs/latest.log"
```

PowerShell 5.1 strips double quotes from native arguments, so for anything with quotes or `$(...)`,
write a script to the scratch directory and pipe it to `ssh ... bash -s` through `tr -d '\r'`.

- `~/gnollhack-dev/VERSION`: what is deployed
- `~/gnollhack-dev/logs/latest.log`: the newest run, starting with its environment. `XDG_CURRENT_DESKTOP=KDE`
  means the run was in Desktop Mode, so it does not count as the Game Mode test
- `GAMEPAD` lines: backend eligibility, opened devices and hints; their meanings are in the Diagnostics
  section of `DEVEL/gamepad_controls.md`
- `~/gnollhack-dev/dev.env`: optional environment for the next run, such as `GNOLLHACK_GAMEPAD_LOG=raw`
  or `GNOLLHACK_KEY_LOG=1`. It is the user's file: ask before changing it, and say what you changed

Game data in `~/.local/share/GnollHack` is the user's. Read it only when the task needs it, and do
not change it.

## Related Skills

- **`build_pipeline`** -- Linux native prerequisites, packaging, and what `Deploy-SteamDeck.ps1` does
- **`client_implementation_planning`** -- How a plan records this
- **`client_workarounds`** -- A controller workaround for a Steam or Avalonia defect is registered too
- **`client_developer_runbook`** -- Where the checks and their results are recorded
