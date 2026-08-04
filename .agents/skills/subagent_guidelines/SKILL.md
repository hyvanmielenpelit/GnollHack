---
name: subagent_guidelines
description: Guidelines for using AI subagents and human pair programming in GnollHack implementation plans. Covers when to spawn subagents, model selection defaults, and planning constraints such as file-level exclusivity.
---

# Subagent and Pair Programming Guidelines

## Overview

GnollHack uses a **pair programming** model for AI-assisted development. The orchestrating AI agent works together with:
- **AI subagents** for parallelizable tasks
- **The human user** for rare tasks where humans clearly outperform AI

Every implementation plan MUST include a **Subagent Use** section, even when no subagents are needed (document why).

## Implementation Plan Requirement

Every implementation plan must contain the following section:

```markdown
## Subagent Use

### Subagents Needed
[Yes / No — if no, explain why (e.g., "single-file change, not worth the overhead")]

### Subagent Assignments
| Task | Model | Rationale |
|------|-------|-----------|
| ... | Inherit / Flash | ... |

### Human Assignments (if any)
| Task | Rationale | Fallback if Not Approved |
|------|-----------|--------------------------|
| ... | ... | Orchestrator handles directly |
```

When human tasks are listed, the plan MUST explicitly ask the user to approve or reject each human assignment. If the user rejects a human assignment, the orchestrator handles it directly.

## Model Selection Guide

### Default: Inherit (`inherit`) — Match the Orchestrator

**Use for 99% of all subagent tasks.** The `inherit` model matches the orchestrator's own model (e.g., Claude Opus spawns Claude Opus subagents, Gemini 3.1 Pro spawns Gemini 3.1 Pro subagents). This ensures subagents have the same reasoning capability as the orchestrator.

Suitable for virtually all tasks including:
- Multi-file changes with reasoning required
- Implementing new features following existing patterns
- Updating structs across C and C# interop boundaries
- Writing utility functions, tool handlers, or service classes
- Researching subsystems, reviewing code, answering questions
- Debugging, refactoring, and architectural changes

**Key principle**: If a task is worth spawning a subagent for, it's worth giving it the orchestrator's full capability.

### Exception: Flash (`flash`) — Extremely Mundane Mechanical Tasks

Use **only** for tasks that are trivially mechanical search-and-replace with **zero judgment** required:
- Applying an identical, pre-specified text replacement across many files
- Inserting the exact same boilerplate line into multiple files

**Key criterion**: The subagent does NOT need to decide *what* to change — only *where* to paste an already-specified string. If there is any ambiguity, adaptation, or context-sensitivity, use `inherit` instead.

> [!IMPORTANT]
> Flash is the rare exception, not the default. When in doubt, use `inherit`.

## Planning Constraints

### File-Level Exclusivity (STRICT)

**No two agents (including the orchestrator) may edit the same file concurrently.** This is a hard planning constraint:
- When decomposing tasks for parallel subagents, assign each agent a **non-overlapping set of files**
- If two tasks touch the same file, they must be **sequenced**, not parallelized
- The orchestrator must not edit a file that a subagent is also editing

### Build Dependency Chains

Some GnollHack tasks are inherently sequential due to build dependencies. The plan must identify these chains and not parallelize across them:
- Modifying `monst.c` or `objects.c` → rebuild `makedefs` → regenerated headers (`pm.h`, `onames.h`) → then update C# interop
- Modifying `.des` files → rebuild `levcomp` → regenerated `.lev` files → rebuild `dlb`/`nhdat`
- Modifying XAML in `GnollHackX` → `makedefsdroid` regeneration → then update GnollHackM code-behind

### Communication Overhead

For tasks that take under 30 seconds to do directly, spawning a subagent is slower due to setup and message-passing latency. Prefer doing these yourself.

### Protecting Uncommitted Changes (STRICT)

**No agent (orchestrator or subagent) may overwrite uncommitted changes in a file without explicit user permission.** This includes restoring a file to an earlier repository version, regenerating file contents from scratch, or any operation that would discard prior edits.

Before editing a file, agents should consider whether uncommitted changes (from the user or from previous agent work) exist in that file. If the planned work has a risk of corrupting or losing those changes:
1. The **subagent** must report the risk to the **orchestrator**
2. The **orchestrator** must ask the user to either:
   - **Commit the changes first** (preferred), so they can be recovered if needed, OR
   - **Explicitly approve** the work with the understanding that uncommitted changes may be lost
3. Only after receiving user approval may the agent proceed

This applies equally to user-made and agent-made uncommitted changes, since the two cannot always be distinguished.

## Human Pair Programming Tasks

### When to Assign to the Human

Assigning tasks to the human user is the **rare exception**, not the norm. Only consider it for:

#### Very Extensive Cut-and-Paste (Move) Operations

AI agents struggle with large move operations because they require coordinating a deletion in one location and an insertion in another, potentially across files. When the code block is very large, the AI is likely to get it wrong and waste a significant number of tokens retrying. Humans can do this atomically in Visual Studio with `Ctrl+X` → `Ctrl+V`.

**Assign to human when**: A large code block (50+ lines) needs to be relocated (not copied), especially across files, AND the AI would likely fail and waste tokens on retries.

> [!IMPORTANT]
> For small moves, simple find-and-replace, or any task where the AI can handle it reliably, the orchestrator or a subagent should do the work — not the human. The threshold for human assignment is high: the task must be one where AI failure is likely and the token cost of retries would be substantial.

### Fallback When Human Declines

If the user does not approve a human-assigned task, the orchestrator handles it directly.

## Example Subagent Use Section

```markdown
## Subagent Use

### Subagents Needed
Yes — the task involves changes across 8 files in 3 subsystems that can be parallelized.

### Subagent Assignments
| Task | Model | Files | Rationale |
|------|-------|-------|-----------|
| Add `FOO_BAR` define to all platform headers | Flash | `include/pcconf.h`, `include/unixconf.h`, `include/macconf.h` | Trivially mechanical insertion of identical `#define` line |
| Update `struct layer_info` and marshalling | Inherit | `include/layer.h`, `win/win32/xpl/libshare/GnollHackDefs.h` | Needs to understand C-to-C# struct alignment |
| Implement new window proc callback end-to-end | Inherit | `include/winprocs.h`, `src/windows.c`, `GnollHackService.cs` | Complex cross-layer change requiring deep system understanding |
```
