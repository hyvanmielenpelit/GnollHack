---
name: client_gamepad_testing
description: >-
  How gamepad and game controller work in GnollHack is tested on a real device: the rule that it
  must be, which devices are required (today the Steam Deck, through the Avalonia Linux build) and
  which are a backup (an Xbox controller on the Avalonia Windows build), the packaging and
  Deploy-SteamDeck.ps1 sequence, what to do when the Deck cannot be reached, what the user must
  test, and how to read the Deck's logs afterward. Read whenever you plan, change or
  review anything that alters what a controller does or shows: GHGamepad* or GHEvdev* code,
  radial menus, hint strips, focus navigation on a page, controller settings, Steam Input layouts,
  or the Steam keyboard. Read before reporting such work as finished.
---

# Testing Gamepad Work on a Device

## The Rule

Work that changes what a game controller does or shows is **built for, deployed to and tested on
every shipping build that supports game controllers**. Until those tests have passed, the work is
not verified, however green everything else is. The requirement follows what ships:

| Device and controller | Build | Ships | Role | How it is run |
|-----------------------|-------|-------|------|---------------|
| Steam Deck | Avalonia Linux | Yes | **Required** | Through Steam, in Gaming Mode, from the Non-Steam Game shortcut to `run-gnollhack.sh` |
| Windows PC, Xbox controller | Avalonia Windows | No | Backup and extra check | Directly, from the build output; see [Xbox Backup](#xbox-backup) |

WinUI 3, Android and iOS have no controller support yet. When a shipping build gains it, add a row
here with the role **Required**, and write its procedure in this skill. The Xbox controller on WinUI 3
then becomes a required test, not a backup. The current state of support is in the introduction and
the Eligibility section of `DEVEL/gamepad_controls.md`.

## When This Applies

Anything that changes controller behavior, in any of these:

- `win/win32/xpl/GnollHackX/GnollHackX/Gamepad/` and `Radial/`
- `Pages/Game/GamePage.Gamepad.cs`, `GamePage.GamepadDialogs.cs`, `GamePage.Radial.cs`
- `win/win32/xpl/GnollHackM/Platforms/Avalonia/`: the gamepad and evdev backends
  (`GHAvaloniaGamepad*.cs`, `GHEvdev*.cs`), the Steam keyboard, key handling for the grip bridge,
  and controller input in the in-app browser (`Cef/GHCefInput.cs`, `GHAvaloniaWebScriptHost.cs`)
- `Platforms/Linux/Packaging/steam/input/` and `steamdeck/run-gnollhack.sh`
- Any page with controller navigation, when a change adds, moves or removes a focusable control,
  changes the focus order, or changes a hint strip, an in-page popup or a list's selection
- Settings: the Controller and Trigger Quick Actions sections
- C core or bridge changes made for a controller command

It does not apply to comment-only changes, changes that only touch unit tests, documentation-only
changes, or code that never runs on Linux. A change confined to the XInput backend
(`GHXInputGamepadBackend.cs`) is one of these: it runs only in the Avalonia Windows build, so test it
there with an Xbox controller.

## What Does Not Replace the Device

Run these too, but none of them satisfies the rule:

- **Unit tests.** They cover decoding and routing, not what happens on the device.
- **WSL.** It has no gamepad drivers.
- **The MAUI builds.** They have no gamepad support. Shared code must still compile there, so the
  usual verification builds still apply.
- **The Deck's Desktop Mode.** Game Mode is the real target; `DEVEL/gamepad_controls.md` lists the
  differences.

## Procedure

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
7. **Hand over the tests.** Give the user numbered, concrete checks for this change (press this, in
   this place, expect that), run in **Gaming Mode**, including what should still work next to the
   change. Then report the state as "deployed to the Deck, tests M1-Mn pending".
8. **After the user has tested,** read the log (below) if anything went wrong, and record the results
   in the walkthrough. Only the user's result makes the work verified.

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
package was built, it was **not** deployed, and the Deck tests are outstanding. Give the deploy
command and the test list, and offer the Xbox backup below. Record the same in the plan's `task.md`
and walkthrough. Never describe the work as verified.

## Xbox Backup

The Avalonia Windows build does not ship, so a test here never counts as the required test. Use it in
two situations:

- **As a backup,** when the Deck is unreachable or otherwise unavailable.
- **As an extra check,** when something is unclear. For example, a bug that also appears here is
  probably in the shared logic, and one that appears only on the Deck points at evdev, Steam Input or
  Gaming Mode.

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
Steam keyboard, Gaming Mode, and the Deck's screen size. If the change depends on any of these, say that
the backup cannot speak for it.

**Report it as what it is:** "tested with the Xbox backup on Avalonia Windows, Deck tests M1-Mn
outstanding". The Deck test stays outstanding, and only the user can decide that the backup is enough
for a particular change. Record that decision in the walkthrough if they make it.

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
