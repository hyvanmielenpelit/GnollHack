---
name: client_developer_runbook
description: >-
  Developer Runbooks in the GnollHack client repository: where manual tests and every other
  post-implementation step go, and how to find what is still open. Covers the step kinds and Where
  values used here, device test sessions on the Steam Deck, WinUI 3, desktop Linux, Android and
  iOS, wizard-mode checks, combining open tests into one device session, upstream reports waiting
  on a test, Sentry issues to watch after a release, and recording results. Read when a GnollHack
  change needs a manual test or anything else from a person, before reporting such work as
  finished, when the user asks which tests are pending or what to test on a device, and when the
  user reports a test result.
---

# GnollHack Developer Runbooks (Project Overlay)

> **If the shared skills are not installed**, this is the whole baseline: anything the work leaves
> a person beyond the commit goes in `developer_runbook_v<N>.md` in the task directory, with an
> `At a Glance` table of one row per ID (`| ID | Kind | Where | What | Needs |`), followed by the
> steps. Before the first step is handed over, the walkthrough gets a `## Runbook status` section with
> the table `| ID | Kind | Where | What | State | Date | Note |`, and `State` is one of `remaining`,
> `failed`, `blocked`, `done`, `waived` or `superseded`. Only the user's report closes a check. Ask for the
> fewest checks that could show the change is wrong: one for a trivial change, at most three for an
> ordinary one, at most five for a large one. Install
> the full guidance from `https://github.com/hyvanmielenpelit/SharedAgentSkills` (`.\setup.ps1`).

> [!IMPORTANT]
> **Global baseline delegation.** The document format, step cards, test sessions, the status table
> and its states, recording results, and the lister are in the global
> **`agent-developer-runbook`** skill. Read it first. This skill is what is specific to **this
> repository**.

---

## What Goes in a Runbook Here

| Left for a person | Kind | Example |
|-------------------|------|---------|
| A build or setup an agent cannot or may not run: the Visual Studio IDE, a legacy platform, a Mac build host, `setup.ps1` | `BUILD` | The iOS build with `Pair to Mac` |
| Putting a build on a device. The agent runs it when it can, and records `done` | `DEPLOY` | `Deploy-SteamDeck.ps1`, which the Deck may be too asleep to accept |
| A check on a device or in the running game | `TEST` | Deck checks in Gaming Mode; a wizard-mode scenario |
| Work for another developer or another repository's session | `HANDOFF` | A plan for the Overseer developer; a wiki session |
| An upstream issue ready to be filed | `FILE` | A draft in the task directory that waits on a device run |
| Committing, in a runbook that exists for another reason | `COMMIT` | The walkthrough's commit description |
| A check that can only happen after a release | `WATCH` | A Sentry issue on the first version with the fix |
| An agent session the person starts later | `AGENT` | "Read the Deck's log after M3" |

A plan whose only remaining step is the commit has no runbook. A check that belongs to the next
release candidate is a `TEST`, left `blocked` with a note saying so.

**`DEVEL/developer_runbooks.md`** is the short guide for developers; keep it in step with this
skill and with `agent-developer-runbook`.

### `Where` values

The `Where` column uses exactly these, so that a filter finds everything for one device.

| `Where` | Means |
|---------|-------|
| `Steam Deck` | Avalonia Linux build, in Gaming Mode unless the session says otherwise |
| `Linux PC` | Avalonia Linux package on a desktop Linux machine |
| `WinUI 3` | Windows PC, Debug x64 from Visual Studio |
| `Avalonia Windows` | The non-shipping Avalonia Windows build |
| `Android` | An Android device; the session names the model when it matters |
| `iOS` | An iPhone or iPad |
| `Any` | Any build you have at hand, usually a wizard-mode check of C core behavior |
| `Visual Studio` | A build or project step in the IDE |
| `Mac` | The Mac: the build host, and the macOS flavor run from Terminal |
| `This machine` | A command on your own computer, such as `setup.ps1` |
| `Git` | A commit |
| `GitHub` | An upstream issue |
| `Sentry` | Sentry's issues |
| `Overseer developer` | Another developer's implementation and deployment |
| `Agent session` | A new agent session |

A new value is added here first. A gamepad check's `Where` is the device `client_gamepad_testing`
chose for it.

---

## How Few Checks

`agent-developer-runbook` sets the limit: **one for a trivial change, at most three for an ordinary one,
at most five for a large one**, in total, and fewer when fewer will do. What that means here:

- **C core and game data:** one wizard-mode scenario on `Any` for each behavior changed. The same C runs
  everywhere, so do not repeat it per platform.
- **Shared C#:** one platform, the one easiest to run. Add another only if the change alters how the code
  uses something that platform implements itself: a native control, a dialog, the frame loop.
- **Shared controller code:** the Steam Deck only, as `client_gamepad_testing` already says. Its first
  implementation on a new platform gets a full pass there, the one standing exception to the limit, and
  that list is still as short as it can be.
- **Android and iOS smoke tests:** only when their own code changed. The agent's compile builds cover the rest.
- **Automated first:** whatever `GnollHackTests` or a build can establish is not a check for a person.

