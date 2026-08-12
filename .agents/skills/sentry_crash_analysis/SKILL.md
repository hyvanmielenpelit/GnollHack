---
name: sentry_crash_analysis
description: >-
  Systematic methodology for diagnosing GnollHack crash reports from the Sentry
  MCP server. Use when the user asks to analyze a Sentry issue, investigate a
  crash, or review unresolved GnollHack errors. Covers data retrieval via Sentry
  MCP tools, structured analysis of stack traces, breadcrumbs, debug buffers,
  and game state, cross-event pattern recognition, and drawing evidence-based
  conclusions with confidence levels. Emphasizes avoiding false diagnoses.
---

# Sentry Crash Analysis for GnollHack

This skill is a **diagnostic tool**. Its purpose is to help narrow down the
correct diagnosis of a GnollHack crash — or identify what further data is needed
to progress toward one — while equally importantly **avoiding false diagnoses**.

> [!CAUTION]
> **No jumping to strong conclusions based on inconclusive evidence.** Every
> hypothesis must cite specific evidence. If the data is insufficient, say so
> explicitly rather than speculating. A wrong diagnosis is worse than no
> diagnosis — it wastes developer time and can lead to incorrect fixes.

## Critical Rules

- This skill is for **diagnosis only** — do NOT modify GnollHack source code as
  part of the analysis. Fix implementation is a separate task.
- Always retrieve **both** issue details AND breadcrumbs before drawing any
  conclusions. Never analyze based on stack trace alone.
- When multiple events exist for an issue, examine **all of them** and compare
  across events. Patterns across events are among the strongest diagnostic
  signals.
- Research subagents **cannot call MCP tools directly**. The orchestrator agent
  must fetch all Sentry data and forward it to subagents via `send_message`.

---

## Section A: Retrieving Issue Data from Sentry MCP Server

### A.1 Organization & Project Constants

Use these hardcoded values for all Sentry MCP tool calls:

```
organizationSlug = "hyvan-mielen-pelit-ry"
regionUrl        = "https://de.sentry.io"
project          = "gnollhack"
```

Only fall back to `find_organizations` / `find_projects` if these values stop
working (e.g., the MCP tool returns an error about an unknown organization).

### A.2 Issue Discovery

When the user asks to **list unresolved issues**, use:

```
search_issues(
    organizationSlug = "hyvan-mielen-pelit-ry",
    regionUrl        = "https://de.sentry.io",
    query            = "is:unresolved",
    sort             = "new",
    period           = "90d",
    limit            = 25
)
```

When the user asks about a **specific issue** (e.g., `GNOLLHACK-G6`), skip
listing and go directly to retrieval (A.3).

### A.3 Issue Detail Retrieval

For every issue under analysis, fetch **at minimum two resources**:

**1. Issue details** — error message, stack trace, tags, extra data:

```
get_sentry_resource(
    resourceType     = "issue",
    resourceId       = "GNOLLHACK-XX",
    organizationSlug = "hyvan-mielen-pelit-ry",
    regionUrl        = "https://de.sentry.io"
)
```

**2. Breadcrumbs** — chronological record of player actions before the crash:

```
get_sentry_resource(
    resourceType     = "breadcrumbs",
    resourceId       = "GNOLLHACK-XX",
    organizationSlug = "hyvan-mielen-pelit-ry",
    regionUrl        = "https://de.sentry.io"
)
```

Both calls are independent and can be made in parallel.

### A.4 Multi-Event Investigation

Check the **occurrences count** from the issue details. If the issue has
multiple events, examine all of them:

**1. List individual events:**

```
search_events(
    organizationSlug = "hyvan-mielen-pelit-ry",
    regionUrl        = "https://de.sentry.io",
    dataset          = "errors",
    query            = "issue:GNOLLHACK-XX",
    sort             = "-timestamp",
    limit            = 25
)
```

**2. Fetch each event's details:**

