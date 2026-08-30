@../.agents/AGENTS.md

## Claude Code

This project keeps its AI configuration in the tool-neutral `.agents/` directory so
that agents other than Claude can be pointed at the same files. `.claude/` is only a
thin adapter layer over it.

- **Project rules**: `.agents/AGENTS.md`, imported at the top of this file. Edit the
  rules there, not here.
- **Skills**: the full bodies live in `.agents/skills/<name>/SKILL.md`, each with a
  matching pointer stub in `.claude/skills/`. The stub contract, and why stubs are
  regenerated rather than hand-edited, are in the global `claude-code-conventions` skill.
- **Naming**: canonical folders use underscores (`build_pipeline`); the invocable skill
  name is the hyphenated form (`build-pipeline`). Skill bodies cross-reference each other
  by the underscore folder name — that is correct and refers to the canonical file.
- **The `client_` prefix**: skill names share one flat global namespace across projects,
  user-level skills, and plugins. A generic `implementation-planning` would collide with
  the MobileGnollHackLogger repository's equivalent, and the loser of such a collision is
  silently never loaded. New skills here take `client_`; that repository uses `server_`.
- **Global skills** are installed at the user level from `hyvanmielenpelit/SharedAgentSkills`
  via its `setup.ps1`, and have no in-repository bodies:
  `agent-implementation-planning`, `agent-subagent-guidelines`,
  `agent-powershell-guidelines`, plus the Claude-only `claude-plan-mode` and
  `claude-code-conventions`. See that repository's `docs/ai-skill-management.md`.

## Implementation Planning Workflow (MANDATORY)

**Non-trivial tasks require a written implementation plan, approved by the user
before any source file is modified.**

- **This repository's** subsystems, build boundaries, and plan triggers are in the
  `client-implementation-planning` skill.
- **The lifecycle, plan format, plans repository layout, versioning, the commit
  protocol, and the `.plans/` fallback** are in the global
  `agent-implementation-planning` skill. Plans go to
  `<plans-root>/hyvanmielenpelit/GnollHack/YYYY-MM-DD/task_name/`, not into this
  repository.
- **Claude Code plan mode** — the harness plan file, copying to the plans repository
  before `ExitPlanMode`, reaching it via `permissions.additionalDirectories` in
  `.claude/settings.json`, and the `Explore` / `Plan` research agents — is in the global
  `claude-plan-mode` skill.
- **Never commit or push in this repository.** The shared `plans` repository is the only
  one an agent may commit to.

Read those rather than relying on a summary here. `.agents/AGENTS.md`, imported at the
top of this file, carries the GnollHack-specific triggers for when a plan is required.
