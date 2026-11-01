# Developer Runbooks

When an agent finishes work on GnollHack, whatever only a person can do next goes in a **Developer
Runbook**: device tests, wizard-mode checks, builds an agent cannot run, upstream reports to file,
work for another developer, things to look for in Sentry after a release.

## Where They Are

Each task has its own, in the shared `plans` repository, beside its plan:

```text
hyvanmielenpelit/GnollHack/<date>/<task>/developer_runbook_v<N>.md    what to do
hyvanmielenpelit/GnollHack/<date>/<task>/walkthrough.md               what happened: "## Runbook status"
```

## What Is Still Open

Pull the plans repository first, to see other developers' results:

```powershell
git -C C:\hmp\plans pull
```

```powershell
& C:\hmp\SharedAgentSkills\tools\Get-OpenRunbookSteps.ps1 -Scope hyvanmielenpelit/GnollHack
```

Only the Deck, only tests:

```powershell
& C:\hmp\SharedAgentSkills\tools\Get-OpenRunbookSteps.ps1 -Scope hyvanmielenpelit/GnollHack -Where 'Steam Deck' -Kind TEST
```

Or ask an agent: "what is pending on the Deck?" It can also build once and combine the tests of
several tasks into one session.

The list covers tasks with a status table. Older tasks appear only if their tests were added to it
later, so an empty list does not prove that nothing older is outstanding.

## Recording Results

Tell an agent, in any words: "gamepad_focus_glow M1-M6 passed, M7 failed: the glow is clipped." If the
task has follow-up rounds, say which one; the list shows it as `<task> (A)`. The agent updates the
table and commits it to the `plans` repository. You can also edit the table in the walkthrough
yourself.

| State | Meaning |
|-------|---------|
| `remaining` | Not done yet |
| `failed` | Tried, and it did not do what the runbook expects |
| `blocked` | Cannot be done now; the note says why |
| `done` | Done, and it did what the runbook expects |
| `waived` | Deliberately not done; your decision |
| `superseded` | Replaced by a later round or task, which the note names |

The first three are open, and they are what the list shows.

## For Agents

The format and rules are in the `agent-developer-runbook` and `client_developer_runbook` skills.