```
get_sentry_resource(
    resourceType     = "event",
    resourceId       = "<event_id>",
    organizationSlug = "hyvan-mielen-pelit-ry",
    regionUrl        = "https://de.sentry.io"
)
```

**3. Cross-event comparison** (mandatory when multiple events exist):

| Comparison Axis      | What to Look For                                          |
|----------------------|-----------------------------------------------------------|
| **Platform**         | All same platform, or spread across Android/iOS/Windows?  |
| **User / Device**    | Same user/device, or different users?                     |
| **Breadcrumbs**      | Same player action sequence, or different paths to crash? |
| **Stack Traces**     | Identical frames, or divergent crash points?              |
| **Version**          | Same GnollHack version, or spanning multiple versions?    |
| **Debug Buffers**    | Same code paths, or different game logic areas?           |
| **Device Resources** | Low memory/storage common across events?                  |

**Burst detection**: If all events share the same user, device, and timestamps
within a narrow window (< 60 seconds), treat them as a **single logical
incident**, not independent crashes. Note the burst count — it indicates how
many times the same code path was hit (e.g., iterating over a collection of
affected objects), which itself is a diagnostic clue.

Document cross-event findings prominently in the analysis report.

### A.5 Data Extraction Checklist

After retrieval, extract and record the following for each event:

- **Crash type**: Segmentation fault (`SIGSEGV`), abort signal (`SIGABRT`),
  `impossible()` warning, `panic()` fatal error, or managed .NET exception
- **Platform**: Android, iOS, or Windows (from tags: `os.name`, `device.family`)
- **GnollHack version**: From `release` tag or `app.app_version` — compare
  against the latest known release to assess if this is a current or outdated
  version issue
- **Stack trace**: Full list of frames, noting which are known symbols vs
  `<unknown>`
- **Breadcrumbs**: Chronological list, paying special attention to the last
  10–20 entries before the crash
- **Debug Buffers**: The `1:...; 2:...; 3:...` formatted C-side debug log from
  `extra` data — circular buffer entries from `debugprint()` calls in the C
  library (25 slots max, newest first)
- **Game State**: Appended after debug buffers with this format:
  `|plname:X,ux:N,uy:N,dnum/level:N/N (dungeonname,levelname),moves:N,role:N,race:N,gameover:N,mklev:N,bones:N`
- **Device info**: Memory, storage, battery level (low memory or storage can
  cause crashes on mobile platforms)

### A.6 Subagent Considerations

Research subagents do **not** have access to MCP tools. If you delegate analysis
to subagents:

1. The orchestrator must call all Sentry MCP tools itself.
2. Forward the retrieved data to the subagent via `send_message`.
3. The subagent can then analyze the data, search the GnollHack codebase, and
   report back.

---

## Section B: Analysing the Issue

### B.1 Crash Classification

Categorize every crash into one of these types:

| Category               | Typical Sentry Signature                                   | What It Means                                                                                        |
|------------------------|------------------------------------------------------------|------------------------------------------------------------------------------------------------------|
| **Segmentation Fault** | `SIGSEGV` / `SIGABRT` / `EXC_BAD_ACCESS`                  | Null pointer dereference, use-after-free, buffer overflow, or dangling pointer in C core or native code |
| **Impossible**         | Message contains `impossible()` text                       | GnollHack detected an internal logic inconsistency — non-fatal warning, but indicates a bug          |
| **Panic**              | Message contains `panic()` text                            | GnollHack detected a fatal error condition it cannot recover from                                    |
| **Managed Exception**  | `NullReferenceException`, `ObjectDisposedException`, etc.  | Bug in the C# MAUI frontend code                                                                    |
| **Platform/OS Crash**  | Stack frames entirely in system libraries (`UIKit`, `WKWebView`, `libdispatch`) | Crash in platform code, possibly triggered by GnollHack but rooted in OS-level behavior |

