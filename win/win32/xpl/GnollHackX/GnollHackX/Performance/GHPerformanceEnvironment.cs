using System;
using System.Collections.Generic;
using System.Globalization;
using System.Reflection;
#if GNH_MAUI
using GnollHackM;
using Microsoft.Maui.Devices;
using Microsoft.Maui.Storage;
#else
using Xamarin.Essentials;
#endif

namespace GnollHackX.Performance
{
    /* Snapshot of the environment a performance run was measured in: the device, the
       versions, the display and the feature-toggle vector in effect. */
    public struct GHPerformanceEnvironmentFacts
    {
        public string Platform;
        public string DeviceId;
        public string DeviceModel;
        public string DeviceOs;
        public string AppVersion;
        public string RuntimeVersion;
        public string FrameworkVersion;
        public string UiFrameworkVersion;
        public string SkiaSharpVersion;
        public string FmodVersion;
        public string GpuBackend;
        public long GpuCacheSize;
        public bool MainCanvasUsesGpu;
        public string MapRefreshRateSetting;
        public double ReportedRefreshHz;
        public string BuildConfiguration;
        public string GitCommit;
        public Dictionary<string, object> Configuration;
        public Dictionary<string, string> Fingerprint;
    }

    /* Reads GHApp's static state into a GHPerformanceEnvironmentFacts snapshot. Every
       read is guarded, so a missing service or platform API leaves its field null or
       default rather than throwing. Must compile under C# 7.3. */
    public static class GHPerformanceEnvironment
    {
        public const string BuildSdkMetadataKey = "GHBuildSdkVersion";

        /* Assembly name prefixes left out of the component.* fingerprint keys */
        private static readonly string[] ExcludedComponentPrefixes =
        {
            "System", "mscorlib", "netstandard", "Microsoft.CSharp", "Microsoft.VisualBasic", "Microsoft.Win32", "GnollHack"
        };

        /* Component versions by assembly name; a loaded assembly's version never
           changes, so each is read once. */
        private static readonly object _componentLock = new object();
        private static readonly Dictionary<string, string> _componentVersions = new Dictionary<string, string>();

        public static GHPerformanceEnvironmentFacts Capture()
        {
            return CaptureFacts(false);
        }

        /* The environment fingerprint as Capture builds it, with the platform part
           re-read (WMI included) when refresh is true. Never throws. */
        public static Dictionary<string, string> CaptureFingerprint(bool refresh)
        {
            try
            {
                Dictionary<string, string> fingerprint = CaptureFacts(refresh).Fingerprint;
                return fingerprint != null ? fingerprint : new Dictionary<string, string>();
            }
            catch
            {
                return new Dictionary<string, string>();
            }
        }

        private static GHPerformanceEnvironmentFacts CaptureFacts(bool refreshFingerprint)
        {
            GHPerformanceEnvironmentFacts f = new GHPerformanceEnvironmentFacts();
            f.Configuration = new Dictionary<string, object>();

            try
            {
                f.Platform = GHApp.IsAndroid ? "Android" : GHApp.IsiOS ? "iOS" : GHApp.IsWindows ? "Windows" : GHApp.RuntimePlatform;
            }
            catch { }

            try
            {
                string manufacturer = DeviceInfo.Manufacturer;
                if (manufacturer != null && manufacturer.Length > 0)
                    manufacturer = manufacturer.Substring(0, 1).ToUpper() + manufacturer.Substring(1);
                f.DeviceModel = (manufacturer + " " + DeviceInfo.Model).Trim();
                f.DeviceOs = DeviceInfo.Platform + " " + DeviceInfo.VersionString;
                f.DeviceId = f.DeviceModel + " / " + f.DeviceOs;
            }
            catch { }

            try
            {
                f.AppVersion = GHApp.GHVersionString;
                f.RuntimeVersion = GHApp.RuntimeVersionString;
                f.FrameworkVersion = GHApp.FrameworkVersionString;
                f.UiFrameworkVersion = GHApp.UIFrameworkVersionString;
                f.SkiaSharpVersion = GHApp.SkiaSharpVersionString;
                f.FmodVersion = GHApp.FMODVersionString;
                f.GpuBackend = GHApp.GPUBackend;
                f.GpuCacheSize = GHApp.CurrentGPUCacheSize;
            }
            catch { }

            try
            {
                f.MainCanvasUsesGpu = GHApp.UseGPU;
            }
            catch { }

#if DEBUG
            f.BuildConfiguration = "Debug";
#else
            f.BuildConfiguration = "Release";
#endif
            f.GitCommit = ReadGitCommit();

            try
            {
                f.ReportedRefreshHz = GHApp.RoundedReconciledRefreshRate;
            }
            catch { }

            try
            {
                int value = Preferences.Get("MapRefreshRate", -1);
                MapRefreshRateStyle style = value < 0 ? UIUtils.GetDefaultMapFPS() : (MapRefreshRateStyle)value;
                f.MapRefreshRateSetting = style.ToString();
            }
            catch { }

            AddToggle(f.Configuration, "useTileBatching", () => GHApp.UseTileBatching);
            AddToggle(f.Configuration, "useTextBlobCaching", () => GHApp.UseTextBlobCaching);
            AddToggle(f.Configuration, "runtimeEffects", () => GHApp.RuntimeEffects);
            AddToggle(f.Configuration, "usePlatformRenderLoop", () => GHApp.UsePlatformRenderLoop);
            AddToggle(f.Configuration, "useMainGLCanvas", () => GHApp.UseGPU);
            AddToggle(f.Configuration, "useAuxiliaryGLCanvas", () => GHApp.UseAuxGPU);
            AddToggle(f.Configuration, "useMainMipMap", () => GHApp.UseMipMap);
            AddToggle(f.Configuration, "fixRects", () => GHApp.FixRects);
            AddToggle(f.Configuration, "fixFiltering", () => GHApp.FixFiltering);
            AddToggle(f.Configuration, "primaryGPUCacheLimit", () => GHApp.PrimaryGPUCacheLimit);
            AddToggle(f.Configuration, "secondaryGPUCacheLimit", () => GHApp.SecondaryGPUCacheLimit);
            AddToggle(f.Configuration, "frameTimeProfiler", () => FrameTimeProfiler.IsEnabled);
            AddToggle(f.Configuration, "screenLogging", () => GHApp.ScreenLogging);
            AddToggle(f.Configuration, "debugLogMessages", () => GHApp.DebugLogMessages);
            AddToggle(f.Configuration, "developerMode", () => GHApp.DeveloperMode);
            AddToggle(f.Configuration, "backgroundSampler", () => GHSystemLoadSampler.Enabled);

            f.Fingerprint = BuildFingerprint(f, refreshFingerprint);
            return f;
        }