A hint text change is one check, or none. A fix in one C function is one wizard-mode scenario.

---

## Order of the Steps

| Position | Kinds | Why |
|----------|-------|-----|
| First | `BUILD`, `DEPLOY` | The tests need them |
| Then | `TEST` | One session per device |
| Then | `HANDOFF`, `FILE` | An upstream report or another developer's work often waits on a test result; say so in `Needs` |
| Then | `COMMIT` | `Needs` stays `-`: you may commit before testing, as long as a later fix is a new commit |
| Last | `WATCH`, `AGENT` | These can stay open for a long time, and nothing else may wait for them |

Keep an agent session's steps out of the middle of a device session.

---

## Writing a Test Session

**Setup** says what must be running, in terms a person can check:

- The build: the version, and how to confirm it. On the Deck, `~/gnollhack-dev/VERSION` and the
  `client_gamepad_testing` deploy output.
- The starting state: which page, which kind of game, which setting.
- For wizard mode, the character and commands, with the exact keys: `^W` to wish for an item,
  `^G` to create a monster, `^V` to teleport between levels (see `debugging_gnollhack`).

**Do** and **Expect** name what the person presses and sees, using the labels the app prints. Check
UI text against the XAML or C# rather than from memory. Check something unchanged only when you can name
how this change could break it.

**If a check fails** names what to capture: the Deck's `~/gnollhack-dev/logs/latest.log`, a
screenshot, or the Sentry event, and tells the person to give the agent the check's ID.

A controller check follows `client_gamepad_testing`: that skill chooses the devices and its
procedure produces the build. The session records which rule it applied.

```markdown
### TEST: Steam Deck (M1-M3)
- Who: You
- Needs: S1
- Setup: Gaming Mode, started from the GnollHack shortcut. The deploy in S1 printed the version;
  `~/gnollhack-dev/VERSION` on the Deck shows the same one. Start a new game with any character.

| ID | Do | Expect |
|----|----|--------|
| M1 | Open Settings and walk to the bottom with the D-pad | Focus reaches the last row, and the page scrolls with it |
| M2 | Return to the game, open the inventory and pick an item | The item's menu opens, and focus is on its first entry |
| M3 | Answer a y/n prompt with the controller | The prompt closes, and the game takes the answer you gave |

- If a check fails: note the ID, then run the Deck's log command from `client_gamepad_testing` or
  ask the agent to read `~/gnollhack-dev/logs/latest.log`.
```

---

## One Device, Many Tasks

Tests pile up per device, and one build of the current tree can serve many tasks. When the user
says they have a device at hand, or asks what to test on it:

1. Pull the plans repository and list them:

   ```powershell
   & C:\hmp\SharedAgentSkills\tools\Get-OpenRunbookSteps.ps1 -Scope hyvanmielenpelit/GnollHack -Where 'Steam Deck'
   ```

2. Build and deploy once, following `client_gamepad_testing` or `build_pipeline`.
3. Read the sessions of the tasks in the list. Before handing a check over, check that the code it
   describes still exists; `git log` on the files the plan names is usually enough. A check of a
   feature that has since changed is either re-worded in the session you hand over, or marked
   `superseded` with a note once the user agrees.
4. Give the user the checks **with their task, round and ID**, so that the results can be recorded.
5. Record the results per task, as `agent-developer-runbook` says.

---

## Connections to Other Skills

- **`client_implementation_planning`** -- a plan's Manual verification names its runbook and IDs;
  the checks live only in the runbook.
- **`client_gamepad_testing`** -- which device; packaging and deploying; an unreachable Deck leaves
  the `DEPLOY` step and its checks `blocked`. An agent never waives them. If the user decides that the
  Xbox backup is enough, the checks become `waived`, and the note says so.
- **`sentry_crash_analysis`** -- when a fix is implemented, the Testing action that best shows the fix
  works becomes a `TEST` check, within the limit above, and its Monitoring action becomes a `WATCH`
  step. A `WATCH` step names the issue, the
  first version with the fix, and what would refute it.
- **`client_workarounds`** -- a full re-check that needs a device is a `TEST` check. Until its result
  is recorded, the entry's newest `Checks` line says `unverified`. Recording it also appends a new
  `Checks` line and updates `Last checked`, and that edit stays uncommitted in this repository for the
  user. An upstream report drafted with `agent-issue-drafting` that waits on a test is a `FILE` step.

## What the List Does Not Show

- **Steps of a task whose walkthrough does not exist yet.** The task is listed as planned, or counted
  when a filter is on. A session that stopped before writing its walkthrough looks the same.
- **`gnollhack_` plans filed under `hyvanmielenpelit/MobileGnollHackLogger`.** A benchmark round
  keeps its GnollHack plan in that scope, with the steps in that round's runbook.
- **Standing procedures**, such as the release smoke list in `client_gamepad_testing` and the upgrade
  runbook in `win/win32/xpl/gnhsentrywin/README.md`. Those stay where they are, and a step may
  point to them.
- **Older tasks**, unless a backfill added a status table to their walkthroughs.
