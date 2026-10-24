---
name: client_workarounds
description: >-
  How to record, re-check, and remove GnollHack's workarounds for defects and limitations
  in third-party components -- .NET MAUI, SkiaSharp, the .NET runtime, .NET for Android
  and iOS, WinUI, Avalonia, CEF, FMOD, Sentry, and the operating systems -- and the known
  issues it cannot work around, in the register in DEVEL/workarounds. Covers WA and KI
  entries, IDs, every field, version sources, upstream status, WORKAROUND(WA-...) tags,
  the lint, linking a known issue to its Sentry issue, re-checks after a package, SDK or
  workload upgrade, and removal or resolution. Read whenever you add, change or remove a
  workaround, conclude that a crash or defect is upstream's with no workaround, upgrade a
  NuGet package, SDK or workload, file or update an upstream issue, meet a
  WORKAROUND(WA-...) tag or a KI- ID, or are asked to re-check the register.
---

# Workaround and Known-Issue Register

`DEVEL/workarounds/README.md` is the source of truth for the schema, the vocabularies, the
tag forms and the known-issue rules. This skill is the procedure. Read the README before
writing an entry.

## Critical Rules

- **Register and tag in the same change.** A workaround added without its entry, or an
  entry without its tags, is unfinished work. The lint fails on both.
- **A defect upstream must fix, with no workaround, is a known issue.** Record it as a
  `KI-` entry once its cause is attributed, and link its Sentry issue both ways. It has no
  code tags.
- **Never archive a known issue's Sentry issue forever.** Archive it until escalating,
  and resolve it only in the newest release without the fix, as the README's Known Issues
  section says.
- **Run the lint** after every register or tag change, from the repository root:
  `powershell -NoProfile -ExecutionPolicy Bypass -File DEVEL\workarounds\Test-WorkaroundRegister.ps1`.
- **Never edit `Upstream status when added`** or an existing `Checks` line. They are
  history.
- **Never renumber, reuse or delete an ID.** A removed workaround keeps its entry.
- **Prefer `unknown` to a guess** where the schema allows it. A wrong version or status
  is worse than an honest gap.
- **No version numbers in this skill.** Read them from the files named below.

## When This Applies

An entry is required for GnollHack code that compensates for a **defect**, an
**undocumented limitation**, or a **missing feature** in a third-party component. This
includes a whole custom handler written because the stock one is broken, a reflection
call into a framework's internals, a suppressed framework exception, an MSBuild target
that patches a broken SDK target, and a deliberately omitted call that is known to
misbehave.

Out of scope: GnollHack's own bugs, documented behavior that only needs correct use,
platform requirements (16 KB page alignment, the iOS scene manifest), and warnings
suppressed for our own legacy code. Before calling something a framework bug, read the
framework's code at the shipped version, as `client_framework_sources` describes.

## Adding an Entry

1. **Choose the file** from the component table in the README: the component that would
   have to ship the fix. If the fix belongs in a wrapper, use the wrapper's file. If
   ownership is unclear, use the component where the symptom appears, and link the other
   entry with `Related`.
2. **Take the next ID**: the highest `WA-` number in that file plus one, four digits. If you
   are one of several agents adding entries for the same component, use the ID the
   orchestrator reserved for you. Insert a new workaround before the file's
   `<!-- known-issues -->` line, if it has one; otherwise at the end.
3. **Fill the fields** in schema order, using the version sources below. `Added` is
   today. The first `Checks` line is today's check, dated the same as `Last checked`.
4. **Write the prose**: `**Symptom.**` and `**Workaround.**` are required, and
   `**Cause.**` and `**Verify removal.**` are expected. **Verify removal** must be a
   procedure someone else can run: what to disable, what to build, what to run, and what
   must hold.
5. **Insert the tags**, one new line directly above each site, in the form the README
   gives for the language. Leave existing lines unchanged. In `GnollHackX` XAML the
   generated `GnollHackM` copy picks the tag up when `makedefsdroid` runs; never edit the
   generated copy.
6. **Delete `No entries.`** if this is the file's first entry.
7. **Run the lint.**

