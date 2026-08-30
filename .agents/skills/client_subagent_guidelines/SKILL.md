---
name: client_subagent_guidelines
description: >-
  Subagent constraints specific to the GnollHack client repository. Covers the three build
  dependency chains that must not be parallelized across -- makedefs to generated headers
  to C# interop, des files to levcomp to dlb, and GnollHackX XAML to makedefsdroid to
  GnollHackM -- and why cross-layer interop work in this repository escalates to the deep
  tier far more often than elsewhere. Read when writing a plan's Subagent Use section for
  GnollHack.
---

# GnollHack Subagent Guidelines (Project Overlay)

> **If the shared skills are not installed**, this is the whole baseline:
> no two agents may edit the same file concurrently; do not parallelize across the build
> chains below; never overwrite uncommitted changes without asking the user to commit
> first; every plan must contain a **Subagent Use** section even when the answer is "no".
> Install the full guidance from
> `https://github.com/hyvanmielenpelit/SharedAgentSkills` (`.\setup.ps1`).

> [!IMPORTANT]
> **Global baseline delegation.** The mandatory **Subagent Use** plan section, the model
> tiers and the rule for choosing between them, file-level exclusivity, `.plans/`
> isolation for subagents, communication overhead, protecting uncommitted changes, and the
> human pair-programming threshold are all in the global **`agent-subagent-guidelines`**
> skill.
>
> How to resolve a tier against the models your session actually offers, and the boundary
> that an application can spawn only its own vendor's models, are in
> `claude-code-conventions` or `gemini-antigravity-conventions`, whichever is installed.
>
> The sections below are what is specific to **this repository**.

---

## Build Dependency Chains

These are the reason this overlay exists. Each is inherently sequential: one step's output
is the next step's input. **A plan must not parallelize across a chain**, and a
regeneration boundary falls *between* plan steps, never inside one.

| Chain | Constraint |
|-------|-----------|
| `src/monst.c` / `src/objects.c` -> rebuild `makedefs` -> regenerated `include/pm.h`, `include/onames.h` -> C# interop | The generated headers must exist before any agent edits code that references the new constants. No agent may hand-edit the generated headers at any point. |
| `dat/*.des` -> `levcomp` -> `.lev` files -> `dlb` -> `nhdat` | Level data changes are invisible until both steps have run. **Never modify `nhdat` directly.** |
| XAML in `GnollHackX/` -> `makedefsdroid` regeneration -> GnollHackM code-behind | The GnollHackM build cannot resolve `x:Name` references from new XAML elements until the transform has run. Never edit the generated GnollHackM XAML. Adding a *new* page or control also requires a `makedefsdroid.vcxproj` edit first. |

See `client_implementation_planning` for the full Build Impact section, and
`build_pipeline` for the pipeline reference.

---

## Escalation Is the Common Case Here

The global rule is that a well-specified step from an approved plan is `standard` work,
escalating to `deep` when the step is ambiguous, spans a layer boundary, or touches a
contract. **Both halves matter equally, and in this repository the second half fires
often.**

GnollHack has an unusually high share of genuinely cross-layer work. Assign `deep` for
anything touching:

- **C-to-C# interop** -- struct layout, marshalling, enum values, callback signatures.
  Getting alignment wrong produces silent corruption, not a compile error.
- **Save-file format** -- struct alignment and file I/O that must stay compatible across
  platforms and versions.
- **The glyph and layer pipeline** -- glyph numbering, `LayerInfo`, tile mapping, and the
  double-buffered map data shared across threads.
- **Anything crossing one of the build chains above**, where the consequences of a mistake
  only appear after a regeneration step.

Routine work -- adding a monster following the existing macro pattern, a localized C fix,
a self-contained MAUI page change -- is `standard`.

---

## Example Subagent Use Section

```markdown
## Subagent Use

### Subagents Needed
Yes -- 8 files across 3 subsystems, with non-overlapping file sets after the makedefs step.

### Subagent Assignments
| Task | Tier | Files | Rationale |
|------|------|-------|-----------|
| Add the `FOO_BAR` define to all platform headers | mechanical | `include/pcconf.h`, `include/unixconf.h`, `include/macconf.h` | Identical pre-specified line; the subagent decides nothing |
| Add the new monster entry following the existing MON macro pattern | standard | `src/monst.c` | Well-specified step following an established pattern |
| Update `struct layer_info` and its C# marshalling | deep | `include/layer.h`, `win/win32/xpl/GnollHackX/GnollHackX/GHStructs.cs` | Interop struct alignment: a mistake corrupts silently rather than failing to compile |
| Implement the new window proc callback end to end | deep | `include/winprocs.h`, `src/windows.c`, `GnollHackService.cs` | Cross-layer contract spanning C core, bridge, and frontend |

Sequencing: the `makedefs` rebuild runs after the `src/monst.c` change and before any
agent touches code referencing the regenerated constants.

### Human Assignments
None -- no large relocation operations.
```
