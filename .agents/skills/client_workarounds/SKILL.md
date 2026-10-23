---
name: client_workarounds
description: >-
  How to record, re-check, and remove GnollHack's workarounds for defects and limitations
  in third-party components -- .NET MAUI, SkiaSharp, the .NET runtime, .NET for Android
  and iOS, WinUI, Avalonia, CEF, FMOD, Sentry, and the operating systems -- in the
  workaround register in DEVEL/workarounds. Covers what counts as a workaround, choosing
  the component file and the next WA ID, filling every field, where each version number
  comes from, establishing upstream status, the WORKAROUND(WA-...) code tag, the lint
  script, re-checks after a package, SDK or workload upgrade, and removal. Read whenever
  you add, change or remove a workaround for a framework or OS bug, upgrade a NuGet
  package, SDK or workload, file or update an upstream issue, meet a WORKAROUND(WA-...)
  tag, or are asked to re-check the workaround register.
---

# Workaround Register

`DEVEL/workarounds/README.md` is the source of truth for the schema, the vocabularies and
the tag forms. This skill is the procedure. Read the README before writing an entry.

## Critical Rules

- **Register and tag in the same change.** A workaround added without its entry, or an
  entry without its tags, is unfinished work. The lint fails on both.
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
2. **Take the next ID**: the highest number in that file plus one, four digits. If you
   are one of several agents adding entries for the same component, use the ID the
   orchestrator reserved for you.
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
| NuGet packages | The `PackageReference` in `GnollHackM.csproj`. MAUI, SkiaSharp and Avalonia have several references selected by conditions (`GnhAvalonia`, `SKIASHARP_3119`, the target framework); record the one in effect for the affected platform |
| .NET SDK | The `global.json` that applies: `win/win32/xpl/GnollHackM/global.json`, or `win/win32/xpl/GnollHackM/Platforms/Avalonia/global.json` for the Avalonia flavor |
| Workloads (android, ios, maui-windows) | `dotnet workload list`, run in `win/win32/xpl/GnollHackM` |
| WinUI / Windows App SDK | `Microsoft.WindowsAppSDK.WinUI` in `win/win32/xpl/GnollHackM/obj/project.assets.json` after a Windows restore |
| FMOD | `VERSION.number` in `win/win32/xpl/GnollHackX/GnollHackX.FMOD/fmod.cs`, a hexadecimal `0x00MMmmpp` |
| sentry-native | The tag named in `win/win32/xpl/gnhsentrywin/gnhsentry.c` |
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
  lists a release tag. Then it is `released`, and `Fixed upstream in` names the release.
- **No issue is linked:** search once, with
  `gh search issues --repo <owner>/<repo> "<keywords>"`. Link what you find, or record
  `unreported`. If you draft a report with `agent-issue-drafting`, write
  `none (report drafted, not filed)`. Once the user files it, put the URL in
  `Upstream issue`.
- **Non-GitHub trackers** (the FMOD forum, Apple Feedback, Feedback Hub): link the page,
  and state what it says in the newest `Checks` note.

## Re-Checking

Triggers:

- A package, SDK, workload, FMOD or sentry-native upgrade. For NuGet packages the lint's
  version-drift warning lists the entries that matter. For everything else, search the
  register's `Package` fields.
- The upstream issue closes or its fix ships.
- The user asks for a sweep of the register.

There are two depths of re-check:

- **Minimal**, for routine upgrades: refresh `Upstream status` and `Fixed upstream in`.
  Append a `Checks` line only when the status changed or the new version contains the
  fix. Otherwise leave the entry alone, so that one routine bump does not add identical
  lines to every entry.
- **Full**: run **Verify removal**, then append a `Checks` line with `still-needed`,
  `not-needed` or, if it could not be tested, `unverified`.

Every appended line also updates `Last checked`. Set `Status: removable` only after a
full re-check found `not-needed` and GnollHack uses the fixed release.

`-CheckUpstream` makes the lint compare every linked GitHub issue's state with the
entry. It uses the network, so run it during sweeps, not in CI.

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

## Concurrency

Two sessions can take the same next ID. The lint reports the duplicate. The later change
renumbers its own new entry and its code tags together, and never the earlier entry.
When several subagents add entries for one component, the orchestrator assigns each
register file to exactly one agent (see `client_subagent_guidelines`).

## Related Skills

- **`client_framework_sources`**: reading a framework's code at the shipped version and
  deciding whether a bug is ours or theirs
- **`agent-issue-drafting`**: drafting the upstream report
- **`sentry_crash_analysis`**: when a crash leads into a third-party component
- **`build_pipeline`**: package, SDK and workload upgrades