**Key distinction**: `impossible()` and `panic()` crashes are caught by
GnollHack's own error detection — they include a descriptive message string that
often directly names the problem (e.g., `"onbill: unpaid obj not on bill?"`,
`"Cannot find monster usteed"`). These messages are the single most valuable
diagnostic clue for these crash types. Search the codebase for the exact message
string to locate the triggering code.

### B.2 Stack Trace Analysis

> [!NOTE]
> For `impossible()` and `panic()` crashes, a native stack trace is often **not
> available** — the error is caught in C code and reported to Sentry via the GUI
> command callback (e.g., `GHGame.ClientCallback_IssueGuiCommand`). In these
> cases, **the error message string itself is the primary locator**. Search the
> codebase for the exact message text (e.g., `grep_search` for
> `"onbill: unpaid obj not on bill"`) to find the triggering code.

Follow this systematic approach:

1. **Identify the crash point** — the topmost frame in GnollHack code, or the
   transition point from system frameworks to application code.
2. **Map function names to source files** — use `grep_search` to find the
   function definition in the codebase (search in `src/`, `include/`,
   `win/win32/xpl/`).
3. **Handle `<unknown>` frames**:
   - These are stripped symbols, common in release builds.
   - Note their position in the stack and consider what functions could
     logically sit between the known frames above and below, based on the call
     graph in the source code.
   - The `<unknown>` frame may be the actual buggy function.
4. **For native C crashes**: Read the source file around the crash location.
   Look for:
   - Pointer dereferences (especially without null checks)
   - Array indexing (potential out-of-bounds)
   - Use of freed objects (`DEADMONSTER()` checks, `dealloc_obj()` calls)
   - Conditional branches that might skip initialization
5. **For platform-only stacks** (no GnollHack frames visible): The crash is in
   system code triggered by a GnollHack action. Breadcrumbs become the
   **primary** diagnostic tool — the stack trace alone cannot explain the cause.

> [!WARNING]
> **A function appearing in the stack trace does not mean it caused the crash** —
> it may be a victim of corruption caused earlier. Look for the _initiating_
> action (often visible in breadcrumbs or debug buffers), not just the _failing_
> function.

### B.3 Breadcrumb Analysis

Breadcrumbs record what the player was doing before the crash. GnollHack uses
four custom breadcrumb categories:

| Category                  | Constant Name                               | What It Records                                               |
|---------------------------|---------------------------------------------|---------------------------------------------------------------|
| `GnollHack Information`   | `SentryGnollHackGeneralCategoryName`        | App lifecycle: start, pause, resume, save, memory warnings    |
| `GnollHack Library`       | `SentryGnollHackLibraryCategoryName`        | C library commands sent to the game engine (player actions)   |
| `GnollHack Callback`      | `SentryGnollHackCallbackCategoryName`       | UI callbacks from C: display windows, yn questions, menus, getline |
| `Button Clicked`          | `SentryGnollHackButtonClickCategoryName`    | UI button presses (CustomImageButton, LabeledImageButton, etc.) |

Platform breadcrumbs may also appear with categories like `ui.lifecycle`,
`ui.useraction`, `touch`, `navigation`, `device.event`, `ui.rendering`.

**Reconstruction technique**:

1. Read breadcrumbs chronologically (oldest to newest).
2. Identify the sequence of player actions: button presses → game commands →
   UI callbacks → game responses.
3. Look for the **transition point** — the action after which things went wrong.
4. Pay special attention to:
   - App lifecycle events (`OnSleep`, `OnResume`) — backgrounding/foregrounding
     can corrupt state.
   - Memory warnings — may indicate resource exhaustion.
   - The specific game command or UI interaction immediately before the crash.

> [!WARNING]
> **The last breadcrumb before the crash is NOT necessarily the cause.** The
> causal action may be several breadcrumbs earlier, with the crash manifesting
> later due to silently corrupted state. Always examine the full breadcrumb
> sequence, not just the final entries.

### B.4 Debug Buffer Analysis

