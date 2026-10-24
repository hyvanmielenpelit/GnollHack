# GnollHack Workaround and Known-Issue Register

GnollHack works around defects and limitations in the components it is built on: .NET
MAUI, SkiaSharp, the .NET runtime, .NET for Android and iOS, WinUI 3, Avalonia, CEF, FMOD,
Sentry, and the operating systems themselves. This directory records every such
workaround, one file per component, so that anyone can answer "why is this code here?",
"was this ever reported upstream?" and "can we remove it now that the new release is
out?" without reconstructing the history from the code.

Each entry has a stable ID, such as `WA-MAUI-0007`. The code that implements the
workaround carries a matching `WORKAROUND(WA-MAUI-0007)` tag. `Test-WorkaroundRegister.ps1`
keeps the two consistent: every tag has an entry, and every active entry has its tags.

The same files also record **known issues**: defects GnollHack has no workaround for and is
waiting for upstream to fix. They have IDs such as `KI-RUNTIME-0001`, follow the
workarounds in each file, and carry no code tags. See [Known Issues](#known-issues).

## Scope

A workaround entry is required for GnollHack code that compensates for a **defect**, an
**undocumented limitation**, or a **missing feature** in a third-party component.

Out of scope:

- GnollHack's own bugs, even when the fix looks like a workaround.
- Documented behavior that only needs to be used correctly, for example `DOTNET_*`
  environment variables being parsed as hexadecimal.
- Platform requirements, for example 16 KB page alignment on Android or the iOS scene
  manifest.
- Warnings suppressed for GnollHack's own legacy code, for example `-Wno-error=` flags for
  newer compilers.

## Components

An entry goes in the file of the component that would have to **ship the fix**, which is
where an upstream issue would be filed. The platforms affected are a field, not a file.
If the fix belongs in a wrapper (for example SkiaSharp's MAUI views), file the entry
under the wrapper. If ownership is unknown, file it under the component where the
symptom appears, and link the other with `Related`. An entry is never duplicated across
files. A file's `- ID prefix:` line gives both of its series: `WA-RUNTIME` also gives
`KI-RUNTIME`.

| File | Workaround prefix | Covers |
|------|-----------|--------|
| [maui.md](maui.md) | `WA-MAUI` | dotnet/maui: Controls, Core handlers and mappers, Essentials, Shell, Resizetizer |
| [skiasharp.md](skiasharp.md) | `WA-SKIASHARP` | mono/SkiaSharp, SkiaSharp.Views.Maui, mono/skia (Skia), HarfBuzzSharp |
| [dotnet_runtime.md](dotnet_runtime.md) | `WA-RUNTIME` | dotnet/runtime: CoreCLR, Mono, GC, BCL, AOT compilers |
| [android.md](android.md) | `WA-ANDROID` | dotnet/android, the Android OS, AndroidX and Google Play libraries, vendor quirks |
| [ios.md](ios.md) | `WA-IOS` | dotnet/macios, iOS, iPadOS, UIKit, WebKit on Apple, Mac running the iPad app |
| [winui.md](winui.md) | `WA-WINUI` | WinUI 3, Windows App SDK, CsWinRT |
| [windows.md](windows.md) | `WA-WINDOWS` | The Windows OS, Win32, DWM, WebView2 runtime |
| [avalonia.md](avalonia.md) | `WA-AVALONIA` | Avalonia, Avalonia.Controls.Maui, Avalonia's web view adapters |
| [cef.md](cef.md) | `WA-CEF` | CEF, CefGlue, Chromium |
| [linux.md](linux.md) | `WA-LINUX` | Linux distributions, glibc, FreeType, fontconfig, X11, AppImage, Steam runtime |
| [fmod.md](fmod.md) | `WA-FMOD` | FMOD Core and FMOD Studio |
| [sentry.md](sentry.md) | `WA-SENTRY` | sentry-dotnet, Sentry.Maui, sentry-native, sentry-cocoa |
| [build_tooling.md](build_tooling.md) | `WA-BUILD` | MSBuild, the .NET SDK, Visual Studio, NuGet, C compilers and linkers |

A new component gets a new file with the header below and a row in this table. The lint
reads the prefix from the file header, so nothing else needs to change.

### Component file header

```markdown
# .NET MAUI Workarounds

- Component: .NET MAUI (Microsoft.Maui.Controls, Microsoft.Maui.Core, Microsoft.Maui.Essentials)
- Upstream: https://github.com/dotnet/maui
- ID prefix: WA-MAUI

Entries follow the schema in [README.md](README.md). Append workarounds before the
`<!-- known-issues -->` line if there is one, otherwise at the end of the file, and known
issues at the end of the file. Never renumber or reuse an ID.

<!-- entries -->
```

A file with no entries has the line `No entries.` after the marker. Delete it when the
first entry is added. A file's first known issue adds the line `<!-- known-issues -->` after
its last workaround, or directly after `<!-- entries -->` when it has none. Known issues
follow that marker. The `No entries.` line means the file has no entries of either kind.

## Entry Schema

### Example (illustrative, not a real entry)

```markdown
## WA-EXAMPLE-0001: Widget forgets its color after a theme change

- Status: active
- Added: 2026-10-14
- GnollHack version: 3.27 (427)
- Component: Example Widgets
- Package: Example.Widgets
- Defective version: Example.Widgets 2.4.1
- Platforms: Android, iOS
- Upstream issue: https://github.com/example/widgets/issues/1234
- Upstream status when added: reported
- Upstream status: fixed-unreleased
- Fixed upstream in: https://github.com/example/widgets/pull/1240
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX/ExampleView.cs` (`ExampleView.OnThemeChanged`); `win/win32/xpl/GnollHackM/MauiProgram.cs` (`ConfigureWidgetMappers`)
- Remove when: GnollHack uses an Example.Widgets release containing the fix, and Verify removal passes.
- Related: WA-EXAMPLE-0002

**Symptom.** After switching between light and dark themes, widgets show the default
color.

**Cause.** The widget's theme handler resets the color mapping and does not re-apply it.

**Workaround.** The color is re-applied from `OnThemeChanged`, and the mapper skips the
reset.

**Verify removal.** Remove both tagged sites. Then:
- Build for Android and iOS.
- Switch theme three times on each, and confirm every widget keeps its color.

Checks:
- 2026-10-14 | Example.Widgets 2.4.1 | still-needed | Reproduced on Android 16 and iOS 26.5.
- 2026-10-23 | Example.Widgets 2.4.1 | still-needed | Fix merged upstream, not released.
```

### Heading

`## <ID>: <title>`. The ID is `<prefix>-NNNN`: four digits, the next free `WA-` number in
the file, never reused, and gaps are allowed. The title names the defect in a few words.

### Fields

The fields come directly after the heading, one per line, in this order, and end at the
first blank line. All are required unless marked otherwise.

| Field | Content |
|-------|---------|
| `Status` | The workaround's own state: `active`, `removable`, `removed` or `superseded`. |
| `Added` | ISO date the workaround landed. An entry reconstructed from git history appends ` (from git history)`. |
| `GnollHack version` | `ApplicationDisplayVersion (ApplicationVersion)` from `win/win32/xpl/GnollHackM/GnollHackM.csproj` when the workaround landed, for example `3.30 (430)`. The version is bumped at the start of a development cycle, so this is the first release that ships the workaround. For history before `GnollHackM.csproj` existed, use `game <VERSION_MAJOR>.<VERSION_MINOR>.<PATCHLEVEL>-<EDITLEVEL>` from `include/patchlevel.h` at that commit, the release tag format (`4.3.0-27`). |
| `Component` | The human name of the component, matching the file. |
| `Package` | The NuGet package ID, or the SDK, workload, OS or library name, such as `workload ios`, `iOS` or `FMOD`. Several are separated by `, `. `none` only for a component with no versioned artifact. The lint's version-drift warning keys on this field. |
| `Defective version` | The version GnollHack used when the defect was seen, with the package name: `Microsoft.Maui.Controls 10.0.120`, `FMOD 2.03.14`, `iOS 27.0`. Add the affected range if upstream states one. |
| `Platforms` | Comma-separated tokens from `Android`, `iOS`, `Mac (iPad app)`, `Windows`, `Linux`, or `all`. A token may take a ` (Avalonia)` or ` (MAUI)` suffix when only one flavor is affected; the suffix applies to that token only, as in `Linux, Windows (Avalonia)`. |
| `Upstream issue` | One or more URLs separated by `, `, or `none`, or `none (report drafted, not filed)`. |
| `Upstream status when added` | The upstream status when the workaround landed. **Never edited later.** |
| `Upstream status` | The current upstream status, as of `Last checked`. |
| `Fixed upstream in` | A version, a PR or commit URL, or both. Required when `Upstream status` is `fixed-unreleased` (the PR or commit) or `released` (a release version), and when `Status` is `removable`. Optional otherwise. |
| `Last checked` | ISO date of the newest line in `Checks`. |
| `Code` | The **sites**, separated by `; `. A site is one backticked repository-relative path followed by the symbol name in parentheses; the same path may repeat with different symbols. Never line numbers, which drift. Each site carries one tag. If there is nothing to tag, write `none (<reason>)`. A `removed` or `superseded` entry keeps its historical sites. |
| `Remove when` | The exit criterion: which release, plus a passing **Verify removal**. For `wontfix`, what would change that, or `never (upstream declined)`. |
| `Related` | *Optional.* Other workaround or known-issue IDs, Sentry issue IDs (`GNOLLHACK-GK`), GnollHack commit hashes. |
| `Removed` | *Required if `removed`.* `<ISO date>, <GnollHack version>`, then the commit hash once known. |
| `Superseded by` | *Required if `superseded`.* The replacing workaround ID. |

`unknown` is allowed in `GnollHack version`, `Defective version` and
`Upstream status when added`, but only on entries marked `(from git history)`, where
history cannot tell.

### Prose

After the fields come paragraphs, each starting with a bold label:

- `**Symptom.**` (required): what goes wrong without the workaround.
- `**Cause.**` (expected): what the component does wrong, as far as it is known.
- `**Workaround.**` (required): what GnollHack does about it.
- `**Verify removal.**` (expected): a concrete procedure, saying what to disable, what to
  build, what to run, and what must hold.

### Checks

`Checks:` on its own line, then one line per check, oldest first, newest last:

```text
- <ISO date> | <versions checked> | <result> | <note>
```

The first line is the check made when the entry was written. A re-check appends a line
and updates `Upstream status`, `Fixed upstream in`, `Last checked` and, if warranted,
`Status`. Earlier lines are never edited. The package version at the last check is the
second column of the newest line.

## Vocabularies

Two separate axes, so that "fixed upstream" is never confused with "removed from
GnollHack".

**Upstream status:**

| Value | Meaning |
|-------|---------|
| `unreported` | The defect is known to us, and no upstream issue exists. |
| `reported` | An issue is open, and the defect is not fixed. |
| `fixed-unreleased` | A fix is merged upstream, but no release of the kind GnollHack ships with contains it yet (see `released`). |
| `released` | A release GnollHack could ship on contains the fix: a stable release, or a preview for a component GnollHack only uses as previews. GnollHack may not use that release yet. |
| `wontfix` | Upstream declined, or closed the issue as by design. |
| `not-needed` | Nothing to fix upstream: the limitation is inherent or the behavior is documented. |
| `unknown` | Only in `Upstream status when added`, on entries reconstructed from git history. |

**Status:**

| Value | Meaning |
|-------|---------|
| `active` | The workaround is in the code and still needed. |
| `removable` | The fix is released and adopted, and **Verify removal** passed; the code has not been removed yet. |
| `removed` | The code is gone. The entry stays, so the history and the ID are kept. |
| `superseded` | Another entry, named in `Superseded by`, replaced this one. |

**Check result:**

| Value | Meaning |
|-------|---------|
| `still-needed` | The defect still reproduces, or upstream has not fixed it. |
| `not-needed` | Verified that the workaround is no longer needed. |
| `unverified` | The defect could not be re-tested; only the upstream status was refreshed. |
| `removed` | The workaround was removed from the code in this change. |

## Known Issues

A **known issue** is a third-party defect that GnollHack has no workaround for, or only a
partial one, and is waiting for upstream to fix. It is recorded in the same component file
as the workarounds, so that the file answers "what does upgrading this component fix for
us?" in one place. Known issues have IDs such as `KI-RUNTIME-0001`, where the part after
`KI-` comes from the file's `- ID prefix:` line, and they carry no code tags.

### Scope

An entry is required when all of these hold:

- The defect is in a third-party component, under the ownership rule in
  [Components](#components).
- Its cause is **attributed with evidence**: a stack into the component, a reproduction, or
  upstream's acknowledgement. A crash still under investigation stays in Sentry and the
  analysis reports until then.
- GnollHack has no workaround for it. A partial workaround gets a workaround entry for the
  part it covers and a known issue for the rest, linked through `Related`.
- It matters: a crash, hang, data loss or malfunction a user can see, a measurable
  performance cost, or a build failure.

Out of scope: GnollHack's own bugs, crash buckets with no attributed cause, Sentry noise
(archive it in Sentry with a comment instead), and documented behavior.

### Placement

Known issues follow the line `<!-- known-issues -->`, which comes after the file's last
workaround, or directly after `<!-- entries -->` when the file has no workarounds. New
workarounds go before that line, and new known issues at the end of the file. Each series is
numbered on its own: the next known-issue ID is the highest `KI-` number in the file plus
one. IDs are never renumbered or reused.

### Example (illustrative, not a real entry)

```markdown
## KI-EXAMPLE-0001: Widget renderer aborts when the GPU driver resets

- Status: open
- Added: 2026-10-23
- GnollHack version: 3.29 (429)
- Component: Example Widgets
- Package: Example.Widgets
- Defective version: Example.Widgets 2.4.1
- Platforms: Android
- Impact: crash
- Sentry: GNOLLHACK-ZZ
- Upstream issue: https://github.com/example/widgets/issues/1300
- Upstream status when added: fixed-unreleased
- Upstream status: fixed-unreleased
- Fixed upstream in: https://github.com/example/widgets/pull/1310
- Last checked: 2026-10-23
- Resolve when: GnollHack uses an Example.Widgets release containing the fix, and Verify fix passes.

**Symptom.** After a GPU driver reset, the app aborts in the widget renderer.

**Cause.** The renderer keeps a context that the reset invalidated.

**Why no workaround.** The context is private to the renderer and is used on its own
thread; GnollHack cannot observe the reset before the renderer touches it.

**Verify fix.** After adopting the fixed release, no event in GNOLLHACK-ZZ comes from a
release built with it for 90 days.

Checks:
- 2026-10-23 | Example.Widgets 2.4.1 | still-present | Fix merged upstream, not released.
```

### Fields

In this order. Fields marked "as for workarounds" follow the [Fields](#fields) table above.

| Field | Content |
|-------|---------|
| `Status` | `open`, `worked-around`, `resolved` or `obsolete`; see below. |
| `Added` | As for workarounds. |
| `GnollHack version` | The first release in which an event attributed to the defect was seen, in the workaround format, or `unknown` when there is no such event (for example a performance defect measured outside Sentry). |
| `Component`, `Package`, `Defective version`, `Platforms` | As for workarounds. |
| `Impact` | `crash`, `hang`, `data-loss`, `malfunction`, `performance` or `build`. |
| `Sentry` | Sentry issue short IDs separated by `, `, such as `GNOLLHACK-HJ`, or `none`. |
| `Upstream issue`, `Upstream status when added`, `Upstream status`, `Fixed upstream in` | As for workarounds. `Fixed upstream in` is also required when `Status` is `resolved`. |
| `Last checked` | As for workarounds. |
| `Resolve when` | The exit criterion: which release GnollHack must ship, plus a passing **Verify fix**. |
| `Worked around by` | *Required if `worked-around`.* The workaround ID. |
| `Related` | *Optional.* As for workarounds; may name workaround and known-issue IDs. |
| `Resolved` | *Required if `resolved`.* `<ISO date>, <GnollHack version>`, then the commit hash once known. |

There is no `Code` field. If a code site matters, for example the call that triggers the
defect, the prose names it.

### Prose

- `**Symptom.**` (required): what users or the team see.
- `**Cause.**` (expected): the mechanism, as far as it is known, and what is unverified.
- `**Why no workaround.**` (required): what was considered and why it was rejected, is
  impossible, or has not been tried. This paragraph stops the next investigator from
  repeating the same dead ends.
- `**Verify fix.**` (expected): how to confirm the fix after adopting it, either a
  reproduction or what Sentry must show and for how long.
- `**Player guidance.**` (optional): what to tell a player who reports it.

### Checks

The same line format as for workarounds, with these results:

| Value | Meaning |
|-------|---------|
| `still-present` | The defect still occurs, or upstream has not shipped a fix GnollHack uses. |
| `not-present` | Verified that the defect no longer occurs. |
| `unverified` | Only the upstream status was refreshed. |
| `resolved` | The entry is closed as resolved in this change. |

### Status

| Value | Meaning |
|-------|---------|
| `open` | No workaround; waiting for upstream. |
| `worked-around` | A workaround now covers it, named in `Worked around by`. The entry stays open in spirit until that workaround is removed, and is resolved in the same change. |
| `resolved` | GnollHack ships a release containing the fix, and **Verify fix** passed. |
| `obsolete` | The component or the affected code path is no longer used. |

### Lifecycle

- **A workaround is found later:** add the workaround entry, with `Related` naming the known
  issue. Set the known issue to `worked-around`, add `Worked around by`, and append a check
  line.
- **GnollHack ships the fix:** resolve the Sentry issue as [Sentry](#sentry) step 4 says.
  The entry stays `open`, with `Upstream status: released`, `Fixed upstream in` naming the
  release, and an `unverified` check line naming the adopted version, until **Verify fix**
  passes.
- **Verify fix passes:** set `resolved`, add `Resolved`, and append a `resolved` check line.
  A `worked-around` known issue is resolved in the same change that removes its workaround.
- **The component or code path is no longer used:** set `obsolete` and append a
  `not-present` check line whose note says why.
- **Re-checks** follow the same triggers and depths as for workarounds: package, SDK,
  workload, FMOD or OS upgrades, upstream changes, and sweeps.

### Sentry

Every known issue with a Sentry issue is linked both ways:

1. The entry's `Sentry` field names the issue.
2. A note on the Sentry issue names the entry ID, the upstream URL, the fix version if known,
   and the register file.
3. Until GnollHack ships the fix, the Sentry issue is **archived until escalating**. It is never archived forever, which would hide a new trigger, and never left
   unresolved, which would raise it in every triage.
4. When GnollHack ships the fix, resolve the Sentry issue **in the newest shipped release
   that does not contain the fix**, using "another existing release" in the Sentry UI. Any
   event from a later version then reopens it as a regression.
   - Do not use "next release" or "current release", and do not resolve in the fixing
     release itself. Sentry decides a regression by version order, and an event from the
     same version as the resolving release does not regress it.
   - Observed on GNOLLHACK-FN: it was resolved in `GnollHackM@3.25+…`, and a 3.25+425 event
     raised no regression.
5. The register is the durable record. Sentry keeps error events for 90 days.

## Code Tags

A tag is **one new line, inserted directly above** the comment or code that implements
the workaround. Existing explanatory comments and upstream URLs stay where they are.

| Language | Form |
|----------|------|
| C# | `// WORKAROUND(WA-MAUI-0007): <title>` |
| C, C headers | `/* WORKAROUND(WA-SENTRY-0001): <title> */` |
| XAML, MSBuild, `.plist`, `.manifest`, Android XML | `<!-- WORKAROUND(WA-BUILD-0002): <title> -->` |
| PowerShell, shell | `# WORKAROUND(WA-LINUX-0002): <title>` |

- Indent the tag like the line below it.
- After the colon comes the entry's title, shortened if needed to stay within 120
  columns.
- A workaround spread over several sites has a tag at each.
- A tag goes on its own line above a statement, member or element. It never goes inside
  an expression, an attribute list or a `#define` continuation, and never before an
  `<?xml` declaration.
- XAML is tagged only in the `GnollHackX` source, never in a generated `GnollHackM` copy.
  `makedefsdroid` carries the tag over.
- Known issues have no tags. A `WORKAROUND(KI-...)` tag is an error.

Find every site with:

```powershell
git grep -n -E "WORKAROUND\(WA-[A-Z]+-[0-9]{4}\)"
```

## Procedures

AI agents follow the `client_workarounds` skill, which has the full procedure, including
where to find each version number.

- **Adding a workaround.** Choose the file from the component table and take the next
  free ID. Fill in the fields, the prose and the first `Checks` line. Insert the tags in
  the same change, and run the lint.
- **Re-checking.**
  - When a package is bumped, refresh `Upstream status`. Append a `Checks` line only when
    the status changed or the new version contains the fix.
  - A full re-check also runs **Verify removal**.
  - The lint warns when the version of a NuGet package (a `$(...)` version is resolved
    from the csproj) or of sentry-native (from the native binaries' build manifests) no
    longer matches the last check of an entry whose upstream status is `fixed-unreleased`
    or `released`. For SDK, workload, WinUI, FMOD and OS bumps, find the entries by their
    `Package` field.
- **Removing.**
  - Run **Verify removal**, then delete the code and its tags.
  - Set `Status: removed`, add the `Removed` field, append a `Checks` line with the result
    `removed`, and update `Last checked`.
  - Keep the entry.
- **ID collisions.** Two changes made at the same time may take the same ID. The lint
  reports the duplicate; the later change renumbers its own new entry and its tags
  together.
- **Known issues.** Add, re-check and close them as [Known Issues](#known-issues)
  describes, including the Sentry steps.

## Lint

From the repository root:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File DEVEL\workarounds\Test-WorkaroundRegister.ps1
```

| Parameter | Effect |
|-----------|--------|
| `-RegisterOnly` | Checks the register files only, not the code tags. |
| `-CheckUpstream` | Also asks GitHub, through `gh`, whether the state of each linked issue or pull request matches `Upstream status`. Needs the network. |
| `-StaleDays <n>` | Age in days after which an active workaround or open known issue is reported as stale (default 180). |
| `-RepositoryRoot <path>` | The repository to check (default: two levels above the script). |

Known issues are checked against their own field set and vocabularies, and a
`WORKAROUND(KI-...)` tag is an error.

Exit code `0` means no errors (there may be warnings), `1` means errors were found, and `2`
means the script could not run.

## Background

The format follows established practice for tracking downstream patches and workarounds:

- [Debian DEP-3](https://dep-team.pages.debian.net/deps/dep3/): the `Bug`, `Forwarded`,
  `Applied-Upstream` and `Last-Update` fields.
- [Chromium's GPU driver bug list](https://chromium.googlesource.com/chromium/src/+/HEAD/gpu/config/gpu_driver_bug_list.json):
  stable IDs and per-entry OS and version conditions.
- [Electron's patch policy](https://github.com/electron/electron/blob/main/docs/development/patches.md):
  every patch says how to check later whether it is still needed.
- [Homebrew](https://docs.brew.sh/Formula-Cookbook) and
  [WebKit](https://bugs.webkit.org/show_bug.cgi?id=254389): a code comment that leads back
  to the reason.
- [ITIL problem management](https://en.wikipedia.org/wiki/Known_error): a known error
  record with or without a workaround.
- [Debian's bug control server](https://www.debian.org/Bugs/server-control): the
  `forwarded` command and the `upstream` tag.
- [Sentry's issue states](https://docs.sentry.io/product/issues/states-triage/): archive
  until escalating, and resolve in a release.
