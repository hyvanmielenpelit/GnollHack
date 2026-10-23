# FMOD Workarounds

- Component: FMOD Core and FMOD Studio
- Upstream: https://qa.fmod.com
- ID prefix: WA-FMOD

Entries follow the schema in [README.md](README.md). Append new entries at the end of the
file and never renumber or reuse an ID.

<!-- entries -->

## WA-FMOD-0001: mixerSuspend can crash on a freed semaphore on Android

- Status: active
- Added: 2026-10-09 (from git history)
- GnollHack version: 3.25 (425)
- Component: FMOD Core
- Package: FMOD
- Defective version: FMOD 2.03.14
- Platforms: Android
- Upstream issue: https://qa.fmod.com/t/24471
- Upstream status when added: reported
- Upstream status: reported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX/GHConstants.cs` (`GHConstants.FmodAndroidPauseInsteadOfMixerSuspend`); `win/win32/xpl/GnollHackX/GnollHackX.FMOD/FmodService.cs` (`FmodService.UseMasterPauseForSuspend`)
- Remove when: FMOD names a release that fixes the mixerSuspend defect, GnollHack uses that release on Android, and Verify removal passes.
- Related: GNOLLHACK-FN

**Symptom.** When the app goes to the background on Android and `System::mixerSuspend` is
called, the process can crash with a SIGSEGV in FMOD's mixer thread, in `sem_post`.

**Cause.** Per FMOD on the forum thread: when an error occurs during `mixerSuspend`, such
as the AAudio stream failing to stop, the mixer thread is not closed and later posts a
semaphore that has already been freed. FMOD acknowledged the defect in 2.03.14. The same
thread also identifies a separate race, a device disconnected while its replacement is
starting, for which FMOD announced a fix in 2.03.16; this workaround does not avoid that
race.

**Workaround.** On Android, backgrounding pauses the master channel group
(`ChannelGroup.setPaused(true)`) instead of calling `mixerSuspend`, and resuming unpauses
it instead of calling `mixerResume`, as FMOD suggested on the thread. The switch is
`GHConstants.FmodAndroidPauseInsteadOfMixerSuspend`; `FmodService.UseMasterPauseForSuspend`
selects the path in `SuspendMixer`, `ResumeMixer` and `ShutdownFmod`. Other platforms
still suspend the mixer.

**Verify removal.** Upgrade the FMOD bindings and Android libraries to the fixed release,
then set `FmodAndroidPauseInsteadOfMixerSuspend` to `false`. Then:
- Build and install an Android Release build.
- Background and foreground the app at least 50 times, some with music playing, and some
  while connecting or disconnecting headphones during the transition.
- Confirm that the app never crashes, audio resumes each time, and the log shows
  "mixer suspended successfully" on each suspend.
- After shipping, confirm in Sentry that no `sem_post` crash in the FMOD mixer thread is
  reported for that release.

Checks:
- 2026-10-23 | FMOD 2.03.14 | still-needed | Forum thread read today: FMOD acknowledged the defect in 2.03.14 and planned a fix for an unnamed later release; the 2.03.16 fix announced there is for a separate device-restart race. GnollHack still ships 2.03.14.

## WA-FMOD-0002: Studio flushCommands can hang while Android backgrounds the app

- Status: active
- Added: 2026-10-21 (from git history)
- GnollHack version: 3.28 (428)
- Component: FMOD Studio
- Package: FMOD
- Defective version: FMOD 2.03.14
- Platforms: Android
- Upstream issue: none
- Upstream status when added: unreported
- Upstream status: unreported
- Last checked: 2026-10-23
- Code: `win/win32/xpl/GnollHackX/GnollHackX.FMOD/FmodService.cs` (`FmodService.SuspendMixer`)
- Remove when: The hang is identified and fixed upstream, in FMOD or in the Android audio service, GnollHack uses a release containing the fix, and Verify removal passes.
- Related: WA-FMOD-0001, GNOLLHACK-DR

**Symptom.** While the app was being backgrounded, the thread running `FmodService.Suspend`
blocked inside `Studio::System::flushCommands`, and Android reported an ANR.

**Cause.** `flushCommands` waits for the Studio update thread, which can hang in the
platform audio service. Whether FMOD or the Android audio service has to fix this is not
established.

**Workaround.** `SuspendMixer` runs the flush passes on a worker task and waits for them
only for `GHConstants.FmodSuspendFlushTimeoutMs`, or for the shorter
`GHConstants.FmodSuspendFlushAfterDrainTimeoutMs` when the active calls did not drain. On timeout the
mixer is left running and the worker stays inside FMOD.

**Verify removal.** No reliable reproduction is known. With a candidate fix:
- Replace the bounded wait in `SuspendMixer` with direct `flushCommands` calls.
- Build an Android Release build, and background the app repeatedly while music, ambient
  sounds and dialogue play.
- After shipping, confirm that Sentry and Google Play vitals show no ANR with the main
  thread in `FmodService.Suspend` or `flushCommands` for a full release cycle.

Checks:
- 2026-10-23 | FMOD 2.03.14 | still-needed | No upstream report exists, and no fix is known.