Debug buffers are a **circular ring of 25 entries** from `debugprint()` calls
scattered throughout the C codebase (~50+ source files use it). They record code
paths reached during gameplay.

> [!IMPORTANT]
> Debug buffer entry **1 is the MOST RECENT** (the last `debugprint()` call
> before the crash). Entry 25 is the oldest. **To read chronologically, reverse
> the list** — start from the highest-numbered entry and work down to 1.

**How to interpret them**:

- Entries are numbered `1:...; 2:...; 3:...` (1 = most recent).
- Each entry typically contains a source file name and line number, e.g.,
  `"Line 1005 in shk.c"`.
- **Cross-reference** these line numbers with the actual source code to
  understand what code paths were recently executed. Use `view_file` to read the
  relevant lines.
- Look for: repeated entries (indicating a loop or retry), entries from
  unexpected source files (indicating unusual code paths), and entries that
  correspond to the code area implicated by the stack trace.
- Present debug buffers in the report as a **chronological table** (oldest
  first) with columns: `#`, `Entry`, `Source File/Function`, `Interpretation`.

**Limitation**: The 25-slot circular buffer overwrites old entries. If many
`debugprint()` calls occurred between the problematic action and the crash, the
most relevant entry may have been lost. This is a known limitation — the skill
should recommend adding more targeted `debugprint()` calls in the suspected area
if the existing buffers are insufficient.

### B.5 Game State Analysis

> [!NOTE]
> Game state data may not be present in older Sentry events, as this feature was
> added recently. If absent, note this in the report and rely on breadcrumbs and
> debug buffers for context about the player's situation.

Game state data is appended after the debug buffers in the `extra` data field.
Parse the pipe-delimited string:

```
|plname:X,ux:N,uy:N,dnum/level:N/N (dungeonname,levelname),moves:N,role:N,race:N,gameover:N,mklev:N,bones:N
```

| Field           | Meaning                                    | Diagnostic Value                                              |
|-----------------|--------------------------------------------|---------------------------------------------------------------|
| `plname`        | Player name                                | Identifies the player (cross-reference with user info)        |
| `ux`, `uy`      | Player x,y coordinates                     | Where on the map the player was                               |
| `dnum/level`    | Dungeon number / dungeon level             | Which dungeon branch and depth                                |
| `(name,name)`   | Dungeon name, level name                   | Human-readable location (e.g., `"Dungeons of Doom,normal"`)   |
| `moves`         | Turn count                                 | Higher = later game = more complex state accumulated          |
| `role`          | Role index                                 | Player's character class                                      |
| `race`          | Race index                                 | Player's race                                                 |
| `gameover`      | 0 or 1                                     | **1 = crash during end-of-game cleanup** — implicates `end.c` |
| `mklev`         | 0 or 1                                     | **1 = crash during level generation** — implicates `mklev.c`, `mkmaze.c`, `mkroom.c`, `sp_lev.c` |
| `bones`         | 0 or 1                                     | **1 = crash during bones processing** — implicates `bones.c`  |

The `gameover`, `mklev`, and `bones` flags are high-value signals that
immediately narrow the investigation to a specific subsystem.

### B.6 Platform Specificity Analysis

1. Check if the issue has events from multiple platforms (compare `os.name` tags
   across events from A.4).
2. Interpret the distribution:
   - **Single platform only**: Suggests platform-specific code paths, OS API
     differences, or hardware-specific issues. Check for platform-conditioned
     code (`#if __ANDROID__`, `#if __IOS__`, platform-specific `.cs` files).
   - **Multiple platforms**: Suggests a C core logic bug in the shared code
     (`src/`), since the C library is compiled for all platforms.
3. Pay special attention to:
   - Memory pressure on mobile (Android/iOS) — check for memory warning
     breadcrumbs.
   - App lifecycle events (backgrounding/foregrounding) — these can interrupt
     save operations or leave state inconsistent.
   - WebView behavior differences between platforms (relevant for Overseer
     integration).

