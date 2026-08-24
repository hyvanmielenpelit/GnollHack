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

## Where to Save Plans and Other Documents

All AI-produced documents — implementation plans, reviews, analyses (including
bug analyses), reports (including bug reports), and any other plans or structured
artifacts — are saved **inside the repository** under the `plans/` directory
(which is gitignored). The directory structure is:

```
plans/
  YYYY-MM-DD/
    task_name/
      implementation_plan_v<N>.md       ← N=1 for the first version
      code_review_v<N>.md               ← example: a review document
      bug_analysis_v<N>.md              ← example: an analysis document
      task.md                           ← single file, based on the final approved plan
      walkthrough.md                    ← single file, post-completion summary
```

### Directory Naming Rules

- **Date directory** (`YYYY-MM-DD`): Use the creation date of the **first plan**
  for the topic/project/task. If you create follow-up plans for the same task on
  later dates, they go into the **same date directory** as the original plan.
- **Task directory** (`task_name`): A short, descriptive `snake_case` name for
  the project or task (e.g., `game_page_update`, `add_new_monster_class`,
  `save_file_migration`).
- **Create subdirectories** as needed — they will not exist the first time.

### Document Versioning (STRICT)

This rule applies to **all document types** saved in `plans/`: implementation
plans, reviews, analyses (including bug analyses), reports (including bug
reports), and any other plans.

1. **First version**: Always append the `_v1` suffix to the filename.
   - Example: `implementation_plan_v1.md`, `bug_analysis_v1.md`
2. **Never overwrite an existing version.** When you read an existing document
   (e.g., `implementation_plan_v1.md`) and want to propose changes, create a
   **new file** with the next version number:
   - Read `implementation_plan_v1.md` → write `implementation_plan_v2.md`
   - Read `implementation_plan_v2.md` → write `implementation_plan_v3.md`
3. **Determine the next version number** by checking which version files already
   exist in the task directory. The new file gets the highest existing number
   plus one.
4. **Do not delete or modify** older versions. They form a revision history.

> [!IMPORTANT]
> Different AIs (or the same AI in different sessions) may propose very different
> revisions. Preserving every version ensures the user can compare approaches and
> revert to any prior version.

**Exception — `task.md` and `walkthrough.md`**: These are **singular files**
(no version suffix). There is only one `task.md` and one `walkthrough.md` per
task, and they are based on whichever plan version was ultimately approved and
implemented.

### Example

A task started on 2026-08-24 to update the game page UI:

```
plans/
  2026-08-24/
    game_page_update/
      implementation_plan_v1.md          ← first draft
```

The user requests changes; the AI (same or different) reads v1 and creates v2:

```
plans/
  2026-08-24/
    game_page_update/
      implementation_plan_v1.md          ← preserved, not overwritten
      implementation_plan_v2.md          ← revised plan
```

After v2 is approved and implemented:

```
plans/
  2026-08-24/
    game_page_update/
      implementation_plan_v1.md
      implementation_plan_v2.md
      task.md                            ← checklist based on v2
      walkthrough.md                     ← references v2 as the implemented plan
```

### Agent-Specific Notes

- **Claude Code**: Use your file-writing tool (e.g., `Write` / `create_file`) to
  create the document at `plans/YYYY-MM-DD/task_name/<document_name>_v<N>.md`
  (relative to the repo root, where N=1 for the first version). Create the
  intermediate directories if they do not exist. Tell the user the file path so
  they can open and review it. Before creating a revision, check which versions
  already exist in the directory.
- **Antigravity / other agents**: Same path convention. Use your file-writing
  tool to save the document. The `plans/` directory is gitignored, so documents
  will not be committed unless the user explicitly adds them.

### How to Write the File (IMPORTANT)

**Always use the agent's native file-writing tool** (e.g., `Write`,
`create_file`, `write_to_file`) to create documents. Pass the entire document
content as a tool parameter.

**Do NOT use shell commands** (`cat << EOF`, `echo`, `printf`, heredoc, etc.) to
write document files. Markdown documents contain backticks, dollar signs, angle
brackets, and other characters that cause shell quoting and escaping failures.
Shell-based file writing is fragile, token-wasteful (escaping overhead), and
produces corrupted output when it breaks.

The native file-writing tool bypasses the shell entirely — the content goes
directly from the tool parameter to the file with no quoting layer in between.

## Progress Tracking

After receiving user approval, create a **task checklist** (`task.md`) to track
implementation progress. There is only **one** `task.md` per task — it is based
on whichever plan version was approved.

Format:

```markdown
# Task Checklist

Based on: implementation_plan_v2.md

- [ ] Uncompleted task
- [/] In-progress task
- [x] Completed task
  - [x] Sub-task A
  - [ ] Sub-task B
```

Update the checklist as you work through each step.

## Walkthrough Document

After completing all work, create a **walkthrough document** (`walkthrough.md`)
summarizing:

- **Which plan version was implemented** (e.g., "Implemented
  `implementation_plan_v2.md`")
- What was changed (with file links)
- What was tested
- Validation results (build output, test results)
- Any remaining follow-up items

There is only **one** `walkthrough.md` per task. If this is follow-up work to an
earlier task, update the existing walkthrough rather than creating a new one.

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
