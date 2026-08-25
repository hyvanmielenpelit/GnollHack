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

- Create the plan as a **Markdown file** saved inside the repository under
  `.plans/` (which is gitignored). See "Plan Document Structure" and "Where to
  Save Plans and Other Documents" below.
- Include any open questions or design decisions that the user needs to weigh in
  on — put them directly in the plan document.
- Present the plan to the user for review.
- If the harness confines you to its own plan file while planning, write the plan
  there and then **copy it to `.plans/` before requesting approval** — see
  "Harness Rules Take Precedence" below.

### Phase 3 — Obtain User Approval

- **STOP and wait for the user's explicit approval before editing any source
  file.** This is a hard gate.
- **Always print the plan's file path in chat** so the user can open it.
- **How to request approval**: if the harness provides an explicit approval
  mechanism (e.g., Claude Code `ExitPlanMode`), use it. Otherwise, print a brief
  summary of the plan — not the full document — and wait. **Approval is never
  skipped.**
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

### Scaling the Format

The full format applies to **non-trivial** work. If a harness asks for "concise"
plans, that means *no excessive verbosity within this format* — keep every
mandatory section but keep each one tight. Never trim the Affected Files table.

For **truly trivial** work, the harness's own concise format (or no plan at all)
is acceptable. State in chat that the shortcut was taken because the task was
trivial. If the user then asks for the full format, produce it as the next
revision (`_v<N+1>`).

## Where to Save Plans and Other Documents

All AI-produced documents — implementation plans, reviews, analyses (including
bug analyses), reports (including bug reports), and any other plans or structured
artifacts — are saved **inside the repository** under the `.plans/` directory
(which is gitignored). The directory structure is:

```
.plans/
  YYYY-MM-DD/
    task_name/
      implementation_plan_v<N>.md       ← N=1 for the first version
      code_review_v<N>.md               ← example: a review document
      bug_analysis_v<N>.md              ← example: an analysis document
      task.md                           ← single file, based on the final approved plan
      walkthrough.md                    ← single file, post-completion summary
      implementation_review_A_v<N>.md   ← follow-up round A (see "Follow-Up Rounds")
      task_A.md                         ← follow-up A task checklist
      walkthrough_A.md                  ← follow-up A walkthrough
```

### Directory Naming Rules

- **Date directory** (`YYYY-MM-DD`): Use the creation date of the **first plan**
  for the topic/project/task. If you create follow-up plans for the same task on
  later dates, they go into the **same date directory** as the original plan.
- **Task directory** (`task_name`): A short, descriptive `snake_case` name for
  the project or task (e.g., `game_page_update`, `add_new_monster_class`,
  `save_file_migration`).
- **Create subdirectories** as needed — they will not exist the first time.
- **Task directory conflict resolution**: Every new task (not a follow-up round)
  gets its **own new folder**. Before creating a folder, check whether a folder
  with the desired name already exists under the same date directory. If it
  does, find the next free name in the sequence `task_name`, `task_name_2`,
  `task_name_3`, ... — always use the **base name** (without any numeric
  suffix) as the root of the sequence, and increment the number until a free
  name is found. **Never** append a suffix to an already-suffixed name (e.g.,
  `task_name_2_3` is wrong — the correct next name after `task_name_2` is
  `task_name_3`). The existing folder is never renamed. Use the newly created
  directory for all work on the new task — do not read, modify, or otherwise
  interact with the conflicting directory.

> [!IMPORTANT]
> **Three distinct suffix types — do not confuse them:**
>
> | Suffix | Applies to | Meaning | Example |
> |--------|-----------|---------|---------|
> | `_2`, `_3`, ... | **Folder** names | Conflict resolution for separate tasks with the same name | `game_page_update_2/` |
> | `_A`, `_B`, ... | **File** names | Follow-up round within the same task folder | `implementation_review_A_v1.md` |
> | `_v1`, `_v2`, ... | **File** names | Document revision (never overwrite, always increment) | `implementation_plan_v2.md` |