> [!NOTE]
> **Don't assume platform exclusivity without evidence.** A crash seen only on
> iOS may simply reflect that most players are on iOS, not that the bug is
> iOS-specific. Check user counts per platform before concluding.

### B.7 Version Currency Check

1. Extract the `release` tag from the issue (format: typically `X.Y.Z.N`).
2. Compare against the latest GnollHack version (check `include/date.h` for
   `VERSION_MAJOR`, `VERSION_MINOR`, `PATCHLEVEL`, `EDITLEVEL`).
3. If the crash is from an **older version**:
   - Check `git log` for fixes to the relevant code area since that version.
   - The bug may already be fixed — note this in the report but don't dismiss
     the issue entirely (see Pitfall E.4).
4. If the crash is from the **current version**: It is an active issue requiring
   attention.

---

## Section C: Drawing Conclusions and Suggesting Further Actions

### C.1 Root Cause Assessment — Confidence Scale

Every analysis must assign a confidence level to its conclusions:

| Confidence Level | Criteria                                                    | Example                                                       |
|-----------------|-------------------------------------------------------------|---------------------------------------------------------------|
| **High**        | Stack trace points directly to a specific line in GnollHack code; the bug is mechanically obvious (null pointer, array bounds) | Crash at `splitobj()` line 42 where `otmp->nobj` is dereferenced without null check |
| **Medium**      | Stack trace + breadcrumbs + debug buffers together suggest a specific code area, but the exact mechanism is unclear | Breadcrumbs show inventory manipulation, stack has `<unknown>` frames between `use_container` and crash, debug buffers confirm shop code was active |
| **Low**         | Only a general area can be identified; multiple possible causes exist | Crash during level change; could be save/restore, level generation, or monster migration |
| **Insufficient** | Available data does not meaningfully narrow down the cause  | Stack is entirely in system libraries with no GnollHack frames; breadcrumbs only show UI lifecycle events |

> [!CAUTION]
> **Epistemic humility rule**: If confidence is **Low** or **Insufficient**, the
> AI **MUST** explicitly state: _"I cannot identify the root cause with the
> available data."_ Do not speculate. Instead, focus the analysis on what
> additional data (debug instrumentation, specific test scenarios, or future
> crash reports) would be needed to make progress.

### C.2 Hypothesis Formulation

When confidence is **Medium** or higher, list possible causes as numbered
hypotheses:

For **each hypothesis**, document:

1. **Description**: What the hypothesized cause is.
2. **Evidence For**: Cite specific evidence — stack frame names, breadcrumb
   entries, debug buffer entries, game state flags. Use exact quotes and
   line/entry numbers.
3. **Evidence Against**: What evidence would disprove this hypothesis, or what
   data is missing that would be needed to confirm it.
4. **Likelihood Rank**: Relative ranking among the listed hypotheses.

When confidence is **Low** or **Insufficient**, skip hypothesis formulation and
proceed directly to C.3 (debug instrumentation recommendations) and C.4 (testing
feasibility).

### C.3 Recommended Debug Instrumentation

Suggest targeted `debugprint()` additions that would help diagnose this crash
type in future Sentry reports:

- **Identify the implicated code area** from the analysis.
- **Suggest specific `debugprint()` calls** with format strings that capture the
  key variables and conditions. Follow the existing codebase pattern:
  ```c
  debugprint("function_name: key_var=%d, ptr=%p, condition=%d",
             key_var, (void *)ptr, condition);
  ```
- **Prioritize prints that would distinguish between hypotheses** — if you have
  two hypotheses, suggest a print that would confirm one and refute the other.
- **Consider entry/exit prints** for suspected functions, and state-capture
  prints before pointer dereferences or complex operations.
- **Note the 25-slot circular buffer limit** — don't suggest so many prints that
  they would flood the buffer and overwrite each other. Focus on the most
  diagnostic locations.

