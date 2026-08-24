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