        /* The fingerprint of a facts snapshot. The legacy mapping comes first, so that
           the keys it shares with older records carry identical values; every later key
           is added only when missing. */
        private static Dictionary<string, string> BuildFingerprint(GHPerformanceEnvironmentFacts f, bool refresh)
        {
            Dictionary<string, string> fp;
            try
            {
                fp = GHEnvironmentFingerprint.FromLegacyFields(f.AppVersion, f.GitCommit, f.BuildConfiguration,
                    f.RuntimeVersion, f.FrameworkVersion, f.UiFrameworkVersion, f.SkiaSharpVersion, f.FmodVersion,
                    f.Platform, f.DeviceOs, f.DeviceModel, f.MapRefreshRateSetting, f.GpuBackend, f.Configuration);
            }
            catch
            {
                fp = new Dictionary<string, string>();
            }

            fp[GHEnvironmentFingerprint.MetaFingerprintVersionKey] = GHEnvironmentFingerprint.FingerprintVersion;
            fp[GHEnvironmentFingerprint.MetaCapturedUtcKey] = DateTime.UtcNow.ToString("o", CultureInfo.InvariantCulture);

            AddIfMissing(fp, "code.portVersion", () => GHApp.GetPortVersionString());
            AddIfMissing(fp, "code.portBuild", () => GHApp.GetPortBuildString());
            AddIfMissing(fp, "code.renderSubscription", () => GHApp.RenderSubscriptionName);
            AddIfMissing(fp, "toolchain.compiler",
                () => GHApp.IsLLVM ? "LLVM" : GHApp.IsCoreCLR ? "Crossgen2" : GHApp.IsiOS ? "Clang" : GHApp.IsWindows ? "Standard" : "Mono AOT");
            AddIfMissing(fp, "toolchain.sdk", ReadBuildSdkVersion);
            AddIfMissing(fp, "toolchain.packaging", () => GHApp.IsPackaged ? "Packaged" : "Unpackaged");
            AddIfMissing(fp, GHEnvironmentFingerprint.SettingsKey("gpuCacheSize"), () => GHEnvironmentFingerprint.FormatValue(f.GpuCacheSize));
            AddIfMissing(fp, GHEnvironmentFingerprint.SettingsKey("mainCanvasUsesGpu"), () => GHEnvironmentFingerprint.FormatValue(f.MainCanvasUsesGpu));
            AddIfMissing(fp, GHEnvironmentFingerprint.SettingsKey("refreshHz"), () => GHEnvironmentFingerprint.FormatValue(f.ReportedRefreshHz));
            AddIfMissing(fp, GHEnvironmentFingerprint.SettingsKey("backgroundSampler"), () => GHEnvironmentFingerprint.FormatValue(GHSystemLoadSampler.Enabled));

            AddComponents(fp);
            AddIfMissing(fp, "component.native.skia", () => GHApp.SkiaVersionString);

            try
            {
                IPlatformService service = GHApp.PlatformService;
                if (service != null)
                {
                    Dictionary<string, string> platform = new Dictionary<string, string>();
                    service.AddEnvironmentFingerprint(platform, refresh);
                    GHEnvironmentFingerprint.MergeMissing(fp, platform);
                }
            }
            catch { }

            return fp;
        }

