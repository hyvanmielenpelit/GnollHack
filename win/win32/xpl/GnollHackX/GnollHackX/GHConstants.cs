using System;
using System.Collections.Generic;
using System.Globalization;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;

#if GNH_MAUI
namespace GnollHackM
#else
using Xamarin.Forms;

namespace GnollHackX
#endif
{
    public static class GHConstants
    {
        public const string GeneralTaskQueueDirectory = "task";
        public const string BonesDeleteFileNamePrefix = "bonesdelete_";
        public const string BonesDeleteFileNameSuffix = ".json";

        public const int MinimumFileDescriptorLimit = 16384;
        public const int CancelChar = 27;
        public const int SelfChar = 46;
        public const int InputBufferLength = 32;
        public const int BUFSZ = 256;
        public const int UTF8BUFSZ = 256 * 4;
        public const int MaxEngraveQuickTextLength = 95; /* PL_ESIZ */
        public const int MaxXlogUserNameLength = 31;        
        public const int MaxGHWindows = 32;
        public const int MapCols = 80;
        public const int MapRows = 21;        
        public const int DelayOutputDurationInMilliseconds = 50;
        public const int ExitWindowsWithStringDelay = 1100;
        public const int FadeToBlackDelay = 325;
        public const int FadeToBlackDuration = 200;
        public const int FadeFromBlackDuration = 200;
        public const int MaxMessageHistoryLength = 256;
        public const int MaxLongerMessageHistoryLength = 16384;
        public const float WindowBaseFontSize = 16f;
        public const float WindowMessageFontSize = 12f;
        public const float WindowMenuFontSize = 16f;
        public const float WindowStatusBarFontSize = 12f;
        public const float WindowMessageFontSizeMaxMultiplier = 1.40f;
        public const float StatusBarBaseFontSize = 13.5f;
        public const float StatusBarFontSizeMaxMultiplier = 1.55f;
        public const float StatusBarHorizontalMargin = 5.0f;
        public const float StatusBarVerticalMargin = 5.0f;
        public const float StatusBarRowMargin = 5.0f;
        public const float StatusBarShieldFontSize = StatusBarBaseFontSize * 32f / 42f;
        public const float StatusBarDifFontSize = StatusBarBaseFontSize * 24f / 42f;
        public const float StatusScreenWidthThresholdMultiplierPortrait = 0.675f;
        public const float StatusScreenWidthThresholdMultiplierLandscape = 0.55f;
        public const float SkillButtonBaseFontSize = 9.5f;
        public const float ContextButtonBottomStartMargin = 30.0f;
        public const double ContextButtonSpacing = 10.0;
        public const float ContextButtonBaseFontSize = 8.5f;
        public const float MoveDistanceThreshold = 25.0f;
        public const long MoveOrPressTimeThreshold = 200L; /* Milliseconds */
        public const long DoubleClickTimeThreshold = 500L; /* Milliseconds */
        public const long SwipeTimeThreshold = 300L; /* Milliseconds */
        public const long FreeScrollingTime = 200L; /* Milliseconds */
        public const long ScrollRecordThreshold = 200L; /* Milliseconds */
        public const float ScrollConstantDeceleration = 0.5f; /* pixels in % of screen height per second squared */
        public const float ScrollSpeedDeceleration = 2.0f; /* % of speed per second */
        public const float ScrollConstantDecelerationOverEdgeMultiplier = 10f; /* multiplier for ScrollSpeedDeceleration when over edge */
        public const float ScrollSpeedDecelerationOverEdgeMultiplier = 25f; /* % multiplier for ScrollSpeedDeceleration when over edge */
        public const float ScrollOverEdgeDeceleration = 64f; /* constant multiplying distance in pixels from edge */
        public const float ScrollDistanceAnchorFactor = 0.10f; /* % of screen height */
        public const float ScrollCloseAnchorFactor = 0.025f; /* % of screen height */
        public const float ScrollTargetSpeedAtDistanceAnchor = 0.25f; /* % of screen height per second */
        public const float ScrollTargetSpeedAtCloseAnchor = 0.15f; /* % of screen height per second */
        public const float ScrollTargetSpeedAtEdge = 0.05f; /* % of screen height per second */
        public const float ScrollDistanceEdgeConstant = 0.05f; /* pixels in % of screen height added to distance in pixels from edge to determine acceleration */
        public const float ScrollSpeedThreshold = 0.1f; /* pixels in % of screen height per second */
        public const float ScrollStretchLimit = 0.10f; /* pixels in % of screen height */
        public const float ScrollConstantStretch = 0.075f; /* pixels in % of screen height */
        public const double MoveByHoldingDownThreshold = 0.20; /* Seconds */
        public const double LongMenuTapThreshold = 0.75; /* Seconds */
        public const double KeyboardFocusDelay = 0.25; /* Seconds */        
        public const int TileWidth = 64;
        public const int TileHeight = 96;
        public const int StatusMarkWidth = 16;
        public const int StatusMarkHeight = 16;
        public const int MaxTileSheets = 4;
        public const int NumberOfTilesPerSheet = 8192; // 16224;
        public const int PowerOf2ForNumberOfTilesPerSheet = 13;
        public const int MaxTileSheetWidthInTiles = 128;
        public const int PowerOf2ForMaxTileSheetWidthInTiles = 7;
        public const int MaxSoundParameters = 10;
        public const int MaxFramesPerAnimation = 32;
        public const int NumPositionsInEnlargement = 5;
        public const int MaxTilesPerReplacement = 32;
        public const int MaxLeashed = 2;
        public const int MaxPlayedSpecialEffects = 12;
        public const int MaxPlayedZapAnimations = 16;
        public const int MaxNormalImmediateSoundInstances = 36;
        public const int MaxLongImmediateSoundInstances = 48;
        public const int MaxUISoundInstances = 8;
        public const int MaxChannels = 512;
        public const int MaxBitmapCacheSize = 64;
        public const int MaxDarkenedBitmapCacheSize = 256;
        public const int MaxDarkenedAutodrawBitmapCacheSize = 128;
        public const int MaxColorFilterCacheSize = 128;
        public const int PIT_BOTTOM_BORDER = 2;
        public const int SPECIAL_HEIGHT_IN_PIT = -32;
        public const int SPECIAL_HEIGHT_LEVITATION = 16;
        public const float OBJECT_PIT_SCALING_FACTOR = 0.75f;
        public const int NUM_BUFF_BIT_ULONGS = 8;
        public const int BUFFS_PER_TILE = 24;
        public const int MAX_PROPS = 194;
        public const int LAST_PROP = (MAX_PROPS - 1);
        public const int MaxObjectsDrawn = 12;
        public const int OBJECT_PILE_START_HEIGHT = 2;
        public const int OBJECT_PILE_HEIGHT_DIFFERENCE = 2;
        public const int MAX_UI_TILE_COMPONENTS = 32;
        public const int MAX_UI_TILE_16_x_16_COMPONENTS = 24;
        public const int MAX_UI_TILE_8_x_24_COMPONENTS = 32;
        public const float MinimumMapMiniRelativeFontSize = 0.5f;
        public const float MaximumMapMiniRelativeFontSize = 2.0f;
        public const float MinimumMapFontSize = 4.0f;
        public const float MaximumMapFontSize = 500.0f;
        public const float MapFontDefaultSize = 72.0f;
        public const float TileSizeAdjustmentModifier = 0.9f;        
        public const float MapFontRelativeAlternateSize = 7.0f / 16.0f;
        public const int DefaultMessageRows = 5;
        public const int AllMessageRows = 250;
        public const int DefaultPetRows = 2;
        public const int IntroGHSound = 1;
        public const string IntroEventPath = "event:/Music/Main Screen/Splash";
        public const int IntroBankId = 0;
        public const int MusicGHSound = 2;
        public const string MusicEventPath = "event:/Music/Start/Player Selection";
        public const float IntroMusicVolume = 0.5f;
        public const float BackgroundMusicVolume = 0.08f;
        public const int ButtonClickGHSound = 28;
        public const string ButtonClickEventPath = "event:/UI/Button Down";
        public const int ButtonClickBankId = 0;
        public const float ButtonClickVolume = 0.15f;
        public const int MenuSelectGHSound = 29;
        public const string MenuSelectEventPath = "event:/UI/Menu Select";
        public const int MenuSelectBankId = 0;
        public const float MenuSelectVolume = 0.15f;
#if DEBUG
        public const bool DefaultDeveloperMode = true;
        public const bool DefaultLogMessages = true;
        public const bool DefaultDebugPostChannel = true;
        public const bool DefaultPosting = true;
#else
        public const bool DefaultDeveloperMode = false;
        public const bool DefaultLogMessages = false;
        public const bool DefaultDebugPostChannel = false;
        public const bool DefaultPosting = false;
#endif
        public const int NUM_ZAP_SOURCE_BASE_DIRS = 8;
        public const int NUM_ZAP_SOURCE_DIRS = (GHConstants.NUM_ZAP_SOURCE_BASE_DIRS * 2);
        public const int NUM_CHAIN_SOURCE_DIRS = 4;
        public const int NUM_WORM_SOURCE_DIRS = 4;
        public const float ArrowButtonSize = 0.25f;
        public const float NumberButtonSize = 0.3f;
        public const float MoveArrowButtonSize = 0.35f;
        public const int MoreButtonPages = 4;
        public const int MoreButtonsPerRow = 4;
        public const int MoreButtonsPerColumn = 6;
        public const float BackgroundBorderDivisor = 8.0f;
        public const float BackgroundTopBorderExtraDivisor = 4.0f;
        public const bool DownloadFromWebInDebugMode = true;
        public const bool IsDefaultStatusBarClassic = false;
        public const bool AreMenuFadeEffectsDefault = true;
        public const string GnollHackGitHubPage = "https://github.com/hyvanmielenpelit/GnollHack";
        public const string GnollHackWebPage = "https://gnollhack.com";
        public const string GnollHackWikiPage = "https://wiki.gnollhack.com"; /* Old page on GitHub: "https://github.com/hyvanmielenpelit/GnollHack/wiki" */
        public const string GnollHackSponsorPage = "https://hyvanmielenpelit.fi/tule-mukaan/pienkannatusjaseneksi/in-english";
        public const string GnollHackGeneralDowngradePage = GnollHackWikiPage + "/Download";
        public const string GnollHackAndroidDowngradePage = GnollHackWikiPage + "/Android-Releases";
        public const string GnollHackiOSDowngradePage = GnollHackWikiPage + "/iOS-Releases";
        public const string GnollHackiGitHubApiPage = "https://api.github.com/repos/hyvanmielenpelit/GnollHack";
        public const string GnollHackGitHubStarPage = GnollHackWikiPage + "/Give-Us-a-Star-on-GitHub";
        public const string GHSettingsResourcePath = ".Assets.ghsettings.json";
        public const string GHSecretsResourcePath = ".Assets.ghsecrets.sjson";
        public const int MaxRefreshRate = 120;
        public const int PollingFrequency = 60;
        public const int PollingInterval = 15;
        public const int SavePollingTimeoutCount = 1500;
        public const double DefaultTextWindowMaxWidth = 600.0;
        public const double WindowHideIntervals = 5.0;
        public const uint MainCanvasAnimationTime = 2500000;
        public const uint AuxiliaryCanvasAnimationTime = 3600000;
        public const float MenuDefaultRowHeight = 30.0f;
        public const float DefaultGeneralVolume = 1.0f;
        public const float DefaultMusicVolume = 0.5f;
        public const float DefaultAmbientVolume = 0.5f;
        public const float DefaultDialogueVolume = 0.5f;
        public const float DefaultEffectsVolume = 0.5f;
        public const float DefaultUIVolume = 0.5f;
        public const bool DefaultMapNoClipMode = false;
        public const bool DefaultMapAlternateNoClipMode = false;
        public const bool DefaultZoomChangeCenterMode = false;
        public const bool DefaultHideNavigation = true;
        public const bool DefaultHideStatusBar = true;
#if DEBUG
        public const long StoreReviewRequestNumberOfGames = 0L;
        public const long StoreReviewRequestTotalPlayTime = 0L;
#else
        public const long StoreReviewRequestNumberOfGames = 4L;
        public const long StoreReviewRequestTotalPlayTime = 60L * 60L * 2L;
#endif
        public const bool EventAskForStarGazers = false;
#if DEBUG
        public const long StarGazerRequestNumberOfGames = 0L;
        public const long StarGazerRequestTotalPlayTime = 0L;
#else
        public const long StarGazerRequestNumberOfGames = 6L;
        public const long StarGazerRequestTotalPlayTime = 60L * 60L * 3L;
#endif

        public const bool DefaultHTMLDumpLogs = true;
        public const bool DefaultUseSingleDumpLog = true;
        public const int DefaultRightMouseCommand = (int)NhGetPosMods.DefClickRole;
        public const int DefaultMiddleMouseCommand = (int)NhGetPosMods.DefClickRole;
        //public const int RightMouseBitIndex = 22;
        //public const int MiddleMouseBitIndex = 27;
        public const string InstallTimePackName = "installtimepack";
        public const string OnDemandPackName = "ondemandpack";
        public const string UserDataDirectory = "usrdata";
        public const string UserDataFileName = "userdata.json";
        public const string AppLogDirectory = "log";
        public const string AppLogFileName = "ghlog.txt";
        public const string SaveDirectory = "save";
        public const string DumplogDirectory = "dumplog";
        public const string SnapshotDirectory = "snapshot";
        public const string ReplayDirectory = "replay";
        public const string ReplayDownloadFromCloudDirectory = "replay-cloud";
        public const string ArchiveDirectory = "archive"; /* Directory for sharable archives and files; cleaned and deleted at program start */
        public const string UploadDirectory = "upload"; /* Directory for uploadable files; files are never cleaned automatically, only upon successful upload */
        public const string TempDirectory = "temp";  /* Created and deleted on the go */
        public const string ZipDirectory = "zip";
        public const string UnzipTempSubDirectory = "temp";
        public const string UnzipZipSubDirectory = "zip";
        public const string GenericZipFileNameSuffix = ".zip";
        public const string SavedGameSharedZipFileNameSuffix = ".zip";
        public const string ManualFilePrefix = "manual_id_";
        public const string OracleMajorConsultationFilePrefix = "major_consultation_id_";
        public const string AchievementLongPrefix = "achievement_bits_";
        public const string GainedAchievementLongPrefix = "gained_achievement_bits_";
        public const string GainedAchievementTierKey = "gained_achievement_tier";
        public const bool DefaultReadStreamingBankToMemory = false;
        public const ulong AndroidBanksToMemoryThreshold = 3500000000UL;
        public const bool DefaultCopyStreamingBankToDisk = false;
        public const bool DefaultDrawWallEnds = true;
        public const bool DefaultBreatheAnimations = true;
        //public const bool DefaultShowPickNStashContextCommand = true;
        //public const bool DefaultShowPrevWepContextCommand = true;
        public const bool DefaultLighterDarkening = true;
        public const bool DefaultColoredXRayVision = true;
        public const bool DefaultAlternativeLayerDrawing = false;
        public const float CriticalBatteryChargeLevel = 0.15f;
        public const string ForumPostQueueDirectory = "forumpost";
        public const string ForumPostFileNamePrefix = "queued_forum_post_";
        public const string ForumPostFileNameSuffix = ".txt";
        public const string XlogPostQueueDirectory = "xlogpost";
        public const string XlogPostFileNamePrefix = "queued_xlog_post_";
        public const string XlogPostFileNameSuffix = ".txt";
        public const string XlogTopScorePage = "TopScores";
        public const string BonesPostQueueDirectory = "bonespost";
        public const string BonesPostFileNamePrefix = "queued_bones_post_";
        public const string BonesPostFileNameSuffix = ".txt";
        public const string BonesPostPage = "bones";
        public const double BonesPostBaseChance = 2.0 / 3.0;
        public const string ReplayPostQueueDirectory = "replaypost";
        public const string ReplayPostFileNamePrefix = "queued_replay_post_";
        public const string ReplayPostFileNameSuffix = ".txt";
        public const string SaveFileTrackingSuffix = ".ghsft";
        public const double MainScreenGeneralCounterIntervalInSeconds = 2.0;
        public const long MaxGHLogSize = 4194304L;
        public const int LineBuilderInitialCapacity = 256;
        public const ulong LowFreeDiskSpaceThresholdInBytes = 1536 * 1024 * 1024UL;
        public const ulong VeryLowFreeDiskSpaceThresholdInBytes = 1024 * 1024 * 1024UL;
        public const ulong CritiallyLowFreeDiskSpaceThresholdInBytes = 128 * 1024 * 1024UL;
        public const ulong DisableAuxGPUbyDefaultThresholdInBytes = 4096 * 1024 * 1024UL;
        public const ulong UseMipMapThresholdInBytes = 5120 * 1024 * 1024UL;
        public const long MaxSingleReplayFileSizeInBytes = 12 * 1024 * 1024L;
        public const string ReplayFileNamePrefix = "replay-";
        public const string ReplayContinuationFileNamePrefix = "rpcont-";
        public const string ReplayFileNameMiddleDivisor = "-";
        public const string ReplayFileContinuationNumberDivisor = "_";
        public const string ReplayFileNameSuffix = ".gnhrec";
        public const string ReplayZipFileNameSuffix = ".zip";
        public const string ReplayGZipFileNameSuffix = ".gz";
        public const string ReplaySharedZipFileNamePrefix = "shared-replay-";
        public const string ReplaySharedZipFileNameSuffix = ".zip";
        public const string ReplayAllSharedZipFileNamePrefix = "shared-replays-";
        public const string AzureBlobStorageReplayContainerNamePrefix = "replays";
        public const string TransferTempDirectory = "transfer_temp";
        public const string TransferUploadDirectory = "transfer_upload";
        public const string TransferDownloadDirectory = "transfer_download";
        public const string AzureBlobStorageSaveTransferContainerNamePrefix = "savetransfers";
        /* Transfer log session-directory retention policy */
        public const int TransferLogRetentionDays = 30;         /* Normal max age for kept session dirs */
        public const int TransferLogShortRetentionDays = 7;     /* Max age under moderate disk pressure */
        public const int TransferLogMaxKeepCount = 5;           /* Max sessions kept under disk pressure */
        /* Disk-space thresholds that tighten the retention policy (bytes) */
        public const long TransferDiskPressureCriticalBytes = 1L * 1024 * 1024 * 1024;   /* <= 1 GB: delete all */
        public const long TransferDiskPressureHighBytes = 2L * 1024 * 1024 * 1024;       /* <= 2 GB: keep 1 */
        public const long TransferDiskPressureMediumBytes = 5L * 1024 * 1024 * 1024;     /* <= 5 GB: keep 5, 7-day limit */
        public const long TransferDiskPressureLowBytes = 10L * 1024 * 1024 * 1024;       /* <= 10 GB: keep 5, 30-day limit */

        public const string AzureBlobStorageDelimiter = "/";
        public const string AzureBlobStorageGeneralDirectoryName = "_Anonymous";
        public const bool GZipIsDefaultReplayCompression = true;
        public const int ReplayStandardDelay = 128; /* Milliseconds */
        public const int ReplayGetEventDelay = 16; /* Milliseconds */
        public const int ReplayOutripDelay = 1024; /* Milliseconds */
        public const int ReplayAskNameDelay1 = 512; /* Milliseconds */
        public const int ReplayAskNameDelay2 = 1536; /* Milliseconds */
        public const int ReplayPopupDelay = 512; /* Milliseconds */
        public const int ReplayYnDelay = 512; /* Milliseconds */
        public const int ReplayMenuDelay = 512; /* Milliseconds */
        public const int ReplayGetLineDelay1 = 512; /* Milliseconds */
        public const int ReplayGetLineDelay2 = 1024; /* Milliseconds */
        public const int ReplayDisplayWindowDelay = 512; /* Milliseconds */
        //public const GHlong GPUResourceCacheSize = 800000000L;
        public const string AssetsTilesetDirectory = "tileset";
        public const string AssetsBanksDirectory = "banks";
        public const double StartUpTimeOut = 8.0;

#if GNH_MAUI
        public const string PortName = "GnollHackM";
        public const UnmanagedType GHStringMarshallingType = UnmanagedType.LPUTF8Str;
        public const float HoveringColorMultiplier = 0.24f;
        public const float NonHoveringColorMultiplier = 0.88f;
#else
        public const string PortName = "GnollHackX";
        public const UnmanagedType GHStringMarshallingType = UnmanagedType.LPStr;
        public const float HoveringColorMultiplier = 0.24f;
        public const double NonHoveringColorMultiplier = 0.88;
#endif
#if WINDOWS
        public const bool DefaultSimpleCmdLayout = false;
        public const int FadeFromBlackDurationAtStart = 700;
#if DEBUG
        public const double FadeFromBlackAtStartExtraDelaySecs = 0.75;
#else
        public const double FadeFromBlackAtStartExtraDelaySecs = 0.50;
#endif
#else
        public const bool DefaultSimpleCmdLayout = true;
        public const int FadeFromBlackDurationAtStart = 700;
        public const double FadeFromBlackAtStartExtraDelaySecs = 0.30;
#endif
        public const bool DefaultRuntimeEffects = false;
        public const bool DefaultCharacterClickAction = false;
        public const bool DefaultDiceAsRanges = true;
        public const bool DefaultDamageFormula = false;
        public const bool DefaultWornShowsEquipment = true;
        public const bool DefaultAutoDig = true;
        public const bool DefaultIgnoreStopping = false; /* Since travel is also used for normal movement in the modern version */
        public const bool DefaultMetricSystem = false;
        public const long MapDataLockTimeOutTicks = 100L;
        public const long EffectLockTimeOutTicks = 50L;
        public const long MessageLockTimeOutTicks = 50L;
        public const uint TargetDesktopScreenWidth = 1920;
        public const double WideLandscapeThreshold = 1.96; /* Does not yet capture 16:9 display + window title bar above + Windows system bar below  */
#if GNH_MAUI && (WINDOWS || ANDROID || IOS)
        public const bool IsPlatformRenderLoopDefault = true;
#else
        public const bool IsPlatformRenderLoopDefault = false;
#endif
        public const bool AllowRestartGameUponActivityDestruction = true;
        public const string SentryGnollHackGeneralCategoryName = "GnollHack Information";
        public const string SentryGnollHackLibraryCategoryName = "GnollHack Library";
        public const string SentryGnollHackCallbackCategoryName = "GnollHack Callback";
        public const string SentryGnollHackButtonClickCategoryName = "Button Clicked";
        public const string SAVE_GAME_NOTIFICATION_CHANNEL_ID = "save_game_channel";
        public const string SingleCommandPageTitle = "Commands";
        public const int DefaultMoreButtonListSize = MoreButtonPages * MoreButtonsPerRow * MoreButtonsPerColumn + 1;
        public const float KeyboardShortcutRelativeFontSize = 0.9f;
        public const float TextRowMultiplierWithKeyboardShortcuts = 1.0f + KeyboardShortcutRelativeFontSize;
        public const GHSpecialKey GameMenuKeyboardShortcut = GHSpecialKey.F10;
        public const GHSpecialKey CancelKeyboardShortcut = GHSpecialKey.Escape;
        public const bool DefaultShowPolearmContextButton = true;
        public const ulong DiskSpaceSuperCriticalThresholdInBytes = 512UL * 1024UL * 1024UL;
        public const ulong DiskSpaceCriticalThresholdInBytes = 1024UL * 1024UL * 1024UL;
        public const ulong DiskSpaceLowThresholdInBytes = 5 * 1024UL * 1024UL * 1024UL;
        public const bool EnableExperimentalFeatures = false;
        public const int NumGuiAchievementLongs = ((int)gui_achievement_types.NUM_GUI_ACHIEVEMENTS - 1) / 64 + 1;
        public const ulong DefaultMetalGPUCacheSize = 256UL * 1024UL * 1024UL;
        public const ulong MaxMetalGPUCacheSize = 384UL * 1024UL * 1024UL;
    }

}