### C.4 Local Testing Feasibility

Assess whether the crash can be reproduced locally without the player's save
file:

| Crash Context                    | Reproducibility | How to Test                                                   |
|----------------------------------|-----------------|---------------------------------------------------------------|
| Level generation (`mklev=1`)     | **Possible**    | Generate many levels of the implicated type in wizard mode (`^V` to teleport to levels repeatedly) |
| Specific items/monsters          | **Possible**    | Use wizard mode: `^W` (wish) for items, `^G` (create) for monsters, then perform the implicated action |
| Shop interactions                | **Possible**    | Wish for items, find a shop via `^V`, and perform the implicated shop operation |
| Save/restore corruption          | **Unlikely**    | Requires the specific corrupted save file; suggest adding validation code instead |
| App lifecycle (background/resume)| **Possible**    | Minimize and restore the app during gameplay, especially during critical operations |
| Platform-specific iOS            | **Difficult**   | Requires Mac build host + physical device or simulator        |
| Bones file processing            | **Possible**    | Create bones files in wizard mode by dying, then load them with a new character |

If a test scenario is feasible, describe the **specific steps** using wizard
mode commands (refer to the `debugging_gnollhack` skill for details).

### C.5 Cross-Issue Pattern Recognition

When analyzing multiple issues in the same session, look for:

- **Same code area** appearing across different crash issues (suggests a systemic
  bug rather than an isolated edge case).
- **Same player or device model** appearing repeatedly (suggests a
  device-specific or user-specific trigger).
- **Crashes clustering around specific game events** (level changes, save/restore,
  shop interactions, inventory management) — suggests a subsystem-level problem.
- **Version-specific clusters** — multiple crashes appearing on the same version
  but not on others suggests a regression introduced in that version.

### C.6 Action Items Summary

Conclude every analysis with a prioritized action items list:

| Priority           | Category          | Description                                           |
|--------------------|-------------------|-------------------------------------------------------|
| **1. Immediate**   | Fix               | Any obvious fixes identified with **High** confidence (rare for external player crashes) |
| **2. Instrumentation** | `debugprint()` | Specific debug prints to add for future crash diagnosis |
| **3. Investigation** | Code review     | Source code areas to review manually for potential issues |
| **4. Testing**     | Wizard mode       | Specific test scenarios to attempt locally              |
| **5. Monitoring**  | Future reports    | What to watch for in future Sentry reports to confirm/refute hypotheses |

---

## Section D: Analysis Report Format

Produce a structured artifact named `sentry_analysis_report.md` (or update an
existing one if analyzing multiple issues). Use this template:

```markdown
# Sentry Issue Analysis: GNOLLHACK-XX

## Issue Summary

| Field              | Value                                           |
|--------------------|-------------------------------------------------|
| Issue ID           | GNOLLHACK-XX                                    |
| Crash Type         | SIGSEGV / Impossible / Panic / Managed Exception|
| Platform(s)        | Android / iOS / Windows                         |
| GnollHack Version  | X.Y.Z.N (current / outdated by N versions)      |
| Occurrences        | N events, M users                               |
| First Seen         | YYYY-MM-DD                                      |
| Last Seen          | YYYY-MM-DD                                      |
| Confidence         | High / Medium / Low / Insufficient              |

## Evidence Analysis

### Stack Trace
[Annotated stack trace with source file mappings and notes on <unknown> frames.
For impossible()/panic() crashes with no native stack trace, note this and
explain how the crash location was identified via the error message string.]

### Breadcrumb Reconstruction
[Chronological narrative of player actions before crash, highlighting the
suspected triggering action]

### Debug Buffer Reconstruction (chronological order)
[Table with columns: # (chronological), Entry, Source File/Function,
Interpretation. Present in chronological order (oldest first), not raw Sentry
order. Cross-reference each entry with the actual source code.]

### Game State at Crash
[Parsed game state fields with interpretation of flags like mklev, bones,
gameover. If game state data is absent, note this explicitly.]

### Version Currency
[GnollHack version from the release tag compared against current version.
Note whether the affected code has changed since the crash version.]

### Cross-Event Comparison
[If multiple events: table comparing platforms, users, versions, breadcrumb
patterns, and stack traces across events. If burst pattern detected, note
the burst count and what it implies.]

## Conclusions

| # | Hypothesis | Confidence | Evidence For | Evidence Against |
|---|-----------|-----------|--------------|------------------|
| 1 | ...       | High/Med  | ...          | ...              |
| 2 | ...       | Med/Low   | ...          | ...              |

[Or: "Insufficient data to form hypotheses. See Recommended Actions."]

## Recommended Actions

1. **[Priority]** [Action description]
2. ...
```

