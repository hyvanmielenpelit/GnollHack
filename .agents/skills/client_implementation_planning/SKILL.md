---
name: client_implementation_planning
description: >-
  Implementation planning overlay for the GnollHack client repository. Covers when a plan
  is required across the C core, native bridge, MAUI frontend, build utilities, and game
  data; the makedefs, levcomp, dgncomp, dlb, and makedefsdroid regeneration boundaries a
  plan step must never straddle; the XAML generation pipeline from GnollHackX to
  GnollHackM; and C-to-C# interop triggers. Read before any multi-file or cross-subsystem
  GnollHack change.
---

# GnollHack Implementation Planning (Project Overlay)

> **If the shared skills are not installed**, this is the whole baseline:
> a written plan is required for any change touching more than one file or crossing a
> build boundary; save it as
> `.plans/YYYY-MM-DD/task_name/implementation_plan_v1.md` (never overwrite a version --
> increment); print the path; **wait for explicit approval before editing any file**;
> track progress in `task.md`; finish with `walkthrough.md`. Install the full guidance
> from `https://github.com/hyvanmielenpelit/SharedAgentSkills` (`.\setup.ps1`).

> [!IMPORTANT]
> **Global baseline delegation.** The 5-phase lifecycle, plan document structure, the
> Execution Target line, `.plans/` naming and `_v<N>` versioning, follow-up rounds,
> progress tracking, walkthroughs, and `.plans/` research isolation are defined in the
> global **`agent-implementation-planning`** skill. Subagent tiers, how to resolve them,
> and file-level exclusivity are in **`agent-subagent-guidelines`**, with GnollHack's
> build chains in **`client_subagent_guidelines`**. Harness mechanics -- plan mode under
> Claude Code, artifact delivery under Antigravity -- are in `claude-plan-mode` or
> `gemini-antigravity-conventions`, whichever is installed for your application.
>
> The sections below are what is specific to **this repository**.

---

## Project Structure

GnollHack spans five subsystems, and a change that looks local often is not:

| Subsystem | Location |
|-----------|----------|
| C core game engine | `src/`, `include/` |
| Native bridge (C side) | `win/win32/xpl/libshare/` |
| MAUI frontend | `win/win32/xpl/GnollHackM/`, shared code in `win/win32/xpl/GnollHackX/` |
| Build utilities | `util/` -- `makedefs`, `levcomp`, `dgncomp`, `dlb` |
| Game data sources | `dat/` |

Adding a monster changes a data table, regenerates two headers, and may change a C# interop
struct. Editing a `.des` file changes nothing visible until `levcomp` and `dlb` have run.
Planning is what catches that before any source file is edited.

---

## When a Plan Is Required

A written implementation plan is **required** when the task meets **any** of these:

- It touches **more than one file**, or more than one subsystem from the table above
- It crosses a **build regeneration boundary**: `makedefs`, `levcomp`, `dgncomp`, `dlb`,
  or `makedefsdroid`
- It adds or changes a **C-to-C# interop** struct, enum, or callback
- It is a refactor, a new feature, or anything the user describes as large or non-trivial

A plan is **not** required for single-file fixes, typo and comment corrections, answering
questions, or read-only investigation.

**When in doubt, write a plan.** A rejected plan is cheap; a wrong cross-layer change is
not.

---

## Build Impact for This Repository

These are the generated-artifact boundaries a plan must account for. **A plan step must
never straddle one** -- finish the source change, cross the boundary, then continue.

### makedefs

Editing `src/monst.c` or `src/objects.c` regenerates `include/pm.h` and
`include/onames.h`. Those headers are **auto-generated and must never be hand-edited**,
and C# interop that references the generated constants must be updated *after* the
regeneration, not alongside it.

### levcomp / dgncomp / dlb

`.des` files in `dat/` compile to binary `.lev` files, which are then packaged into the
`nhdat` archive by `dlb`. **Never modify `nhdat` directly.** A plan that changes level
data must list the regeneration as its own step.

### makedefsdroid and the XAML generation trap

The **page and control** XAML files in `GnollHackM/` are **auto-generated** from
`GnollHackX/GnollHackX/` by the `makedefsdroid` build step.

- **Always edit the GnollHackX source**, never the generated GnollHackM copy.
- After modifying such a XAML file, regenerate by building `makedefsdroid` (locate MSBuild
  via `vswhere.exe`, then build `win/win32/vs/makedefsdroid.vcxproj`). The GnollHackM
  build will fail to resolve `x:Name` references from new XAML elements until it has run.
- **Exception -- app-level XAML is not generated** and has no GnollHackX source: `App.xaml`,
  `AppShell.xaml`, `Resources/Styles/Colors.xaml`, `Resources/Styles/Styles.xaml`. Edit
  those directly in GnollHackM.
- **Adding a new page or control requires a `makedefsdroid.vcxproj` edit** -- the transform
  is a hardcoded per-file list, not a wildcard. A new XAML file produces no MAUI output
  until a matching block is added. See the `maui_frontend` skill.

### The `WidthRequest  =` double-space convention

In GnollHackX XAML, `makedefsdroid` converts `WidthRequest  =` (two spaces before `=`)
into `MaximumWidthRequest=`, and likewise for `HeightRequest`. A plan touching XAML sizing
must say which form it intends:

- **Never write `MaximumWidthRequest=` in GnollHackX XAML** -- it does not exist in
  Xamarin.Forms and will not compile.
- **Never write the one-space form when the `Maximum` version is intended** -- elements
  come out overly wide or tall in GnollHackM.
- **Four files are exempt** because their `makedefsdroid.vcxproj` blocks omit the
  replacement: `Controls/CustomImageButton.xaml`, `Controls/LabeledImageButton.xaml`,
  `Controls/SimpleImageButton.xaml`, `Controls/SwitchableCanvasView.xaml`.

---

## Related Skills

- **`client_subagent_guidelines`** -- the build dependency chains that constrain
  parallelism, and this repository's escalation guidance
- **`build_pipeline`** -- the full data pipeline, `makedefs` flag reference, and MSBuild
  automation targets
- **`maui_frontend`** -- the XAML generation pipeline and the `makedefsdroid.vcxproj`
  template for a new page or control