If an existing comment at the site already cites the upstream issue, keep it; the tag
goes above it.

## Version Sources

| Value | Source |
|-------|--------|
| GnollHack version | `ApplicationDisplayVersion` and `ApplicationVersion` in `win/win32/xpl/GnollHackM/GnollHackM.csproj`, written `<display> (<build>)`. For history before that file existed, `VERSION_MAJOR`, `VERSION_MINOR`, `PATCHLEVEL` and `EDITLEVEL` in `include/patchlevel.h`, written `game <major>.<minor>.<patch>-<edit>` |
| NuGet packages | The `PackageReference` in `GnollHackM.csproj`. MAUI, SkiaSharp and Avalonia have several references selected by conditions (`GnhAvalonia`, `SKIASHARP_3119`, the target framework); record the one in effect for the affected platform; `Sentry` and `Sentry.Maui` take their version from the `GnhSentryVersion` property |
| .NET SDK | The `global.json` that applies: `win/win32/xpl/GnollHackM/global.json`, or `win/win32/xpl/GnollHackM/Platforms/Avalonia/global.json` for the Avalonia flavor |
| Workloads (android, ios, maui-windows) | `dotnet workload list`, run in `win/win32/xpl/GnollHackM` |
| WinUI / Windows App SDK | `Microsoft.WindowsAppSDK.WinUI` in `win/win32/xpl/GnollHackM/obj/project.assets.json` after a Windows restore |
| FMOD | `VERSION.number` in `win/win32/xpl/GnollHackX/GnollHackX.FMOD/fmod.cs`, a hexadecimal `0x00MMmmpp` |
| sentry-native | `SENTRY_SDK_VERSION_*` in `win/win32/xpl/gnhsentrywin/include/sentry.h`, recorded in the build manifests next to `gnhsentry.dll` and `libsentry.so` |
| OS versions | The device or machine where the defect was seen |

For the matching tag or commit in a framework's own repository, use the table in
`client_framework_sources` rather than guessing.

For an entry reconstructed from git history, read the same files at the introducing
commit (`git show <commit>:<path>`), mark `Added` with ` (from git history)`, and use
`unknown` where history cannot tell.

## Upstream Status

- **An issue is linked:** run `gh issue view <n> --repo <owner>/<repo> --json state,closedAt,stateReason`
  and read the linked pull requests. A closed issue is not a released fix.