        /* Adds key unless present; a null or empty value, or a failing read, adds nothing. */
        private static void AddIfMissing(Dictionary<string, string> fp, string key, Func<string> read)
        {
            if (fp.ContainsKey(key))
                return;
            try
            {
                string value = read();
                if (!string.IsNullOrEmpty(value))
                    fp[key] = value;
            }
            catch { }
        }

        /* component.<AssemblyName> for every loaded, non-dynamic assembly outside
           ExcludedComponentPrefixes, added only when missing. */
        private static void AddComponents(Dictionary<string, string> fp)
        {
            try
            {
                Assembly[] assemblies = AppDomain.CurrentDomain.GetAssemblies();
                lock (_componentLock)
                {
                    for (int i = 0; i < assemblies.Length; i++)
                    {
                        try
                        {
                            Assembly assembly = assemblies[i];
                            if (assembly == null || assembly.IsDynamic)
                                continue;
                            AssemblyName assemblyName = assembly.GetName();
                            string name = assemblyName != null ? assemblyName.Name : null;
                            if (string.IsNullOrEmpty(name) || IsExcludedComponent(name))
                                continue;
                            string key = GHEnvironmentFingerprint.CategoryComponent + "." + name;
                            if (fp.ContainsKey(key))
                                continue;
                            string version;
                            if (!_componentVersions.TryGetValue(name, out version))
                            {
                                version = ReadComponentVersion(assembly, assemblyName);
                                _componentVersions[name] = version;
                            }
                            if (!string.IsNullOrEmpty(version))
                                fp[key] = version;
                        }
                        catch { }
                    }
                }
            }
            catch { }
        }

        private static bool IsExcludedComponent(string name)
        {
            for (int i = 0; i < ExcludedComponentPrefixes.Length; i++)
            {
                if (name.StartsWith(ExcludedComponentPrefixes[i], StringComparison.Ordinal))
                    return true;
            }
            return false;
        }

        /* The informational version up to its first '+', else the assembly version. */
        private static string ReadComponentVersion(Assembly assembly, AssemblyName assemblyName)
        {
            try
            {
                AssemblyInformationalVersionAttribute attribute = Attribute.GetCustomAttribute(
                    assembly, typeof(AssemblyInformationalVersionAttribute)) as AssemblyInformationalVersionAttribute;
                string version = attribute != null ? attribute.InformationalVersion : null;
                if (!string.IsNullOrEmpty(version))
                {
                    int plus = version.IndexOf('+');
                    if (plus > 0)
                        version = version.Substring(0, plus);
                    return version;
                }
            }
            catch { }
            try
            {
                return assemblyName.Version != null ? assemblyName.Version.ToString() : null;
            }
            catch
            {
                return null;
            }
        }

        /* The BuildSdkMetadataKey assembly metadata of the first loaded GnollHack
           assembly that carries it, or null. */
        private static string ReadBuildSdkVersion()
        {
            Assembly[] assemblies = AppDomain.CurrentDomain.GetAssemblies();
            for (int i = 0; i < assemblies.Length; i++)
            {
                try
                {
                    Assembly assembly = assemblies[i];
                    if (assembly == null || assembly.IsDynamic)
                        continue;
                    string name = assembly.GetName().Name;
                    if (name == null || !name.StartsWith("GnollHack", StringComparison.Ordinal))
                        continue;
                    object[] attributes = assembly.GetCustomAttributes(typeof(AssemblyMetadataAttribute), false);
                    for (int j = 0; j < attributes.Length; j++)
                    {
                        AssemblyMetadataAttribute metadata = attributes[j] as AssemblyMetadataAttribute;
                        if (metadata != null && metadata.Key == BuildSdkMetadataKey && !string.IsNullOrEmpty(metadata.Value))
                            return metadata.Value;
                    }
                }
                catch { }
            }
            return null;
        }

        /* The part of the assembly's informational version after its first '+' (the
           format `dotnet build`/MinVer-style versioning stamps a git commit in), or null
           when there is no '+' or no attribute. Never throws. */
        private static string ReadGitCommit()
        {
            try
            {
                AssemblyInformationalVersionAttribute attribute = Attribute.GetCustomAttribute(
                    typeof(GHPerformanceEnvironment).Assembly, typeof(AssemblyInformationalVersionAttribute))
                    as AssemblyInformationalVersionAttribute;
                string version = attribute != null ? attribute.InformationalVersion : null;
                if (string.IsNullOrEmpty(version))
                    return null;
                int plus = version.IndexOf('+');
                return plus >= 0 && plus + 1 < version.Length ? version.Substring(plus + 1) : null;
            }
            catch
            {
                return null;
            }
        }

        private static void AddToggle(Dictionary<string, object> configuration, string key, Func<object> read)
        {
            try
            {
                configuration[key] = read();
            }
            catch
            {
                configuration[key] = null;
            }
        }
    }
}
