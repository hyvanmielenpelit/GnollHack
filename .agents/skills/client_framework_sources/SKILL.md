---
name: client_framework_sources
description: >-
  Where the source code of GnollHack's frameworks is and how to read it at the version
  GnollHack ships: .NET MAUI, the .NET runtime, .NET for Android, .NET for iOS, WinUI,
  CsWinRT, SkiaSharp, Skia, and HarfBuzz. Covers finding the local clones, which may sit
  in a different folder on each computer, mapping GnollHack's package and workload
  versions to a tag or commit, reading that version without changing the clone, and
  deciding whether a bug is GnollHack's or the framework's. Read when a stack trace,
  exception, or behavior leads into framework code, before working around a framework,
  and before drafting an upstream issue.
---

# Framework Sources

GnollHack runs on frameworks whose source is public. When the evidence leads into one of
them, read its code at the version GnollHack ships rather than reasoning from
documentation or memory.

## The Repositories

| Framework | GitHub | Usual folder | The version GnollHack uses |
|---|---|---|---|
| .NET MAUI | `dotnet/maui` | `maui` | `Microsoft.Maui.Controls` in `win/win32/xpl/GnollHackM/GnollHackM.csproj`, one reference per target framework. Tags equal the package version |
| .NET runtime | `dotnet/runtime` | `runtime` | The SDK pinned in `win/win32/xpl/GnollHackM/global.json`; `dotnet --list-runtimes` gives the runtime. Tags are `v<version>` |
| .NET for Android | `dotnet/android` | `android` | The `android` line of `dotnet workload list`, run in `win/win32/xpl/GnollHackM`. Tags equal its first version number |
| .NET for iOS | `dotnet/macios` | `macios` | The `ios` line of the same command. Tags are `dotnet-<sdk band>-xcode<xcode>-<build>`, matching its first version number |
| WinUI | `microsoft/microsoft-ui-xaml` | `microsoft-ui-xaml` | `Microsoft.WindowsAppSDK.WinUI` in `win/win32/xpl/GnollHackM/obj/project.assets.json` after a Windows restore; it comes in through MAUI. Tags are `winui3/release/<major.minor.patch>`; match the patch through the Windows App SDK release notes |
| CsWinRT | `microsoft/CsWinRT` | `CsWinRT` | No direct reference: the projection comes with the .NET SDK's Windows SDK package and the Windows App SDK. Read the default branch, and say that the version is approximate |
| SkiaSharp | `mono/SkiaSharp` | `SkiaSharp` | `SkiaSharp` in `GnollHackM.csproj`; the `SKIASHARP_3119` define selects the older of two versions, so check which is active. Tags are `v<version>` |
| Skia | `mono/skia`, a fork of `google/skia` | `skia` | The `externals/skia` submodule commit of SkiaSharp at its tag: `git -C <SkiaSharp> ls-tree <tag> externals/skia`. That commit is in the fork, on its `release/<SkiaSharp version>` branch, **not** in `google/skia`. The local clone carries the fork as the remote `mono`; if the commit is missing, run `git -C <skia> fetch mono release/<version>:refs/remotes/mono/release/<version>` |
| HarfBuzz | `harfbuzz/harfbuzz` | `harfbuzz` | The `third_party/externals/harfbuzz` commit in Skia's `DEPS` file at the pinned Skia commit |

## Finding the Clones

- On our computers the clones usually sit in `C:\repos\<usual folder>`. On another
  computer the parent folder can differ: look in `C:\repos` first, then beside the
  GnollHack checkout.
- Confirm a folder with `git -C <folder> remote get-url origin` before trusting it.
- If a repository is not found, say so and ask the user where it is. **Do not clone it
  yourself**: these are large downloads, and that is the user's decision.

## Reading the Shipped Version

- Find the version from the table, then the tag: `git -C <repo> tag --list '<pattern>'`.
- If the tag is missing, the clone is older than the release: run
  `git -C <repo> fetch --tags`, which leaves the working tree alone, look again, and say
  that you fetched.
- Read at the tag without changing the checkout:
  `git -C <repo> show <tag>:<path>`, `git -C <repo> grep -n <pattern> <tag> -- <path>`,
  `git -C <repo> log --oneline <tag> -- <path>`.
- **Never check out, pull, reset, commit, or create branches** in these clones. They are
  shared references, and some sit on a branch the user chose deliberately.
- Reading the default branch instead is fine for questions about newer behavior. Say
  which you read.

## Ours or Theirs

- Follow the trail from the last GnollHack frame into the framework, and cite the
  repository, the tag, and the file you rely on.
- Before calling it a framework bug, search that repository's tests and issues for the
  same message or behavior. An existing test case or issue is strong evidence.
- It is the framework's bug only when its code, at the shipped version, fails on valid
  use. If GnollHack's use is outside the framework's contract, the bug is ours.
- A framework bug gets two answers: a workaround in GnollHack, with a comment naming the
  upstream issue, and a report upstream with a minimal reproduction. An issue draft is a
  document; deliver it as `agent-implementation-planning` specifies.

## Related Skills

- **`sentry_crash_analysis`** — when a crash leads into framework code
- **`maui_frontend`** — the MAUI, SkiaSharp and native-bridge layers these frameworks
  sit under
