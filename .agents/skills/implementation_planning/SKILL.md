---
name: implementation_planning
description: >-
  Full workflow for writing, delivering, and executing implementation plans for
  non-trivial GnollHack tasks. Covers when a plan is required, the mandatory
  research-plan-approve-execute-verify lifecycle, plan document structure,
  artifact delivery, and progress tracking. Read this skill before starting any
  multi-file or cross-subsystem change.
---

# Implementation Planning Workflow

## Purpose

This skill defines the **mandatory planning workflow** for non-trivial GnollHack
changes. It ensures that complex work is researched, documented, user-approved,
and verified — preventing wasted effort from wrong-direction changes.

## When a Plan Is Required

A written implementation plan is **required** when the task meets **any** of
these criteria:

- It touches **more than one file**, or more than one subsystem (C core, native
  bridge, MAUI frontend, build utilities, game data)
- It crosses a **build regeneration boundary** (`makedefs`, `levcomp`,
  `dgncomp`, `dlb`, `makedefsdroid`)
- It adds or changes a **C-to-C# interop** struct, enum, or callback
- It is a refactor, a new feature, or anything the user describes as large or
  non-trivial

A plan is **not** required for:

- Single-file bug fixes
- Typo and comment corrections
- Answering questions or read-only investigation
- Minor follow-ups to an already-approved plan

**When in doubt, write a plan.** A rejected plan is cheap; a wrong cross-layer
change is not.

## The Five-Phase Lifecycle

Every planned task follows these phases in strict order:

### Phase 1 — Research

- Use search and file-reading tools to thoroughly understand the affected code,
  dependencies, architecture, and implications.
- **Do NOT modify any source files during this phase.** Read-only operations
  only.
- Take notes on what you find. These notes become input to the plan.

### Phase 2 — Write the Implementation Plan

- Create the plan as a **Markdown file** saved outside the repository. See
  "Plan Document Structure" and "Where to Save the Plan" below.
- Include any open questions or design decisions that the user needs to weigh in
  on — put them directly in the plan document.
- Present the plan to the user for review.

### Phase 3 — Obtain User Approval

- **STOP and wait for the user's explicit approval before editing any source
  file.** This is a hard gate.
- Do not begin implementation alongside the plan, do not "get a head start", and
  do not pre-create files "to save time."
- If the user requests changes to the plan, revise and re-present it.

### Phase 4 — Execute

- Once approved, implement the plan step by step.
- Track progress using a task checklist (see "Progress Tracking" below).
- **If you discover issues that require significant deviation from the approved
  plan**, stop and update the plan document. Present the revised plan for
  re-approval before continuing.
- Do not silently diverge from what the user approved.

### Phase 5 — Verify

- Verify that your changes have the desired effects:
  - Run any applicable build commands
  - Run unit tests if they exist
  - Confirm code compiles without errors
- Create or update a **walkthrough document** summarizing what you changed, what
  was tested, and the results.

## Plan Document Structure

Use this Markdown template. Omit sections that are not relevant (e.g., "Build
Impact" if the change is C#-only), but always include the sections marked
**mandatory**.

```markdown
# [Goal Description]

Brief description of the problem, background context, and what the change
accomplishes.

## User Review Required                          ← include if applicable

Document anything that requires user review: breaking changes, significant
design decisions, trade-offs. Use alert callouts for critical items:

> [!WARNING]
> This change modifies the save file format and breaks backward compatibility.

## Open Questions                                ← include if applicable

Clarifying or design questions for the user that affect the plan.

## Affected Files                                ← mandatory

| File | Subsystem | Change |
|------|-----------|--------|
| `src/foo.c` | C core | Add new function `bar()` |
| `include/foo.h` | C headers | Add prototype for `bar()` |

## Build Impact                                  ← mandatory for C/data changes

Regeneration steps triggered: makedefs / levcomp / dlb / makedefsdroid, or
"None".

## Proposed Changes                              ← mandatory

Group files by component and order by dependency (build dependencies first).

### [Component Name]

Summary of changes in this component.

#### [MODIFY] `filename.c`

What changes in this file and why.

#### [NEW] `filename.c`

What this new file contains and why it is needed.

#### [DELETE] `filename.c`

Why this file is being removed.

## Subagent Use                                  ← mandatory

### Subagents Needed
[Yes / No — if no, explain why]

### Subagent Assignments
| Task | Model | Files | Rationale |
|------|-------|-------|-----------|

### Human Assignments (if any)
| Task | Rationale | Fallback if Not Approved |
|------|-----------|--------------------------|

## Risks                                         ← mandatory

What could break, and how it would be noticed.

## Verification Plan                             ← mandatory

### Automated Tests
- Commands to run.

### Manual Verification
- Steps the user should take (e.g., build and run on device).
```

### Key Structural Rules

1. **Affected Files table** — every file the plan touches must be listed.
2. **Build Impact** — explicitly state which build utilities must be re-run, or
   "None". See the `build_pipeline` skill for the full pipeline reference.
3. **Subagent Use** — mandatory even when no subagents are needed (state "No"
   and explain why). See the `subagent_guidelines` skill for the full
   specification.
4. **Risks** — do not skip this. Even "low risk" changes should state what to
   watch for.
5. **Steps / Proposed Changes** — order by dependency. Build regeneration
   boundaries must fall **between** steps, never inside one.

## Where to Save the Plan

- **Save the plan as a Markdown file outside the repository.** Do not save it
  into the GnollHack source tree.
- Preferred locations, depending on your agent harness:
  - **Claude Code**: Save to `$TMPDIR` or a user-visible scratch directory. If
    your harness supports artifacts, create it as an artifact file. The key
    requirement is that the user can read and review the document.
  - **Antigravity / other agents**: Use the agent's dedicated scratch or
    artifact directory.
- **Do not commit the plan** to git unless the user explicitly asks you to.

### Claude Code–Specific Guidance

Claude Code does not have a built-in "artifact" system, but you can achieve the
same effect:

1. **Write the plan to a file** using `write_to_file` or your file creation
   tool. Save it to a location outside the repo, for example:
   - `$TMPDIR/implementation_plan.md`
   - Or a dedicated planning directory the user has configured
2. **Tell the user the file path** so they can open and review it.
3. **Print a short summary** in chat (not the full plan) directing the user to
   the file.
4. After the user approves, follow the same execute → verify lifecycle.

## Progress Tracking

After receiving user approval, create a **task checklist** to track
implementation progress. This can be a separate file or appended to the plan.

Format:

```markdown
# Task Checklist

- [ ] Uncompleted task
- [/] In-progress task
- [x] Completed task
  - [x] Sub-task A
  - [ ] Sub-task B
```

Update the checklist as you work through each step.

## Walkthrough Document

After completing all work, create a **walkthrough document** summarizing:

- What was changed (with file links)
- What was tested
- Validation results (build output, test results)
- Any remaining follow-up items

If this is follow-up work to an earlier task, update the existing walkthrough
rather than creating a new one.

## Quick Decision Guide

```
Is the task trivial (single file, typo, comment, question)?
  → YES: Skip the plan. Just do it.
  → NO: Continue ↓

Does it touch multiple files, cross subsystem boundaries, or involve interop?
  → YES: Write a full plan. Follow the five-phase lifecycle.
  → NO: Use judgment. When in doubt, write the plan.

Is it a minor follow-up to an already-approved plan?
  → YES: Skip a new plan. Continue executing the existing one.
  → NO: Write a new plan.
```
