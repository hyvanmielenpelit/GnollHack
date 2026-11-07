---
name: client-developer-runbook
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

The full skill lives in this repository's tool-neutral agent directory (`.agents/`),
which is shared with other AI coding agents. This file is only a pointer.

Read `.agents/skills/client_developer_runbook/SKILL.md` (path relative to the repository root) in full
before proceeding, and follow it. Any `references/` files it links are relative to that
same directory.
