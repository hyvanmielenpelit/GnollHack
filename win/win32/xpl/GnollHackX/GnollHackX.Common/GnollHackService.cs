using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;
using System.IO;
#if GNH_MAUI
using GnollHackM;
#else
using Xamarin.Forms;
using Xamarin.Essentials;
#endif
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using GnollHackX;

#if __IOS__
using Foundation;
using UIKit;

#if GNH_MAUI
namespace GnollHackM
#else
[assembly: Dependency(typeof(GnollHackX.iOS.GnollHackService))]
namespace GnollHackX.iOS
#endif
#elif __ANDROID__                   
using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using Android.Content.Res;

#if GNH_MAUI
namespace GnollHackM
#else
[assembly: Dependency(typeof(GnollHackX.Droid.GnollHackService))]
namespace GnollHackX.Droid
#endif
#else
#if GNH_MAUI
namespace GnollHackM
#else
[assembly: Dependency(typeof(GnollHackX.Unknown.GnollHackService))]
namespace GnollHackX.Unknown
#endif
#endif
{
    public class PlatformConstants
    {
#if __IOS__ || (GNH_MAUI && IOS)
        public const string dll     = "__Internal";
        public const string library = "gnollhackios";
#elif __ANDROID__ || (GNH_MAUI && ANDROID)
        public const string dll     = @"libgnollhackdroid.so";
        public const string library = "gnollhackdroid";
#elif GNH_MAUI && WINDOWS
        public const string dll = @"gnollhackwin.dll";
        public const string library = "gnollhackwin";
#else
        public const string dll     = @"libgnollhackunknown.so";
        public const string library = "gnollhackunknown";
#endif
    }

    class GnollHackService : IGnollHackService
    {
        [DllImport(PlatformConstants.dll)]
        public static extern int RunGnollHack(
            [MarshalAs(UnmanagedType.LPStr)] string gnhdir,
            [MarshalAs(UnmanagedType.LPStr)] string cmdlineargs,
            [MarshalAs(UnmanagedType.LPStr)] string preset_player_name,
            [MarshalAs(UnmanagedType.LPStr)] string recovery_name,
            ulong runflags,
            ulong wincaps1,
            ulong wincaps2,
            VoidVoidCallback callback_init_nhwindows,
            IntVoidCallback callback_player_selection,
            AskNameCallback callback_askname,
            VoidVoidCallback callback_get_nh_event,
            VoidConstCharCallback callback_exit_nhwindows,
            VoidConstCharCallback callback_suspend_nhwindows,
            VoidVoidCallback callback_resume_nhwindows,
            CreateGHWindowCallback callback_create_nhwindow_ex,
            VoidIntCallback callback_clear_nhwindow,
            DisplayWindowCallback callback_display_nhwindow,
            VoidIntCallback callback_destroy_nhwindow,
            CursCallback callback_curs,
            PutStrExColorCallback callback_putstr_ex,
            PutStrEx2ColorCallback callback_putstr_ex2,
            PutStrExColorCallback callback_putmixed_ex,
            DisplayFileCallback callback_display_file,
            StartMenuCallback callback_start_menu_ex,
            AddMenuCallback callback_add_menu,
            AddExtendedMenuCallback callback_add_extended_menu,
            EndMenuCallback callback_end_menu_ex,
            IntIntIntOutIntCallback callback_select_menu,
            VoidIntCallback callback_message_menu, /* no need for X-specific handling */
            VoidVoidCallback callback_update_inventory,
            VoidVoidCallback callback_mark_synch,
            VoidVoidCallback callback_wait_synch,
            /* If clipping is on */
            ClipAroundCallback callback_cliparound,
            /* If positionbar is on */
            UpdatePositionBarCallback callback_update_positionbar,
            PrintGlyphCallback callback_print_glyph,
            IssueGuiCommandCallback callback_issue_gui_command,
            VoidConstCharCallback callback_raw_print,
            VoidConstCharCallback callback_raw_print_bold,
            IntVoidCallback callback_nhgetch,
            NhPosKeyCallback callback_nh_poskey,
            VoidVoidCallback callback_nhbell,
            IntVoidCallback callback_doprev_message,
            YnFunctionCallback callback_yn_function_ex,
            GetLineCallback callback_getlin_ex,
            IntVoidCallback callback_get_ext_cmd,
            VoidIntCallback callback_number_pad,
            VoidVoidCallback callback_delay_output,
            VoidIntCallback callback_delay_output_milliseconds,
            VoidIntCallback callback_delay_output_intervals,

            VoidVoidCallback callback_change_color,
            VoidIntCallback callback_change_background,
            VoidIntCallback callback_set_font_name,
            CharVoidCallback callback_get_color_string,

            VoidVoidCallback callback_start_screen,
            VoidVoidCallback callback_end_screen,
            OutRipCallback callback_outrip,
            VoidConstCharCallback callback_preference_update,
            GetMsgHistoryCallback callback_getmsghistory,
            PutMsgHistoryCallback callback_putmsghistory,
            VoidIntCallback callback_status_init,
            VoidVoidCallback callback_status_finish,
            StatusEnableFieldCallback callback_status_enablefield,
            StatusUpdateCallback callback_status_update,
            CanSuspendYesCallback callback_can_suspend_yes,
            VoidVoidCallback callback_stretch_window,
            VoidUlongCallback callback_set_animation_timer_interval,
            OpenSpecialViewCallback callback_open_special_view,
            StopAllSoundsCallback callback_stop_all_sounds,
            PlayImmediateSoundCallback callback_play_immediate_ghsound,
            PlayOccupationAmbientCallback callback_play_ghsound_occupation_ambient,
            PlayEffectAmbientCallback callback_play_ghsound_effect_ambient,
            SetEffectAmbientVolumeCallback callback_set_effect_ambient_volume,
            PlayMusicCallback callback_play_ghsound_music,
            PlayLevelAmbientCallback callback_play_ghsound_level_ambient,
            PlayEnvironmentAmbientCallback callback_play_ghsound_environment_ambient,
            AdjustGeneralVolumesCallback callback_adjust_ghsound_general_volumes,
            AddAmbientSoundCallback callback_add_ambient_ghsound,
            DeleteAmbientSoundCallback callback_delete_ambient_ghsound,
            SetAmbientSoundVolumeCallback callback_set_ambient_ghsound_volume,
            VoidVoidCallback callback_clear_context_menu,
            AddContextMenuCallback callback_add_context_menu,
            UpdateStatusButtonCallback callback_update_status_button,
            ToggleAnimationTimerCallback callback_toggle_animation_timer,
            DisplayFloatingTextCallback callback_display_floating_text,
            DisplayScreenTextCallback callback_display_screen_text,
            DisplayPopupTextCallback callback_display_popup_text,
            DisplayGUIEffectCallback callback_display_gui_effect,
            UpdateCursorCallback callback_update_cursor,
            UIHasInputCallback callback_ui_has_input,
            VoidIntCallback callback_exit_hack,
            CharVoidCallback callback_getcwd,
            IntCharCharUintCallback callback_messagebox,

            FreeMemoryCallback callback_free_memory,
            ReportPlayerNameCallback callback_report_player_name,
            ReportPlayTimeCallback callback_report_play_time,
            SendObjectDataCallback callback_send_object_data,
            SendMonsterDataCallback callback_send_monster_data,
            SendEngravingDataCallback callback_send_engraving_data
        );

        [DllImport(PlatformConstants.dll)]
        public static extern int GetGlyph2Tile(out IntPtr array_ptr, out int size);
        [DllImport(PlatformConstants.dll)]
        public static extern int GetGlyphTileFlags(out IntPtr array_ptr, out int size);
        [DllImport(PlatformConstants.dll)]
        public static extern int GetTile2Animation(out IntPtr array_ptr, out int size);
        [DllImport(PlatformConstants.dll)]
        public static extern int GetTile2Enlargement(out IntPtr array_ptr, out int size);
        [DllImport(PlatformConstants.dll)]
        public static extern int GetTile2Autodraw(out IntPtr array_ptr, out int size);
        [DllImport(PlatformConstants.dll)]
        public static extern int GetAnimationOffsets(out IntPtr array_ptr, out int size);
        [DllImport(PlatformConstants.dll)]
        public static extern int GetEnlargementOffsets(out IntPtr array_ptr, out int size);
        [DllImport(PlatformConstants.dll)]
        public static extern int GetReplacementOffsets(out IntPtr array_ptr, out int size);
        [DllImport(PlatformConstants.dll)]
        public static extern int CountTotalTiles();
        [DllImport(PlatformConstants.dll)]
        public static extern void LibSetGlyphArrays(IntPtr ptr_gl2ti, int size_gl2ti, IntPtr ptr_gltifl, int size_gltifl);
        [DllImport(PlatformConstants.dll)]
        public static extern void LibSetTile2AnimationArray(IntPtr ptr_ti2an, int size_ti2an);
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetUnexploredGlyph();
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetNoGlyph();
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetAnimationOff();
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetEnlargementOff();
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetReplacementOff();
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetGeneralTileOff();
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetHitTileOff();
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetUITileOff();
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetSpellTileOff();
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetSkillTileOff();
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetCommandTileOff();
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetBuffTileOff();
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetCursorOff();

        [DllImport(PlatformConstants.dll)]
        public static extern int LibTest();
        [DllImport(PlatformConstants.dll)]
        public static extern IntPtr LibGetVersionString();

        [DllImport(PlatformConstants.dll)]
        public static extern IntPtr LibGetVersionId();
        [DllImport(PlatformConstants.dll)]
        public static extern ulong LibGetVersionNumber();
        [DllImport(PlatformConstants.dll)]
        public static extern ulong LibGetVersionCompatibility();
        [DllImport(PlatformConstants.dll)]
        public static extern IntPtr LibGetPropertyName(int prop_index);
        [DllImport(PlatformConstants.dll)]
        public static extern IntPtr LibGetExtendedCommands();
        [DllImport(PlatformConstants.dll)]
        public static extern IntPtr LibDumplogDateString(long startdate);

        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetAnimationArraySize();
        [DllImport(PlatformConstants.dll)]
        public static extern AnimationDefinition LibGetAnimationArrayElement(int idx);
        [DllImport(PlatformConstants.dll)]
        public static extern EnlargementDefinition LibGetEnlargementArrayElement(int idx);
        [DllImport(PlatformConstants.dll)]
        public static extern ReplacementDefinition LibGetReplacementArrayElement(int idx);
        [DllImport(PlatformConstants.dll)]
        public static extern AutoDrawDefinition LibGetAutoDrawArrayElement(int idx);
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetEnlargementArraySize();
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetReplacementArraySize();
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetAutoDrawArraySize();

        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetAutoDrawArraySize(int glyph);

        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetTileAnimationIndexFromGlyph(int glyph);


        [DllImport(PlatformConstants.dll)]
        public static extern byte LibGlyphIsExplosion(int glyph);
        [DllImport(PlatformConstants.dll)]
        public static extern byte LibGlyphIsZap(int glyph);
        [DllImport(PlatformConstants.dll)]
        public static extern byte LibGlyphIsAnyDying(int glyph);
        [DllImport(PlatformConstants.dll)]
        public static extern int LibMaybeGetAnimatedTile(int ntile, int tile_animation_idx, int play_type, long interval_counter, 
            out int frame_idx_ptr, out int main_tile_idx_ptr, out sbyte mapAnimated, ref int autodraw_ptr);
        [DllImport(PlatformConstants.dll)]
        public static extern int LibZapGlyphToCornerGlyph(int adjglyph, ulong adjflags, int source_dir);
        [DllImport(PlatformConstants.dll)]
        public static extern void LibSetPetMID(uint m_id);
        [DllImport(PlatformConstants.dll)]
        public static extern int LibChmod(string filename, uint mode);
        [DllImport(PlatformConstants.dll)]
        public static extern void LibSaveAndRestoreSavedGame(int save_style);
        [DllImport(PlatformConstants.dll)]
        public static extern void LibTallyRealTime();
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetMaxManuals();
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetFirstCatalogue();
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetNumCatalogues();
        [DllImport(PlatformConstants.dll)]
        public static extern int LibIsDebug();
        [DllImport(PlatformConstants.dll)]
        public static extern int LibValidateSaveFile(string filename, [MarshalAs(UnmanagedType.LPArray), Out] byte[] out_buffer);
        [DllImport(PlatformConstants.dll)]
        public static extern int LibCheckCurrentFileDescriptor(string dir);
        [DllImport(PlatformConstants.dll)]
        public static extern int LibReportFileDescriptors();
        [DllImport(PlatformConstants.dll)]
        public static extern int LibIncreaseFileDescriptorLimitToAtLeast(int min_cur);
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetFileDescriptorLimit(int is_max_limit);
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetCharacterClickAction();
        [DllImport(PlatformConstants.dll)]
        public static extern void LibSetCharacterClickAction(int new_value);
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetMouseCommand(int is_middle);
        [DllImport(PlatformConstants.dll)]
        public static extern void LibSetMouseCommand(int new_value, int is_middle);

        [DllImport(PlatformConstants.dll)]
        public static extern IntPtr LibGetEventPathForGHSound(int ghsound);
        [DllImport(PlatformConstants.dll)]
        public static extern float LibGetVolumeForGHSound(int ghsound);


        private void LoadNativeLibrary(string libName)
        {
#if __ANDROID__
            Java.Lang.JavaSystem.LoadLibrary(libName);
#endif
        }
        private void UnloadNativeLibrary(string libName)
        {
#if __ANDROID__
            Java.Lang.JavaSystem.Gc();
#endif
        }

        public void LoadLibrary()
        {
            LoadNativeLibrary(PlatformConstants.library);
        }
        public void UnloadLibrary()
        {
            UnloadNativeLibrary(PlatformConstants.library);
        }
        public void ClearFiles()
        {
            ClearCoreFiles();
            ClearTopScores();
            ClearSavedGames();
            ClearDumplogs();
        }

        public void ClearAllFilesInMainDirectory()
        {
            string filesdir = GetGnollHackPath();
            DirectoryInfo di = new DirectoryInfo(filesdir);
            foreach (FileInfo file in di.GetFiles())
            {
                file.Delete();
            }
        }

        public void ClearCoreFiles()
        {
            string filesdir = GetGnollHackPath();
            DirectoryInfo di = new DirectoryInfo(filesdir);
            foreach (FileInfo file in di.GetFiles())
            {
                bool found = false;
                foreach(string txtfile in _txtfileslist)
                {
                    if(file.Name == txtfile)
                    {
                        found = true;
                        break;
                    }
                }
                if(!found)
                {
                    foreach (string binfile in _binfileslist)
                    {
                        if (file.Name == binfile)
                        {
                            found = true;
                            break;
                        }
                    }
                }
                if (found)
                {
                    if (file.Name != "logfile" && file.Name != "xlogfile")
                        file.Delete();
                }
            }
        }

        public void ClearTopScores()
        {
            string filesdir = GetGnollHackPath();
            DirectoryInfo di = new DirectoryInfo(filesdir);
            foreach (FileInfo file in di.GetFiles())
            {
                if (file.Name == "logfile" || file.Name == "xlogfile")
                    file.Delete();
            }
        }

        public void ClearSavedGames()
        {
            string filesdir = GetGnollHackPath();
            string fulldirepath = Path.Combine(filesdir, GHConstants.SaveDirectory);
            if (Directory.Exists(fulldirepath))
            {
                DirectoryInfo disave = new DirectoryInfo(fulldirepath);
                foreach (FileInfo file in disave.GetFiles())
                {
                    file.Delete();
                }
            }
        }

        public void ClearDumplogs()
        {
            string filesdir = GetGnollHackPath();
            string fulldirepath = Path.Combine(filesdir, GHConstants.DumplogDirectory);
            if (Directory.Exists(fulldirepath))
            {
                DirectoryInfo disave = new DirectoryInfo(fulldirepath);
                foreach (FileInfo file in disave.GetFiles())
                {
                    file.Delete();
                }
            }
        }

        public void ClearBones()
        {
            string filesdir = GetGnollHackPath();
            DirectoryInfo di = new DirectoryInfo(filesdir);
            foreach (FileInfo file in di.GetFiles())
            {
                if(file.Name.Length > 4 && file.Name.Substring(0, 4) == "bon-")
                {
                    file.Delete();
                }
            }
        }

        public async Task ResetDefaultsFile()
        {
            try
            {
                string content;
                string filesdir = GetGnollHackPath();
                string assetsourcedir = "gnh";
                string txtfile = "defaults.gnh";
#if __IOS__
                string fullsourcepath = NSBundle.MainBundle.PathForResource("defaults", "gnh", assetsourcedir);
                using (StreamReader sr = new StreamReader(fullsourcepath))
#elif __ANDROID__
                string fullsourcepath = Path.Combine(assetsourcedir, txtfile);
                AssetManager assets = MainActivity.StaticAssets;
                using Stream assetsStream = assets.Open(fullsourcepath);
                using (StreamReader sr = new StreamReader(assetsStream))
#elif WINDOWS
                string fullsourcepath = Path.Combine(assetsourcedir, txtfile);
                using Stream fileStream = await FileSystem.Current.OpenAppPackageFileAsync(fullsourcepath);
                using (StreamReader sr = new StreamReader(fullsourcepath))
#else
                string fullsourcepath = Path.Combine(assetsourcedir, txtfile);
                using (StreamReader sr = new StreamReader(fullsourcepath))
#endif
                {
                    content = sr.ReadToEnd();
                }
                string fulltargetpath = Path.Combine(filesdir, txtfile);
                if (File.Exists(fulltargetpath))
                {
                    File.Delete(fulltargetpath);
                }
                using (StreamWriter sw = new StreamWriter(fulltargetpath))
                {
                    sw.Write(content);
                }
            }
            catch (Exception)
            {
                await Task.Delay(5);
            }
        }

        public string GetGnollHackPath()
        {
#if __ANDROID__
            try
            {
                string path = Android.App.Application.Context.FilesDir.Path;
                if (string.IsNullOrWhiteSpace(path))
                    return ".";
                else
                    return path;
            }
            catch
            {
                return ".";
            }
#elif __IOS__
            try
            {
                string documents = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
                string library = Path.Combine(documents, "..", "Library");
                string path = Path.Combine(library, "GnollHackData");
                if (!Directory.Exists(path))
                    GHApp.CheckCreateDirectory(path);
                return path;
            }
            catch
            {
                return ".";
            }
#elif WINDOWS
            return FileSystem.Current.AppDataDirectory;
#else
            return ".";
#endif
        }


        private string[] _txtfileslist = { "xcredits", "license", "logfile", "perm", "record", "symbols", "sysconf", "xlogfile", "defaults.gnh" };
        private string[] _binfileslist = { "nhdat" };

        public async Task InitializeGnollHack()
        {
            /* Unpack GnollHack files */
            /* Add a check whether to unpack if there are existing files or not */
            try
            {
                string filesdir = GetGnollHackPath();

                /* For debugging purposes now, delete all existing files in filesdir first */
                //System.IO.DirectoryInfo di = new DirectoryInfo(filesdir);

                //foreach (FileInfo file in di.GetFiles())
                //{
                //    file.Delete();
                //}

                /* Make relevant directories */
                string[] ghdirlist = { GHConstants.SaveDirectory, GHConstants.DumplogDirectory };
                foreach (string ghdir in ghdirlist)
                {
                    string fulldirepath = Path.Combine(filesdir, ghdir);
                    GHApp.CheckCreateDirectory(fulldirepath);
                }

                /* Copy missing files from resources */
                string content;
                string assetsourcedir = "gnh";
                //#if GNH_MAUI
                //#if __ANDROID__
                //            assetsourcedir = Path.Combine("Platforms", "Android", assetsourcedir);
                //#elif __IOS__
                //            assetsourcedir = Path.Combine("Platforms", "iOS", assetsourcedir);
                //#else                
                //            assetsourcedir = Path.Combine("Platforms", "Unknown", assetsourcedir);
                //#endif
                //#endif
#if __ANDROID__
                AssetManager assets = MainActivity.StaticAssets;
#endif

                foreach (string txtfile in _txtfileslist)
                {
#if __IOS__
                string extension = Path.GetExtension(txtfile);
                if (extension != null && extension.Length > 0)
                    extension = extension.Substring(1); /* Remove . from the start */
                string fname = Path.GetFileNameWithoutExtension(txtfile);
                string fullsourcepath = NSBundle.MainBundle.PathForResource(fname, extension, assetsourcedir);
                using (StreamReader sr = new StreamReader(fullsourcepath))
#elif __ANDROID__
                    string fullsourcepath = Path.Combine(assetsourcedir, txtfile);
                    using Stream assetsStream = assets.Open(fullsourcepath);
                    using (StreamReader sr = new StreamReader(assetsStream))
#elif WINDOWS
                string fullsourcepath = Path.Combine(assetsourcedir, txtfile);
                using Stream fileStream = await FileSystem.Current.OpenAppPackageFileAsync(fullsourcepath);
                using (StreamReader sr = new StreamReader(fileStream))
#else
                string fullsourcepath = Path.Combine(filesdir, assetsourcedir, txtfile);
                using (StreamReader sr = new StreamReader(fullsourcepath))
#endif
                    {
                        content = sr.ReadToEnd();
                    }
                    string fulltargetpath = Path.Combine(filesdir, txtfile);
                    if (File.Exists(fulltargetpath))
                    {
                        continue;
                        //File.Delete(fulltargetpath);
                    }

                    using (StreamWriter sw = new StreamWriter(fulltargetpath))
                    {
                        sw.Write(content);
                    }
                }

                byte[] data;
                int maxsize = 2048 * 1024;
                foreach (string binfile in _binfileslist)
                {
#if __IOS__
                string extension = Path.GetExtension(binfile);
                if (extension != null && extension.Length > 0)
                    extension = extension.Substring(1); /* Remove . from the start */
                string fname = Path.GetFileNameWithoutExtension(binfile);
                string fullsourcepath = NSBundle.MainBundle.PathForResource(fname, extension, assetsourcedir);
                using (BinaryReader br = new BinaryReader(File.OpenRead(fullsourcepath)))
#elif __ANDROID__
                    string fullsourcepath = Path.Combine(assetsourcedir, binfile);
                    using (BinaryReader br = new BinaryReader(assets.Open(fullsourcepath)))
#elif WINDOWS
                string fullsourcepath = Path.Combine(assetsourcedir, binfile);
                using Stream fileStream = await FileSystem.Current.OpenAppPackageFileAsync(fullsourcepath);
                using (BinaryReader br = new BinaryReader(fileStream))
#else
                string fullsourcepath = Path.Combine(assetsourcedir, binfile);
                using (BinaryReader br = new BinaryReader(File.OpenRead(fullsourcepath)))
#endif
                    {
                        data = br.ReadBytes(maxsize);
                    }

                    string fulltargetpath = Path.Combine(filesdir, binfile);
                    if (File.Exists(fulltargetpath))
                    {
                        /* Should check whether the current one is an up-to-date version; assume for now that it is not */
                        File.Delete(fulltargetpath);
                        //continue;
                    }

                    using (BinaryWriter sw = new BinaryWriter(File.Open(fulltargetpath, FileMode.Create)))
                    {
                        sw.Write(data);
                    }
                }
            }
            catch (Exception)
            {
                await Task.Delay(5);
            }
        }

        public async Task InitializeSecrets(Secrets secrets)
        {
            if (secrets == null)
                return;

            string filesdir = GetGnollHackPath();
#if __ANDROID__
            AssetManager assets = MainActivity.StaticAssets;
#endif
            foreach (SecretsDirectory sdir in secrets.directories)
            {
                string fulldirepath = Path.Combine(filesdir, sdir.name);
                if (!Directory.Exists(fulldirepath))
                {
                    GHApp.CheckCreateDirectory(fulldirepath);
                }
                else
                {
                    int res = Chmod(fulldirepath, (uint)ChmodPermissions.S_IALL);
                }
            }
            //int packfilemaxsize = 512 * 1024 * 1024;
            foreach (SecretsFile sfile in secrets.files)
            {
                string assetfile = sfile.name;
                string sfiledir = sfile.source_directory;
                //#if GNH_MAUI
                //#if __IOS__
                //                sfiledir = Path.Combine("Platforms", "iOS", sfiledir);
                //#elif __ANDROID__
                //                sfiledir = Path.Combine("Platforms", "Android", sfiledir);
                //#else
                //                sfiledir = Path.Combine("Platforms", "Unknown", sfiledir);
                //#endif
                //#endif
#if __IOS__
                string extension = Path.GetExtension(assetfile);
                if (extension != null && extension.Length > 0)
                    extension = extension.Substring(1); /* Remove . from the start */
                string fname = Path.GetFileNameWithoutExtension(assetfile);
                string fullsourcepath = NSBundle.MainBundle.PathForResource(fname, extension, sfiledir);
#elif __ANDROID__
                string fullsourcepath = Path.Combine(sfiledir, assetfile);
#elif WINDOWS
                string fullsourcepath = Path.Combine(sfiledir, assetfile);
#else
                string fullsourcepath = Path.Combine(sfiledir, assetfile);
#endif
                try
                {
                    string fulltargetpath = Path.Combine(filesdir, sfile.target_directory, assetfile);
                    if (File.Exists(fulltargetpath))
                    {
                        FileInfo curfile = new FileInfo(fulltargetpath);
                        long curlength = curfile.Length;
                        string curfileversion = Preferences.Get("Verify_" + sfile.id + "_Version", "");
                        DateTime moddate = Preferences.Get("Verify_" + sfile.id + "_LastWriteTime", DateTime.MinValue);
                        long used_length = GHApp.IsDesktop ? sfile.length_desktop : sfile.length_mobile;

                        if (curlength != used_length || curfileversion != sfile.version || moddate != curfile.LastWriteTimeUtc)
                        {
                            File.Delete(fulltargetpath);
                            if (Preferences.ContainsKey("Verify_" + sfile.id + "_Version"))
                                Preferences.Remove("Verify_" + sfile.id + "_Version");
                            if (Preferences.ContainsKey("Verify_" + sfile.id + "_LastWriteTime"))
                                Preferences.Remove("Verify_" + sfile.id + "_LastWriteTime");
                        }
                        else
                        {
                            continue;
                        }
                    }

                    if (!GHApp.IsSecretsFileSavedToDisk(sfile))
                        continue;
#if __ANDROID__
                    using (Stream s = assets.Open(fullsourcepath))
#elif __IOS__
                    using (Stream s = File.OpenRead(fullsourcepath))
#elif WINDOWS
                    using (Stream s = await FileSystem.Current.OpenAppPackageFileAsync(fullsourcepath))
#else
                    using (Stream s = File.OpenRead(fullsourcepath))
#endif
                    {
                        if (s == null)
                            continue;

                        //bool createtemp = false;
                        //long existinglength = 0;
                        //if (File.Exists(fulltargetpath))
                        //{
                        //    createtemp = true;
                        //    FileInfo file = new FileInfo(fulltargetpath);
                        //    existinglength = file.Length;
                        //}
                        //if (createtemp)
                        //{
                        //    string temptargetpath = fulltargetpath + "temp";
                        //    if (File.Exists(temptargetpath))
                        //    {
                        //        File.Delete(temptargetpath);
                        //    }
                        //    using (FileStream fs = new FileStream(temptargetpath, FileMode.Create))
                        //    {
                        //        s.CopyTo(fs);
                        //    }
                        //    FileInfo tempfile = new FileInfo(temptargetpath);
                        //    long templength = tempfile.Length;

                        //    if (templength == existinglength)
                        //    {
                        //        File.Delete(temptargetpath);
                        //    }
                        //    else
                        //    {
                        //        File.Delete(fulltargetpath);
                        //        tempfile.MoveTo(fulltargetpath);
                        //        if (tempfile.Exists && templength == sfile.length)
                        //        {
                        //            Preferences.Set("Verify_" + sfile.id + "_Version", sfile.version);
                        //            Preferences.Set("Verify_" + sfile.id + "_LastWriteTime", tempfile.LastWriteTimeUtc);
                        //        }
                        //    }
                        //}
                        //else
                        //{
                        using (FileStream fs = new FileStream(fulltargetpath, FileMode.Create))
                        {
                            s.CopyTo(fs);
                        }
                        FileInfo curfile = new FileInfo(fulltargetpath);
                        if (curfile.Exists)
                        {
                            long curlength = curfile.Length;
                            long used_length = GHApp.IsDesktop ? sfile.length_desktop : sfile.length_mobile;
                            if (curlength == used_length)
                            {
                                Preferences.Set("Verify_" + sfile.id + "_Version", sfile.version);
                                Preferences.Set("Verify_" + sfile.id + "_LastWriteTime", curfile.LastWriteTimeUtc);
                            }
                            else
                            {
                                File.Delete(fulltargetpath);
                                if (Preferences.ContainsKey("Verify_" + sfile.id + "_Version"))
                                    Preferences.Remove("Verify_" + sfile.id + "_Version");
                                if (Preferences.ContainsKey("Verify_" + sfile.id + "_LastWriteTime"))
                                    Preferences.Remove("Verify_" + sfile.id + "_LastWriteTime");
                            }
                            //}
                        }
                    }
                    //using (BinaryReader br = new BinaryReader(assets.Open(fullsourcepath)))
                    //{
                    //    data = br.ReadBytes(packfilemaxsize);
                    //}

                    //string fulltargetpath = Path.Combine(filesdir, "bank", assetfile);
                    //if (File.Exists(fulltargetpath))
                    //{
                    //    /* Should check whether the current one is an up-to-date version; assume for now that it is not */
                    //    File.Delete(fulltargetpath);
                    //    //continue;
                    //}

                    //using (BinaryWriter sw = new BinaryWriter(File.Open(fulltargetpath, FileMode.Create)))
                    //{
                    //    sw.Write(data);
                    //}
                }
                catch (Exception)
                {
                    await Task.Delay(5);
                }
            }            
        }

        public void GetGlyphArrays(out IntPtr gl2ti, out int size1, out IntPtr gltifl, out int gltifl_size)
        {
            //size1 = gltifl_size = 0;
            //gl2ti = gltifl = IntPtr.Zero;
            GetGlyph2Tile(out gl2ti, out size1);
            GetGlyphTileFlags(out gltifl, out gltifl_size);
        }
        public void GetTileArrays(out IntPtr ti2an, out int size2, out IntPtr ti2en, out int size3, out IntPtr ti2ad, out int size4, 
            out IntPtr anoff_ptr, out int anoff_size, out IntPtr enoff_ptr, out int enoff_size, out IntPtr reoff_ptr, out int reoff_size)
        {
            //ti2an = ti2en = ti2ad = anoff_ptr = enoff_ptr = reoff_ptr = IntPtr.Zero;
            //size2 = size3= size4 = anoff_size = enoff_size = reoff_size = 0;
            GetTile2Animation(out ti2an, out size2);
            GetTile2Enlargement(out ti2en, out size3);
            GetTile2Autodraw(out ti2ad, out size4);
            GetAnimationOffsets(out anoff_ptr, out anoff_size);
            GetEnlargementOffsets(out enoff_ptr, out enoff_size);
            GetReplacementOffsets(out reoff_ptr, out reoff_size);
        }
        public int GetTotalTiles()
        {
            return CountTotalTiles();
        }
        public int GetUnexploredGlyph()
        {
            return LibGetUnexploredGlyph();
        }
        public int GetNoGlyph()
        {
            return LibGetNoGlyph();
        }

        public void GetOffs(out int a, out int e, out int r, out int gen_tile, out int hit_tile, out int ui_tile, out int spell_tile, out int skill_tile, out int command_tile, out int buff_tile, out int cursor_off)
        {
            //a = e =r = gen_tile = hit_tile = ui_tile = spell_tile = skill_tile = buff_tile = cursor_off = 0;
            a = LibGetAnimationOff();
            e = LibGetEnlargementOff();
            r = LibGetReplacementOff();
            gen_tile = LibGetGeneralTileOff();
            hit_tile = LibGetHitTileOff();
            ui_tile = LibGetUITileOff();
            spell_tile = LibGetSpellTileOff();
            skill_tile = LibGetSkillTileOff();
            command_tile = LibGetCommandTileOff();
            buff_tile = LibGetBuffTileOff();
            cursor_off = LibGetCursorOff();
        }

        public List<AnimationDefinition> GetAnimationArray()
        {
            int siz = LibGetAnimationArraySize();
            List<AnimationDefinition> array = new List<AnimationDefinition>();
            for (int i = 0; i < siz; i++)
            {
                AnimationDefinition anim = LibGetAnimationArrayElement(i);
                array.Add(anim);
            }

            return array;
        }
        public List<EnlargementDefinition> GetEnlargementArray()
        {
            int siz = LibGetEnlargementArraySize();
            List<EnlargementDefinition> array = new List<EnlargementDefinition>();
            for (int i = 0; i < siz; i++)
            {
                EnlargementDefinition enl = LibGetEnlargementArrayElement(i);
                array.Add(enl);
            }

            return array;
        }
        public List<ReplacementDefinition> GetReplacementArray()
        {
            int siz = LibGetReplacementArraySize();
            List<ReplacementDefinition> array = new List<ReplacementDefinition>();
            List<ReplacementDefinition> array2 = array;
            for (int i = 0; i < siz; i++)
            {
                ReplacementDefinition enl = LibGetReplacementArrayElement(i);
                array2.Add(enl);
            }

            return array;
        }
        public List<AutoDrawDefinition> GetAutoDrawArray()
        {
            int siz = LibGetAutoDrawArraySize();
            List<AutoDrawDefinition> array = new List<AutoDrawDefinition>();
            for (int i = 0; i < siz; i++)
            {
                AutoDrawDefinition enl = LibGetAutoDrawArrayElement(i);
                array.Add(enl);
            }

            return array;
        }
        public void SetGlyphArrays(IntPtr ptr_gl2ti, int size_gl2ti, IntPtr ptr_gltifl, int size_gltifl)
        {
            LibSetGlyphArrays(ptr_gl2ti, size_gl2ti, ptr_gltifl, size_gltifl);
        }
        public void SetTile2AnimationArray(IntPtr ptr_ti2an, int size_ti2an)
        {
            LibSetTile2AnimationArray(ptr_ti2an, size_ti2an);
        }

        public int GetTileAnimationIndexFromGlyph(int glyph)
        {
            return LibGetTileAnimationIndexFromGlyph(glyph);
        }
        public bool GlyphIsExplosion(int glyph)
        {
            return (LibGlyphIsExplosion(glyph) != 0);
        }
        public bool GlyphIsZap(int glyph)
        {
            return (LibGlyphIsZap(glyph) != 0);
        }
        public bool GlyphIsAnyDying(int glyph)
        {
            return (LibGlyphIsAnyDying(glyph) != 0);
        }

        public int GetAnimatedTile(int ntile, int tile_animation_idx, int play_type, long interval_counter,
                    out int frame_idx_ptr, out int main_tile_idx_ptr, out sbyte mapAnimated, ref int autodraw_ptr)
        {
            //frame_idx_ptr = main_tile_idx_ptr = autodraw_ptr = 0;
            //mapAnimated = 0;

            return LibMaybeGetAnimatedTile(ntile, tile_animation_idx, play_type, interval_counter,
                out frame_idx_ptr, out main_tile_idx_ptr, out mapAnimated, ref autodraw_ptr);
        }

        public int ZapGlyphToCornerGlyph(int adjglyph, ulong adjflags, int source_dir)
        {
            return LibZapGlyphToCornerGlyph(adjglyph, adjflags, source_dir);
        }

        public int Test()
        {
            return LibTest();
        }
        public string GetVersionString()
        {
            IntPtr resptr = LibGetVersionString();
            string ret = Marshal.PtrToStringAnsi(resptr);
            return ret;
        }
        public string GetVersionId()
        {
            IntPtr resptr = LibGetVersionId();
            string ret = Marshal.PtrToStringAnsi(resptr);
            return ret;
        }
        public ulong GetVersionNumber()
        {
            return LibGetVersionNumber();
        }
        public ulong GetVersionCompatibility()
        {
            return LibGetVersionCompatibility();
        }
        public string GetPropertyName(int prop_index)
        {
            IntPtr resptr = LibGetPropertyName(prop_index);
            string ret = Marshal.PtrToStringAnsi(resptr);
            return ret;
        }
        public string DumplogDateString(long startdate)
        {
            IntPtr resptr = LibDumplogDateString(startdate);
            string ret = Marshal.PtrToStringAnsi(resptr);
            return ret;
        }
        public List<string> GetExtendedCommands()
        {
            List<string> ret = GHUtils.GetAllStringsFromZeroTerminatedArray(LibGetExtendedCommands(), 256);
            ret.Sort();
            return ret;
        }

        public void SetPetMID(uint m_id)
        {
            LibSetPetMID(m_id);
        }

        public int Chmod(string filename, uint mode)
        {
            int res = LibChmod(filename, mode);
            return res;
        }

        public void SaveAndRestoreSavedGame(int save_style)
        {
            LibSaveAndRestoreSavedGame(save_style);
        }
        public void TallyRealTime()
        {
            LibTallyRealTime();
        }

        public int GetMaxManuals()
        {
            return LibGetMaxManuals();
        }

        public int GetFirstCatalogue()
        {
            return LibGetFirstCatalogue();
        }

        public int GetNumCatalogues()
        {
            return LibGetNumCatalogues();
        }

        public bool IsDebug()
        {
            return LibIsDebug() != 0;
        }

        public bool ValidateSaveFile(string filename, out string res_str)
        {
            byte[] buffer = new byte[256 * 4];
            int res = LibValidateSaveFile(filename, buffer);
            res_str = Encoding.UTF8.GetString(buffer);
            return res != 0;
        }

        public int CheckCurrentFileDescriptor()
        {
            return LibCheckCurrentFileDescriptor(GetGnollHackPath());
        }

        public void ReportFileDescriptors()
        {
            LibReportFileDescriptors();
        }
        public bool IncreaseFileDescriptorLimitToAtLeast(int min_cur)
        {
            return LibIncreaseFileDescriptorLimitToAtLeast(min_cur) != 0;
        }
        public int GetFileDescriptorLimit(bool is_max_limit)
        {
            return LibGetFileDescriptorLimit(is_max_limit ? 1 : 0);
        }

        public bool GetCharacterClickAction()
        {
            return LibGetCharacterClickAction() != 0;
        }

        public void SetCharacterClickAction(bool newValue)
        {
            LibSetCharacterClickAction(newValue ? 1 : 0);
        }

        public int GetMouseCommand(bool isMiddle)
        {
            return LibGetMouseCommand(isMiddle ? 1 : 0);
        }

        public void SetMouseCommand(int newValue, bool isMiddle)
        {
            LibSetMouseCommand(newValue, isMiddle ? 1 : 0);
        }

        public string GetEventPathForGHSound(int ghsound)
        {
            IntPtr resptr = LibGetEventPathForGHSound(ghsound);
            string ret = Marshal.PtrToStringAnsi(resptr);
            return ret;
        }

        public float GetVolumeForGHSound(int ghsound)
        {
            return LibGetVolumeForGHSound(ghsound);
        }

        public int StartGnollHack(GHGame ghGame)
        {
            GHApp.SetMirroredOptionsToDefaults();
            string filesdir = GetGnollHackPath();
            bool allowbones = GHApp.AllowBones;
            ulong rightmouse = (ulong)GHApp.MirroredRightMouseCommand << GHConstants.RightMouseBitIndex;
            ulong middlemouse = (ulong)GHApp.MirroredMiddleMouseCommand << GHConstants.MiddleMouseBitIndex;
            ulong runflags = (ulong)(ghGame.WizardMode ? RunGnollHackFlags.WizardMode : 0) |
                (ulong)(GHApp.FullVersionMode ? RunGnollHackFlags.FullVersion : 0) |
                (ulong)(ghGame.ModernMode ? RunGnollHackFlags.ModernMode : 0) |
                (ulong)(ghGame.CasualMode ? RunGnollHackFlags.CasualMode : 0) |
                (ulong)(allowbones ? 0 : RunGnollHackFlags.DisableBones) |
                (ulong)(GHApp.TournamentMode ? RunGnollHackFlags.TournamentMode : 0) |
                (ulong)(GHApp.IsDebug ? RunGnollHackFlags.GUIDebugMode : 0) |
                (ulong)(GHApp.MirroredCharacterClickAction ? RunGnollHackFlags.CharacterClickAction : 0) | /* Use the default; GHApp.CharacterClickAction may contain the option value from the last game */
                rightmouse | middlemouse | (ulong)ghGame.StartFlags;
            string lastusedplname = GHApp.TournamentMode && !ghGame.PlayingReplay ? GHApp.LastUsedTournamentPlayerName : GHApp.LastUsedPlayerName;

            return RunGnollHack(
                filesdir,
                "",
                "",
                lastusedplname,
                runflags,
                0,
                0,
                ghGame.ClientCallback_InitWindows,
                ghGame.ClientCallback_PlayerSelection,
                ghGame.ClientCallback_AskName,
                ghGame.ClientCallback_get_nh_event,
                ghGame.ClientCallback_ExitWindows,
                ghGame.ClientCallback_VoidConstCharDummy,
                ghGame.ClientCallback_VoidVoidDummy,
                ghGame.ClientCallback_CreateGHWindow,
                ghGame.ClientCallback_ClearGHWindow,
                ghGame.ClientCallback_DisplayGHWindow,
                ghGame.ClientCallback_DestroyGHWindow,
                ghGame.ClientCallback_Curs,
                ghGame.ClientCallback_PutStrEx,
                ghGame.ClientCallback_PutStrEx2,
                ghGame.ClientCallback_PutStrEx,
                ghGame.ClientCallback_VoidConstCharBooleanDummy,
                ghGame.ClientCallback_StartMenu,
                ghGame.ClientCallback_AddMenu,
                ghGame.ClientCallback_AddExtendedMenu,
                ghGame.ClientCallback_EndMenu,
                ghGame.ClientCallback_SelectMenu,
                ghGame.ClientCallback_VoidIntDummy, /* no need for X-specific handling */
                ghGame.ClientCallback_VoidVoidDummy,
                ghGame.ClientCallback_VoidVoidDummy,
                ghGame.ClientCallback_VoidVoidDummy,
                /* If clipping is on */
                ghGame.ClientCallback_Cliparound,
                /* If positionbar is on */
                ghGame.ClientCallback_VoidCharDummy,
                ghGame.ClientCallback_PrintGlyph,
                ghGame.ClientCallback_IssueGuiCommand,
                ghGame.ClientCallback_RawPrint,
                ghGame.ClientCallback_RawPrintBold,
                ghGame.ClientCallback_nhgetch,
                ghGame.ClientCallback_nh_poskey,
                ghGame.ClientCallback_VoidVoidDummy,
                ghGame.ClientCallback_IntVoidDummy,
                ghGame.ClientCallback_YnFunction,
                ghGame.ClientCallback_GetLine,
                ghGame.ClientCallback_IntVoidDummy,
                ghGame.ClientCallback_VoidIntDummy,
                ghGame.ClientCallback_DelayOutput,
                ghGame.ClientCallback_DelayOutputMilliseconds,
                ghGame.ClientCallback_DelayOutputIntervals,

                ghGame.ClientCallback_VoidVoidDummy,
                ghGame.ClientCallback_VoidIntDummy,
                ghGame.ClientCallback_VoidIntDummy,
                ghGame.ClientCallback_CharVoidDummy,

                ghGame.ClientCallback_VoidVoidDummy,
                ghGame.ClientCallback_VoidVoidDummy,
                ghGame.ClientCallback_OutRip,
                ghGame.ClientCallback_PreferenceUpdate,
                ghGame.ClientCallback_GetMsgHistory,
                ghGame.ClientCallback_PutMsgHistory,
                ghGame.ClientCallback_StatusInit,
                ghGame.ClientCallback_StatusFinish,
                ghGame.ClientCallback_StatusEnable,
                ghGame.ClientCallback_StatusUpdate,
                ghGame.ClientCallback_BooleanVoidDummy,
                ghGame.ClientCallback_VoidVoidDummy,
                ghGame.ClientCallback_VoidUlongDummy,
                ghGame.ClientCallback_OpenSpecialView,
                ghGame.ClientCallback_StopAllSounds,
                ghGame.ClientCallback_PlayImmediateSound,
                ghGame.ClientCallback_PlayOccupationAmbient,
                ghGame.ClientCallback_PlayEffectAmbient,
                ghGame.ClientCallback_SetEffectAmbientVolume,
                ghGame.ClientCallback_PlayMusic,
                ghGame.ClientCallback_PlayLevelAmbient,
                ghGame.ClientCallback_PlayEnvironmentAmbient,
                ghGame.ClientCallback_AdjustGeneralVolumes,
                ghGame.ClientCallback_AddAmbientSound,
                ghGame.ClientCallback_DeleteAmbientSound,
                ghGame.ClientCallback_SetAmbientSoundVolume,
                ghGame.ClientCallback_ClearContextMenu,
                ghGame.ClientCallback_AddContextMenu,
                ghGame.ClientCallback_UpdateStatusButton,
                ghGame.ClientCallback_ToggleAnimationTimer,
                ghGame.ClientCallback_DisplayFloatingText,
                ghGame.ClientCallback_DisplayScreenText,
                ghGame.ClientCallback_DisplayPopupText,
                ghGame.ClientCallback_DisplayGUIEffect,
                ghGame.ClientCallback_UpdateCursor,
                ghGame.ClientCallback_UIHasInput,
                ghGame.ClientCallback_ExitHack,
                ghGame.ClientCallback_GetCwd,
                ghGame.ClientCallback_MessageBox,
                ghGame.ClientCallback_FreeMemory,
                ghGame.ClientCallback_ReportPlayerName,
                ghGame.ClientCallback_ReportPlayTime,
                ghGame.ClientCallback_SendObjectData,
                ghGame.ClientCallback_SendMonsterData,
                ghGame.ClientCallback_SendEngravingData
                );

        }
    }
}