### Document Versioning (STRICT)

This rule applies to **all document types** saved in `.plans/`.

1. **First version**: Always use the `_v1` suffix.
2. **Never overwrite an existing version.** To revise, create a **new file**
   with the next version number (read `_v1` → write `_v2`).
3. **Determine the next version** by checking which files already exist. The new
   file gets the highest existing number plus one.
4. **Do not delete or modify** older versions. They form a revision history that
   lets the user compare approaches across different AIs or sessions.

**Exception — `task.md` and `walkthrough.md`**: These are **singular files**
(no version suffix). There is only one `task.md` and one `walkthrough.md` per
task, and they are based on whichever plan version was ultimately approved and
implemented. **Follow-up rounds** get their own lettered variants (`task_A.md`,
`walkthrough_A.md`, etc.) — see "Follow-Up Rounds" below.

### Example

A task started on 2026-08-24 to update the game page UI. The plan goes through
revision and then a follow-up review round:

```
.plans/
  2026-08-24/
    game_page_update/
      implementation_plan_v1.md          ← first draft (preserved)
      implementation_plan_v2.md          ← revised after user feedback
      task.md                            ← checklist based on v2
      walkthrough.md                     ← references v2 as the implemented plan
      implementation_review_A_v1.md      ← follow-up A: review of the implementation
      task_A.md                          ← follow-up A: task checklist
      walkthrough_A.md                   ← follow-up A: walkthrough
```

### How to Write Files

**Always use the agent's native file-writing tool** (e.g., `Write`,
`create_file`, `write_to_file`). **Do NOT use shell commands** (`cat << EOF`,
`echo`, heredoc, etc.) — Markdown content contains backticks, dollar signs, and
angle brackets that cause shell quoting failures and corrupted output.

## Harness Rules Take Precedence

Agent harnesses impose their own planning workflows, and some restrict where you
may write. **The harness rules always win.** This skill is guidance layered
*inside* whatever the harness permits — it tells you how to write a good plan,
what it must contain, and where the canonical copy belongs. It never overrides a
harness restriction.

### `.plans/` Is the Source of Truth

A harness may keep its own private plan file (Claude Code plan mode writes to
`~/.claude/plans/<slug>.md`; Antigravity uses an artifact directory). Treat that
file as a **working/backup copy**. The canonical document is always the one in
`.plans/YYYY-MM-DD/task_name/`.

This matters because agents hand work to each other. A different AI picking up
the task reads the **latest `_v<N>` from `.plans/`** and writes its next revision
**to `.plans/`** — it never looks inside a harness-private directory.

### When to Make the `.plans/` Copy

**As soon as the plan is finished, and immediately before requesting approval.**
The copy is part of delivering the plan, not part of executing it.

Order of operations: finish writing the plan → **copy to `.plans/`** → print
the path → request approval via the harness mechanism (or a chat summary).

> [!NOTE]
> **This copy does not violate a harness "no other file edits" restriction.**
> Such restrictions prevent the agent from changing the *project* before approval.
> Copying the plan to its canonical location touches no source file, build file,
> or game data. Make the copy; do not defer it to execution.

Everything else still waits for approval: `task.md`, source edits, build steps.

### Versioning Is Per-Location

| Location | Naming | Revising |
|----------|--------|----------|
| Harness-private plan file | Whatever the harness assigns | Edit **in place** |
| `.plans/` | `<document_name>_v<N>.md` | **Never overwrite** — increment `_v<N>` |

### Agent-Specific Notes

- **Claude Code**: Create the document at
  `.plans/YYYY-MM-DD/task_name/<document_name>_v<N>.md` (relative to the repo
  root). Create intermediate directories if they do not exist. Print the path so
  the user can review it. Before creating a revision, check which versions
  already exist.
- **Antigravity**: First create the artifact in the artifact directory (so the UI
  can present it), then also copy it to `.plans/`. Both locations must receive
  the file.
