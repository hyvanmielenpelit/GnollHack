---
name: subagent_guidelines
description: Guidelines for using AI subagents and human pair programming in GnollHack implementation plans. Covers when to spawn subagents, which model to use (Flash, Pro, Opus), which tasks to assign to the human user, and planning constraints such as file-level exclusivity.
---

# Subagent and Pair Programming Guidelines

## Overview

GnollHack uses a **pair programming** model for AI-assisted development. The orchestrating AI agent works together with:
- **AI subagents** of varying capability for parallelizable tasks
- **The human user** for tasks where humans clearly outperform AI

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
| ... | Flash / Pro / Inherit | ... |

### Human Assignments
| Task | Rationale | Fallback if Not Approved |
|------|-----------|--------------------------|
| ... | ... | Orchestrator / Opus subagent |
```

When human tasks are listed, the plan MUST explicitly ask the user to approve or reject each human assignment. If the user rejects a human assignment, the orchestrator handles it directly or spawns a Claude Opus subagent (when the orchestrator is Opus).

## Model Selection Guide

### Flash (`flash`) — Mechanical Execution

Use for tasks that require **no judgment**, only mechanical application of a known pattern:
- Applying the same text replacement across multiple files
- Adding identical boilerplate entries to several files
- Reformatting code blocks to match a known style
- Inserting pre-written documentation blocks

**Key criterion**: The subagent does NOT need to decide *what* to change — only *where* and *how* to apply a specified change.

### Pro (`pro`) — Limited-Scope Reasoning

Use for tasks that require **some reasoning** but are **limited in scope**:
- Updating a struct field across C and C# interop boundaries (needs to understand marshalling)
- Adding a new entry to a pattern-based list where the entry requires adaptation (e.g., new monster, new sound assignment)
- Writing a small utility function following existing patterns
- Researching a specific subsystem to answer a targeted question

**Key criterion**: The subagent needs to *decide what to change* based on context, but the scope is confined to a small number of files or a single subsystem.

### Inherit (`inherit`) — Complex Reasoning (Opus-to-Opus)

Use **only when the orchestrator is Claude Opus** for tasks requiring deep reasoning:
- Multi-file refactors with non-obvious dependency chains
- Architectural changes spanning the C core and C# frontend
- Tasks requiring understanding of the full system (e.g., adding a new window proc callback end-to-end)
- Debugging complex cross-layer issues

**Key criterion**: The task requires the same level of reasoning capability as the orchestrator itself.

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

The following task types are **preferred for human execution** when the count is manageable (not too many instances). The human benefits from understanding the changes firsthand.

#### Cut-and-Paste (Move) Operations

AI agents struggle with move operations because they require coordinating a deletion in one location and an insertion in another, potentially across files. Humans can do this atomically in Visual Studio with `Ctrl+X` → `Ctrl+V`, with full visual context of the surrounding code.

**Assign to human when**: Code blocks need to be relocated (not copied), especially across files or within a large file where precise placement matters.

#### Simple Find-and-Replace via Visual Studio

For a small number of replacements (roughly ≤10 in a single file), Visual Studio's `Ctrl+H` is faster and more reliable than any AI approach. The human gets a confirmation dialog showing each match, catching unintended occurrences.

**Assign to human when**: A straightforward text substitution is needed in a small number of places within one file, and the pattern is unambiguous.

### Benefits of Human Involvement

- The human builds a **mental model** of what is changing and where
- In a codebase as interconnected as GnollHack (C core → native bridge → C# frontend), this understanding is valuable for future development
- Atomic operations (cut-paste) in an IDE are **more reliable** than multi-step AI tool calls
- Visual Studio's find-and-replace provides **confirmation UI** that prevents errors

### Fallback When Human Declines

If the user does not approve a human-assigned task:
1. The **orchestrator** handles it directly, OR
2. A **Claude Opus subagent** (`inherit` model) is spawned to handle it (when the orchestrator is Opus)

The plan should document which fallback applies for each human task.

## Example Subagent Use Section

```markdown
## Subagent Use

### Subagents Needed
Yes — the task involves changes across 8 files in 3 subsystems that can be parallelized.

### Subagent Assignments
| Task | Model | Files | Rationale |
|------|-------|-------|-----------|
| Add `FOO_BAR` define to all platform headers | Flash | `include/pcconf.h`, `include/unixconf.h`, `include/macconf.h` | Mechanical insertion of identical `#define` line |
| Update `struct layer_info` and marshalling | Pro | `include/layer.h`, `win/win32/xpl/libshare/GnollHackDefs.h` | Needs to understand C-to-C# struct alignment |
| Implement new window proc callback end-to-end | Inherit | `include/winprocs.h`, `src/windows.c`, `GnollHackService.cs` | Complex cross-layer change requiring deep system understanding |

### Human Assignments
| Task | Rationale | Fallback if Not Approved |
|------|-----------|--------------------------|
| Move `old_function()` from `src/foo.c` to `src/bar.c` | Cut-and-paste move — atomic in VS, error-prone for AI | Orchestrator handles directly |
| Replace `OLD_NAME` → `NEW_NAME` in `src/baz.c` (3 occurrences) | Simple find-replace, user benefits from seeing the locations | Orchestrator handles directly |

**User**: Please approve or reject each human assignment above.
```
