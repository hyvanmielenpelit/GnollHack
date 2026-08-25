@../.agents/AGENTS.md

## Claude Code

This project keeps its AI configuration in the tool-neutral `.agents/` directory so
that agents other than Claude can be pointed at the same files. `.claude/` is only a
thin adapter layer over it.

- **Project rules**: `.agents/AGENTS.md`, imported at the top of this file. Edit the
  rules there, not here.
- **Skills**: the full bodies live in `.agents/skills/<name>/SKILL.md`. Each one has a
  matching pointer stub in `.claude/skills/`, which exists purely so Claude Code
  discovers and triggers them.
- **Naming**: canonical folders use underscores (`build_pipeline`); the invocable skill
  name is the hyphenated form (`build-pipeline`), as the skill spec requires. Skill
  bodies cross-reference each other by the underscore folder name — that is correct and
  refers to the canonical file.

<!-- Maintainer note: a skill's `description` is duplicated into its stub, because the
     description is what Claude Code indexes for triggering. If you change a description
     in .agents/skills/, mirror it into the matching .claude/skills/ stub or the skill
     will trigger on stale wording. Bodies and references are never duplicated.
     Regenerate all stubs from the canonical files rather than editing them by hand. -->

## Implementation Planning Workflow (MANDATORY)

**Non-trivial tasks require a written implementation plan, approved by the user
before any source file is modified.** Read the `implementation-planning` skill
(`.agents/skills/implementation_planning/SKILL.md`) for the full specification.
The summary below is binding regardless of whether the skill is triggered.

### When to Plan

A plan is **required** when any of these are true:

- The task touches **more than one file**, or more than one subsystem
- It crosses a **build regeneration boundary** (`makedefs`, `levcomp`, `dgncomp`,
  `dlb`, `makedefsdroid`)
- It adds or changes a **C-to-C# interop** struct, enum, or callback
- It is a refactor, a new feature, or anything the user describes as non-trivial

A plan is **not** required for single-file fixes, typos, comment edits, answering
questions, or read-only investigation.

### Lifecycle (Five Phases)

1. **Research** — read files, search code, understand implications. **No edits.**
2. **Write the plan** — save it as a Markdown file **inside the repository**
   under the gitignored `.plans/` directory (see "Saving Plans and Other
   Documents" below). Tell the user the file path so they can review it. Under
   plan mode, write it to the harness plan file and copy it to `.plans/` **before**
   requesting approval — see "Claude Code Plan Mode" below.
3. **Wait for approval** — **STOP.** Do not edit any source file until the user
   explicitly approves the plan. Request approval via `ExitPlanMode` when plan
   mode is active; otherwise print a brief summary and wait.
4. **Execute** — implement the approved plan step by step. If significant
   deviation is needed, stop, update the plan, and re-confirm.
5. **Verify** — build, test, confirm correctness. Summarize results.

### Plan Document Format

See the `implementation-planning` skill for the full template. The plan **must**
include at minimum:

- **Goal** — what and why
- **Affected Files** — table of every file touched
- **Build Impact** — which build utilities must be re-run, or "None"
- **Proposed Changes** — grouped by component, ordered by dependency
- **Subagent Use** — mandatory even if "No" (see `subagent-guidelines` skill)
- **Risks** — what could break
- **Verification Plan** — how correctness will be confirmed

### Saving Plans and Other Documents

All AI-produced documents — implementation plans, reviews, analyses, reports, and
other structured artifacts — are saved **inside the repository** under the
gitignored `.plans/` directory, using this structure:

```
.plans/YYYY-MM-DD/task_name/<document_name>_v<N>.md    ← N=1 for the first version
```

- **Date directory** (`YYYY-MM-DD`): the creation date of the **first document**
  for the task. Follow-up documents for the same task reuse the same date
  directory.
- **Task directory**: a short, descriptive `snake_case` name (e.g.,
  `game_page_update`).
- **Create subdirectories** if they do not exist.
- **Conflict resolution**: if a task directory with the desired name already
  exists under the same date, find the next free name in the sequence
  `task_name`, `task_name_2`, `task_name_3`, ... — never rename the existing
  folder, and never nest suffixes (e.g., `_2_3` is wrong). Use the new
  directory for all work — do not touch the conflicting directory.

### Document Versioning (STRICT)

This applies to **all** documents in `.plans/` — implementation plans, reviews,
analyses, reports, and other plans:

- **First version** always gets a `_v1` suffix (e.g., `implementation_plan_v1.md`,
  `bug_analysis_v1.md`, `code_review_v1.md`).
- **Never overwrite** an existing version. To revise, create a new file with the
  next version number (read `_v1` → write `_v2`, read `_v2` → write `_v3`).
- Check which versions already exist before creating a revision.
- `task.md` and `walkthrough.md` are **singular** (no version suffix) — based on
  whichever plan version was ultimately approved. The walkthrough must state
  which plan version was implemented. **Follow-up rounds** use lettered variants
  (`task_A.md`, `walkthrough_A.md`, etc.) — see the `implementation-planning`
  skill for the full specification.

To deliver a document:

1. **Use your native file-writing tool** (e.g., `Write` / `create_file`) to
   create the document at `.plans/YYYY-MM-DD/task_name/<document_name>_v<N>.md`
   (N=1 for new documents; check existing versions and increment for revisions).
   **Do NOT use shell commands** (`cat << EOF`, heredoc, `echo`, etc.) — Markdown
   content contains backticks, dollar signs, and angle brackets that cause shell
   quoting failures and corrupted output.
2. Print the file path in chat so the user can open and review it.
3. Print a **brief summary** (not the full document) directing the user to the file.
4. For implementation plans, wait for the user's approval before proceeding to
   execution.

### Claude Code Plan Mode

Plan mode restricts editing to its own plan file (`~/.claude/plans/<slug>.md`),
which conflicts with the `.plans/` rule above. **The harness wins.** Resolve it
like this:

1. **Write** the plan to the harness plan file. In-place editing of that file is
   expected and does not violate the `_v<N>` rule — versioning binds to the
   `.plans/` copy only.
2. **Copy** the finished plan to
   `.plans/YYYY-MM-DD/task_name/implementation_plan_v1.md` — **before** asking for
   approval, not after. `.plans/` is the source of truth: other AIs read revisions
   from there and never look in `~/.claude/`, and the document then survives a
   rejection or a lost session. Creating the task directory is part of this step.
3. **Print** both paths in chat, plus a brief summary.
4. **Request approval** with the `ExitPlanMode` tool. Do not ask "is this plan
   okay?" in chat text — that is what the tool is for.
5. **On approval**, create `task.md`, execute, and finish with `walkthrough.md`.

> **Why step 2 is allowed during plan mode**: plan mode's restriction exists to
> keep the agent from changing the project before the user approves the work.
> Copying the plan document to its canonical location is a copy of the planning
> artifact — it touches no source file, build file, or game data. Everything that
> would actually change the project still waits for approval.

**Subagents**: plan mode prescribes `Explore` agents for research and a `Plan`
agent for design. Follow the harness on whether and when to spawn them. The
`subagent-guidelines` skill governs only *how* — agent type, model tier, and
file-level exclusivity — and its mandatory **Subagent Use** plan section covers
execution-phase agents, not read-only planning research.

**Format**: the full GnollHack plan format applies to non-trivial work. Plan
mode's "concise enough to scan quickly" means *no excessive verbosity within that
format*, not a smaller format. For truly trivial work, the harness's concise form
(or no plan at all) is acceptable — say in chat that the shortcut was taken
because the task was trivial, and produce the full format as the next revision if
the user asks.
