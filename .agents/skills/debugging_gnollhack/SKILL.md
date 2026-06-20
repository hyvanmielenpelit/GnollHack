---
name: debugging_gnollhack
description: Instructions on how to use wizard mode (debug mode), decipher panic logs, test specific levels/monsters, and run the MAUI frontend locally.
---

# Debugging GnollHack

## Critical Rules
- **Wizard Mode**: Run the game with `-D` flag or set `wizard = TRUE` in code to enable debug mode.
- **Panic Logs**: Panics are written to `paniclog` in the local app data folder.
- **Do NOT commit debug flags**: Ensure any hardcoded `wizard = TRUE` changes are reverted before committing.

## Enabling Wizard Mode
1. **Command Line**: Run `gnollhack.exe -D` (if using desktop console).
2. **Code**: Temporarily set `flags.debug = TRUE` in `options.c:initoptions()`.
3. **MAUI**: In `GHApp.cs`, ensure `RunFlags.WizardMode` is passed to the native bridge initialization.

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
- Use `dotnet build -t:Run -f net10.0-windows10.0.19041.0` to run the Windows app locally.
- Use `Debug.WriteLine` in C# for console output.
- For native C debug output, use `raw_printf` (only visible if the console is attached).
