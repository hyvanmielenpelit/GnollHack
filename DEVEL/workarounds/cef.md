# CEF Workarounds

- Component: CEF, CefGlue, Chromium
- Upstream: https://github.com/chromiumembedded/cef; CefGlue from the CefGlue.Next.Core NuGet package
- ID prefix: WA-CEF

Entries follow the schema in [README.md](README.md). Append workarounds before the
`<!-- known-issues -->` line if there is one, otherwise at the end of the file, and known
issues at the end of the file. Never renumber or reuse an ID.

<!-- entries -->

## WA-CEF-0001: cef.runtime copy target nests Resources and skips publish

- Status: active
- Added: 2026-10-20 (from git history)
- GnollHack version: 3.28 (428)
- Component: CefGlue
- Package: cef.runtime.linux-x64
- Defective version: cef.runtime.linux-x64 154.0.34
- Platforms: Linux
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/GnollHackM.csproj` (`_GhCefRoot`)
- Remove when: A cef.runtime.linux-x64 release copies its files flat next to the executable on both build and publish, and Verify removal passes.

**Symptom.** With the package's own copy target, CEF cannot load its ICU data on Linux,
and a published build has no CEF files at all.

**Cause.** The package's `CefRedistLinuxX64CopyResources` target keeps the `Resources`
subfolder, while CEF on Linux loads `icudtl.dat` from libcef.so's directory, and it runs
only after `Build`, never on publish. The package is maintained in the Deon-Berlin/CefGlue
repository, which has no issue tracker.

**Workaround.** The package is referenced with `ExcludeAssets="all"`, and its files are
copied flat next to the executable as `None` items on build and publish, with only the
en-US locale and without `chrome-sandbox`.

**Verify removal.** Remove `ExcludeAssets="all"` and the `_GhCefRoot` and `_GhCefLocale`
items. Publish the Avalonia flavor for linux-x64, run it, and open a web view; CEF must
initialize (the log shows "CEF ... initialized") and pages must render.

Checks:
- 2026-10-23 | cef.runtime.linux-x64 154.0.34 | unverified | Upstream has no issue tracker; the props file in the package still nests Resources and copies after Build only.

## WA-CEF-0002: CefRuntime installs a disk-scanning native library resolver

- Status: active
- Added: 2026-10-20 (from git history)
- GnollHack version: 3.28 (428)
- Component: CefGlue
- Package: CefGlue.Next.Core
- Defective version: CefGlue.Next.Core 154.8037.98
- Platforms: Linux
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/Cef/GHCefRuntime.cs` (`GHCefRuntime`)
- Remove when: CefRuntime's resolver no longer scans the disk on each miss, in a CefGlue.Next.Core release GnollHack uses, and Verify removal passes.

**Symptom.** Once CefRuntime is touched, every unresolved native library lookup in the
process costs a scan of the disk.

**Cause.** CefRuntime's static constructor installs a process-wide native library
resolver that scans the application directory and up to five parents on every miss.

**Workaround.** CEF is loaded and initialized lazily, when the first CEF web view needs a
browser, and the static fields create no CefGlue objects, so a session without a web view
never runs CefRuntime's static constructor. Resolver misses after loading are timed and
logged.

**Verify removal.** Initialize CEF at startup instead. Build the Avalonia flavor for
Linux with debug logging on, play a game, and check that the log shows no slow
"resolver scan" lines.

Checks:
- 2026-10-23 | CefGlue.Next.Core 154.8037.98 | unverified | Upstream has no issue tracker.

## WA-CEF-0003: Chromium's X11 clipboard blocks off-screen pages

- Status: active
- Added: 2026-10-20 (from git history)
- GnollHack version: 3.28 (428)
- Component: Chromium
- Package: cef.runtime.linux-x64
- Defective version: cef.runtime.linux-x64 154.0.34
- Platforms: Linux
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackM/Platforms/Avalonia/Cef/GHCefWebViewControl.cs` (`GHCefWebViewControl.OnKeyDown`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/Cef/GHCefWebViewControl.cs` (`GHCefWebViewControl.CopySelectionAsync`); `win/win32/xpl/GnollHackM/Platforms/Avalonia/Cef/GHCefWebViewControl.cs` (`GHCefWebViewControl.PasteClipboardTextAsync`)
- Remove when: Chromium's clipboard works in GnollHack's windowless CEF browser in a CEF release GnollHack uses, and Verify removal passes.

**Symptom.** Pasting into a page blocks it, and copying from a page does not reach the
system clipboard.

**Cause.** Chromium's own X11 clipboard misbehaves in GnollHack's windowless browser,
whose CEF work runs on the Avalonia UI thread through the external message pump. The
exact mechanism has not been analyzed.

**Workaround.** Copy, cut and paste keys are handled before CEF sees them: the selection
CEF reported goes onto the clipboard through Avalonia, a cut then sends Delete, and a
paste inserts Avalonia's clipboard text with `ImeCommitText`.

**Verify removal.** Remove the clipboard-key branches of `OnKeyDown`. Build the Avalonia
flavor for Linux with CEF, and copy from and paste into the Overseer page's text fields
with Ctrl+C and Ctrl+V; the page must not hang, and text must move to and from other
applications.

Checks:
- 2026-10-23 | cef.runtime.linux-x64 154.0.34 | unverified | No upstream issue found.