- **A fix is merged:** it is `fixed-unreleased` until a release contains it. Check with
  `git -C <clone> tag --contains <merge commit>` in the local framework clone, which is a
  read-only operation, per `client_framework_sources`. A release contains it once that
  lists a tag of the kind GnollHack ships with: a stable release, or a preview for a
  component GnollHack only uses as previews (the README's `released` row). Then it is
  `released`, and `Fixed upstream in` names the release.
- **No issue is linked:** search once, with
  `gh search issues --repo <owner>/<repo> "<keywords>"`. Link what you find, or record
  `unreported`. If you draft a report with `agent-issue-drafting`, write
  `none (report drafted, not filed)`. Once the user files it, put the URL in
  `Upstream issue`.
- **Non-GitHub trackers** (the FMOD forum, Apple Feedback, Feedback Hub): link the page,
  and state what it says in the newest `Checks` note.

## Re-Checking

Triggers:

- A package, SDK, workload, FMOD or sentry-native upgrade. For NuGet packages and
  sentry-native the lint's version-drift warning lists the upstream-fixed entries that
  matter. For everything else, search the `Package` fields of workarounds and known issues.
- The upstream issue closes or its fix ships.
- The user asks for a sweep of the register.

There are two depths of re-check:

- **Minimal**, for routine upgrades: refresh `Upstream status` and `Fixed upstream in`.
  Append a `Checks` line only when the status changed or the new version contains the
  fix. Otherwise leave the entry alone, so that one routine bump does not add identical
  lines to every entry.
- **Full**: run **Verify removal**, then append a `Checks` line with `still-needed`,
  `not-needed` or, if it could not be tested, `unverified`.

For a known issue, a minimal re-check refreshes `Upstream status` and `Fixed upstream in`,
and appends a `Checks` line only when the status changed or the new version contains the
fix. A full re-check runs **Verify fix**, or reads its Sentry issue's events since the last
check, and appends `still-present`, `not-present` or `unverified`.

Every appended line also updates `Last checked`. Set `Status: removable` only after a
full re-check found `not-needed` and GnollHack uses the fixed release.

`-CheckUpstream` makes the lint compare the state of every linked GitHub issue or pull
request with the entry. It uses the network, so run it during sweeps, not in CI.

## Removing

1. Run **Verify removal** with the workaround disabled.
2. Delete the workaround code and every one of its tags.
3. Set `Status: removed`, add `Removed: <ISO date>, <GnollHack version>`, append a
   `Checks` line with the result `removed`, and update `Last checked`.
4. Keep the entry and its `Code` sites, which now record where the workaround was.
5. Run the lint. It fails if a tag for the removed entry is left anywhere.

A workaround replaced by a different one: the new one gets its own entry, and the old
one is set to `superseded` with `Superseded by` naming the new ID. Its tags are removed,
and the new entry's tags replace them.

## Known Issues

1. **When**: a crash analysis, framework-source reading or measurement attributes a defect
   to a third-party component, and GnollHack has no workaround, or only a partial one. The
   README's Known Issues scope decides.
2. **Adding**: choose the component file as for workarounds. If the file has no
   `<!-- known-issues -->` marker, add it after the last workaround. Take the next `KI-`
   number in that file. Fill the fields in the README's order:
   - `GnollHack version` is the release of the first attributed Sentry event, read from
     Sentry, or `unknown`.
   - `Sentry` lists the issue IDs.
   - Write **Why no workaround.** from what was actually considered. "Not tried" is an
     honest answer; a guess is not.
   - Delete `No entries.` if this is the file's first entry of either kind.
3. **Sentry**: with the Sentry MCP server, post a note on each issue in `Sentry` naming the
   entry ID, the upstream URL, the fix version if known, and the register file, and set it
   to ignored with `ignoreMode='untilEscalating'`. `update_issue` with a `reason` does
   both. An issue that is resolved, or ignored in another mode, must first be set to
   `unresolved`. Changing a Sentry issue is visible to the whole team: do it in the change
   that adds the entry, as part of an approved plan or when the user asks.
4. **Partial workarounds**: one `WA-` entry for the covered path and one `KI-` entry for the
   residue, each naming the other in `Related`.
5. **Transitions**: follow the README's Lifecycle.
   - **Workaround found later:** `Status: worked-around`, `Worked around by`, and a
     `still-present` check line naming the workaround.
   - **Fix shipped:** set `Upstream status: released` and `Fixed upstream in`, append an
     `unverified` check line naming the adopted version, and resolve the Sentry issue in
     the newest release without the fix, using "another existing release" in the Sentry UI.
     The MCP `update_issue` cannot resolve in a chosen release. It offers only plain
     `resolved` and `resolvedInNextRelease`, and the latter would name the fixing release.
     So ask the user to do it in the UI. The entry stays `open`.
   - **Verify fix passed:** set `Status: resolved`, add `Resolved`, and append a `resolved`
     check line.
   - **Component or code path gone:** set `Status: obsolete` and append a `not-present`
     check line saying why.
6. **Run the lint.**

## Concurrency

Two sessions can take the same next ID. The lint reports the duplicate. The later change
renumbers its own new entry and its code tags together, and never the earlier entry.
When several subagents add entries for one component, the orchestrator assigns each
register file to exactly one agent (see `client_subagent_guidelines`).

## Related Skills

- **`client_framework_sources`**: reading a framework's code at the shipped version and
  deciding whether a bug is ours or theirs
- **`agent-issue-drafting`**: drafting the upstream report
- **`sentry_crash_analysis`**: when a crash leads into a third-party component, and where
  a known issue's Sentry issue is analyzed
- **`build_pipeline`**: package, SDK and workload upgrades