---

## Section E: Common Pitfalls and Anti-Patterns

These warnings are derived from real GnollHack analysis experience. Follow them
strictly.

### E.1 Don't Fixate on the Stack Trace Alone

A prior GnollHack Sentry analysis initially attributed a SIGABRT crash to the
`ShareFile` code path based on the stack trace. Breadcrumb analysis later
revealed the actual trigger was the iOS Photo Picker invoked from a WebView.
**Always cross-reference stack traces with breadcrumbs before forming any
hypothesis.**

### E.2 Don't Assume the Crashing Function Is the Bug

In C code, a null pointer dereference in function B is often caused by function
A failing to initialize or preserve data correctly earlier. The function that
_crashes_ is frequently a _victim_, not the _perpetrator_. Follow the data flow
backwards from the crash point.

### E.3 Don't Ignore `<unknown>` Frames

Stripped symbols are common in release builds. Consider what function logically
fits between the known frames above and below based on the source code's call
graph. The `<unknown>` frame may be where the actual bug resides.

### E.4 Don't Dismiss "Old Version" Crashes

Even if the crash is from an older GnollHack version, the same bug may still
exist in the current code. Always check whether the relevant source code has
changed since that version using `git log`. Note findings in the report.

### E.5 Don't Over-Trust Debug Buffers

The 25-slot circular buffer overwrites old entries. If many `debugprint()` calls
occurred between the problematic action and the crash, the causally relevant
entry may have been overwritten and lost. If debug buffers don't contain useful
entries, recommend adding more targeted `debugprint()` calls in the suspected
area.

### E.6 Don't Assume Platform Exclusivity Without Evidence

A crash seen only on iOS may simply reflect that most GnollHack players are on
iOS, not that the bug is iOS-specific. Check user counts per platform across
Sentry before concluding that a crash is platform-specific.

### E.7 Don't Make Fixes Based on Low or Insufficient Confidence

It is better to add instrumentation (`debugprint()` calls) and wait for more
data than to apply a speculative fix that may introduce new bugs, mask the real
issue, or waste developer time — as happened with the G6 ShareFile fix that
addressed the wrong root cause.

### E.8 Don't Conflate Correlation with Causation in Breadcrumbs

Just because a breadcrumb appears near the crash timestamp does not mean that
action caused the crash. The actual cause may have occurred much earlier,
creating silently corrupted state that only manifested as a crash during a later,
unrelated operation.

---

## Related Skills

- **`debugging_gnollhack`** — Wizard mode commands, panic log handling, local
  testing procedures
- **`nethack_c_core`** — C code structure, key data structures (`struct monst`,
  `struct obj`, `struct you`), function naming conventions, macro idioms
- **`maui_frontend`** — C# MAUI frontend architecture, SkiaSharp rendering,
  P/Invoke native bridge, XAML pipeline
- **`maui_threading`** — Multi-threaded programming patterns, lock strategies,
  `ConcurrentQueue` communication (relevant for concurrent access crashes)
- **`save_restore_system`** — Save/load system, struct alignment, file I/O
  (relevant for save corruption and restore crashes)