- **Other agents**: Same path convention. The `.plans/` directory is gitignored,
  so documents will not be committed unless the user explicitly adds them.

## Progress Tracking

After receiving user approval, create a **task checklist** (`task.md`) to track
implementation progress. There is only **one** `task.md` per task — it is based
on whichever plan version was approved.

For **follow-up rounds**, create a lettered task checklist (`task_A.md`,
`task_B.md`, etc.) — see "Follow-Up Rounds" below.

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

There is only **one** `walkthrough.md` per task's original implementation. For
**follow-up rounds**, create a lettered walkthrough (`walkthrough_A.md`,
`walkthrough_B.md`, etc.) — see "Follow-Up Rounds" below.

## Follow-Up Rounds

After an implementation plan has been executed and its walkthrough produced, the
task may require **follow-up work** — reviews, corrections, further analyses, or
supplementary changes that relate to the original task. These are tracked as
**follow-up rounds** within the same task directory.

### Round Identifiers

Each follow-up round is identified by a **letter suffix**: `_A`, `_B`, `_C`,
etc. The letter is assigned sequentially in the order follow-up rounds are
created, regardless of the document type within the round. The letter groups all
documents belonging to the same follow-up round.

### Naming Convention

Every document in a follow-up round embeds the round letter between the document
name and the version suffix:

```
<document_name>_<round>_v<N>.md
```

| File Type | Original Plan | Follow-Up Round A | Follow-Up Round B |
|-----------|--------------|-------------------|-------------------|
| Plan / review / analysis | `implementation_plan_v1.md` | `implementation_review_A_v1.md` | `performance_analysis_B_v1.md` |
| Task checklist | `task.md` | `task_A.md` | `task_B.md` |
| Walkthrough | `walkthrough.md` | `walkthrough_A.md` | `walkthrough_B.md` |

- The **document name** describes the content (e.g., `implementation_review`,
  `correction_plan`, `performance_analysis`). Use `snake_case`.
- The **round letter** (`_A`, `_B`, ...) identifies which follow-up round this
  document belongs to.
- The **version suffix** (`_v1`, `_v2`, ...) tracks revisions within the round,
  following the same strict versioning rules as original plan documents: first
  version is always `_v1`, never overwrite, increment to revise.
- Task checklists (`task_A.md`) and walkthroughs (`walkthrough_A.md`) are
  **singular per round** — no version suffix, just like the originals.

### Lifecycle

Each follow-up round follows the **same five-phase lifecycle** as the original
plan (Research → Plan → Approve → Execute → Verify). The follow-up plan
document (e.g., `implementation_review_A_v1.md`) takes the role of the
implementation plan and requires user approval before execution begins.

### When to Use a Follow-Up Round vs. a New Task

Use a **follow-up round** when:

- The work directly relates to the original task (e.g., reviewing how the
  implementation went, fixing issues discovered during verification, adding
  something that was deferred from the original plan)
- The same task directory and date are appropriate

Create a **new task** (new date/task directory) when:

- The work is substantially independent, even if it touches the same files
- Enough time has passed that it is a distinct effort
- The scope has grown beyond what the original task covered

### Determining the Next Round Letter

Before creating a follow-up round, check which round letters already exist in
the task directory. The new round gets the next letter in sequence (`A`, `B`,
`C`, ...). If `_A` and `_B` exist, the next round is `_C`.

### Example

An original implementation is complete. The user asks for a review of the
implementation quality:

1. The agent creates `implementation_review_A_v1.md` — research and review
   findings.
2. The user approves the proposed corrections.
3. The agent creates `task_A.md` and begins execution.
4. After completion, the agent creates `walkthrough_A.md`.

Later, the user requests a performance analysis of the same feature:

1. The agent creates `performance_analysis_B_v1.md`.
2. The user requests changes; the agent creates `performance_analysis_B_v2.md`.
3. After approval, `task_B.md` and then `walkthrough_B.md`.

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
