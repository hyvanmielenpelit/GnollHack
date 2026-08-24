---
name: debugging_gnollhack
description: Instructions on how to use wizard mode (debug mode), decipher panic logs, test specific levels/monsters, and run the MAUI frontend locally.
---

# Debugging GnollHack

## Critical Rules
- **Wizard Mode**: Enabled via the `-D` command-line flag. `wizard` is a macro for `flags.debug` (`include/flag.h`).
- **Panic Logs**: Panics are written to `paniclog` in the local app data folder.
- **Do NOT commit debug flags**: Ensure any hardcoded `wizard = TRUE` changes are reverted before committing.

## Enabling Wizard Mode

**Normal use (MAUI):** toggle the wizard-mode switch on the main page. Do not
patch code for this.

The switch reaches the C engine through a longer chain than you might expect —
worth knowing, because a break can occur at any link:

| # | Where | What happens |
|---|-------|--------------|
| 1 | `MainPage.xaml.cs` | `wizardModeSwitch.IsToggled` sets `gamePage.EnableWizardMode` |
| 2 | `GHGame.cs` | `GHGame.WizardMode` reads `ActiveGamePage?.EnableWizardMode` |
| 3 | `GnollHackService.cs` | Packs `RunGnollHackFlags.WizardMode` (`GHEnums.cs`, `0x02`) into the `runflags` bitmask |
| 4 | `win/win32/xpl/libshare/gnhapi.c` | `RunGnollHack(..., uint64_t runflags, ...)` tests `runflags & GHRUNFLAGS_WIZARD_MODE` (`gnhapi.h`) |
| 5 | `win/win32/xpl/libshare/gnhapi.c` | **Synthesizes the command line `-D -u wizard`** rather than assigning `wizard` directly |
| 6 | `src/options.c` | The engine parses `-D` and sets `wizard` (= `flags.debug`) |

> The enum is `RunGnollHackFlags`, not `RunFlags`. Note step 5: the bridge does
> not set `wizard = TRUE` itself — the direct assignment is commented out in
> `gnhapi.c`. So the `-D` flag is the real mechanism on every platform, and a
> wizard-mode bug can be a command-line-assembly bug.

**Command line (desktop console):** run `gnollhack.exe -D` directly.

**Last resort (code):** setting `wizard = TRUE` in `src/options.c` works but must
never be committed — see the Critical Rules above.

## Wizard Commands
When in wizard mode, special commands become available:
- `^V` (Ctrl+V): Teleport to any level.
- `^W` (Ctrl+W): Wish for any object.
- `^G` (Ctrl+G): Create any monster.
- `^I` (Ctrl+I): Identify all items.
- `^F` (Ctrl+F): Map the entire level.

## Handling Panics
- A "panic" is an intentional crash called via `panic()`.
- Search the codebase for the panic string to find the exact trigger.
- Check the `paniclog` file for the stack trace (if available on the platform).

## Testing the MAUI App
- Build and run the Windows app locally:
  ```powershell
  dotnet build win/win32/xpl/GnollHackM/GnollHackM.csproj -c Debug -t:Run -f net10.0-windows10.0.19041.0
  ```
  > **Note:** The exact target framework moniker may change over time. Check `GnollHackM.csproj` for the current value.
- Use `Debug.WriteLine` in C# for console output.
- For native C debug output, use `raw_printf` (only visible if the console is attached).

## Related Skills

- **`build_pipeline`** — Full build process, two-solution architecture, data pipeline, `makedefs` flags
- **`maui_frontend`** — MAUI frontend architecture, XAML pipeline, SkiaSharp rendering

