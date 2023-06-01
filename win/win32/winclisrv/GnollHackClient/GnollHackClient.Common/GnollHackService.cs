using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;
using System.IO;
using Xamarin.Forms;
using System.Runtime.InteropServices;
using GnollHackClient;
using GnollHackCommon;
using Xamarin.Essentials;

#if __IOS__
using Foundation;
using UIKit;

[assembly: Dependency(typeof(GnollHackClient.iOS.GnollHackService))]
namespace GnollHackClient.iOS
#elif __ANDROID__
using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using Android.Content.Res;

[assembly: Dependency(typeof(GnollHackClient.Droid.GnollHackService))]
namespace GnollHackClient.Droid
#else
[assembly: Dependency(typeof(GnollHackClient.Unknown.GnollHackService))]
namespace GnollHackClient.Unknown
#endif
{
    public class PlatformConstants
    {
#if __IOS__
        public const string dll     = "__Internal";
        public const string library = "gnollhackios";
#elif __ANDROID__
        public const string dll     = @"libgnollhackdroid.so";
        public const string library = "gnollhackdroid";
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
            VoidIntBooleanCallback callback_display_nhwindow,
            VoidIntCallback callback_destroy_nhwindow,
            VoidIntIntIntCallback callback_curs,
            PutStrExColorCallback callback_putstr_ex,
            PutStrEx2ColorCallback callback_putstr_ex2,
            PutStrExColorCallback callback_putmixed_ex,
            VoidConstCharBooleanCallback callback_display_file,
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
            VoidIntIntBooleanCallback callback_cliparound,
            /* If positionbar is on */
            VoidCharCallback callback_update_positionbar,
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
            VoidIntConstCharPtrConstCharPtrBooleanCallback callback_status_enablefield,
            StatusUpdateCallback callback_status_update,
            BooleanVoidCallback callback_can_suspend_yes,
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
            SendMonsterDataCallback callback_send_monster_data
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
        public static extern int get_tile_animation_index_from_glyph(int glyph);


        [DllImport(PlatformConstants.dll)]
        public static extern byte LibGlyphIsExplosion(int glyph);
        [DllImport(PlatformConstants.dll)]
        public static extern byte LibGlyphIsZap(int glyph);
        [DllImport(PlatformConstants.dll)]
        public static extern byte LibGlyphIsAnyDying(int glyph);
        [DllImport(PlatformConstants.dll)]
        public static extern int maybe_get_animated_tile(int ntile, int tile_animation_idx, int play_type, long interval_counter, 
            out int frame_idx_ptr, out int main_tile_idx_ptr, out sbyte mapAnimated, ref int autodraw_ptr);
        [DllImport(PlatformConstants.dll)]
        public static extern int LibZapGlyphToCornerGlyph(int adjglyph, ulong adjflags, int source_dir);
        [DllImport(PlatformConstants.dll)]
        public static extern void LibSetPetMID(uint m_id);
        [DllImport(PlatformConstants.dll)]
        public static extern int LibChmod(string filename, uint mode);
        [DllImport(PlatformConstants.dll)]
        public static extern void LibSaveAndRestoreSavedGame();
        [DllImport(PlatformConstants.dll)]
        public static extern void LibTallyRealTime();
        [DllImport(PlatformConstants.dll)]
        public static extern int LibValidateSaveFile(string filename, [MarshalAs(UnmanagedType.LPArray), Out] byte[] out_buffer);

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

            string[] ghdirlist = { "save" };
            foreach (string ghdir in ghdirlist)
            {
                string fulldirepath = Path.Combine(filesdir, ghdir);
                if (Directory.Exists(fulldirepath))
                {
                    DirectoryInfo disave = new DirectoryInfo(fulldirepath);
                    foreach (FileInfo file in disave.GetFiles())
                    {
                        file.Delete();
                    }
                }
            }
        }

        public void ClearDumplogs()
        {
            string filesdir = GetGnollHackPath();

            string[] ghdirlist = { "dumplog" };
            foreach (string ghdir in ghdirlist)
            {
                string fulldirepath = Path.Combine(filesdir, ghdir);
                if (Directory.Exists(fulldirepath))
                {
                    DirectoryInfo disave = new DirectoryInfo(fulldirepath);
                    foreach (FileInfo file in disave.GetFiles())
                    {
                        file.Delete();
                    }
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

        public void ResetDefaultsFile()
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
            using (StreamReader sr = new StreamReader(assets.Open(fullsourcepath)))
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

        public string GetGnollHackPath()
        {
#if __ANDROID__
            return Android.App.Application.Context.FilesDir.Path;
#elif __IOS__
            try
            {
                string documents = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
                string library = Path.Combine(documents, "..", "Library");
                string path = Path.Combine(library, "GnollHackData");
                if (!Directory.Exists(path))
                    Directory.CreateDirectory(path);
                return path;
            }
            catch
            {
                return ".";
            }
#else
            return ".";
#endif
        }


        private string[] _txtfileslist = { "credits", "xcredits", "license", "logfile", "perm", "record", "recover", "symbols", "sysconf", "xlogfile", "defaults.gnh" };
        private string[] _binfileslist = { "nhdat" };

        public void InitializeGnollHack(Secrets secrets)
        {
            /* Unpack GnollHack files */
            /* Add a check whether to unpack if there are existing files or not */

            string filesdir = GetGnollHackPath();

            /* For debugging purposes now, delete all existing files in filesdir first */
            //System.IO.DirectoryInfo di = new DirectoryInfo(filesdir);

            //foreach (FileInfo file in di.GetFiles())
            //{
            //    file.Delete();
            //}

            /* Make relevant directories */
            string[] ghdirlist = { "save", "dumplog" };
            foreach (string ghdir in ghdirlist)
            {
                string fulldirepath = Path.Combine(filesdir, ghdir);
                App.CheckCreateDirectory(fulldirepath);
            }

            /* Copy missing files from resources */
            string content;
            string assetsourcedir = "gnh";
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
                using (StreamReader sr = new StreamReader(assets.Open(fullsourcepath)))
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
#else
                string fullsourcepath = Path.Combine(assetsourcedir, txtfile);
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

            if(secrets != null)
            {
                foreach (SecretsDirectory sdir in secrets.directories)
                {
                    string fulldirepath = Path.Combine(filesdir, sdir.name);
                    if (!Directory.Exists(fulldirepath))
                    {
                        Directory.CreateDirectory(fulldirepath);
                    }
                }
                //int packfilemaxsize = 512 * 1024 * 1024;
                foreach (SecretsFile sfile in secrets.files)
                {
                    string assetfile = sfile.name;
                    string sfiledir = sfile.source_directory;

#if __IOS__
                    string extension = Path.GetExtension(assetfile);
                    if (extension != null && extension.Length > 0)
                        extension = extension.Substring(1); /* Remove . from the start */
                    string fname = Path.GetFileNameWithoutExtension(assetfile);
                    string fullsourcepath = NSBundle.MainBundle.PathForResource(fname, extension, sfiledir);
#elif __ANDROID__
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

                            if (curlength != sfile.length || curfileversion != sfile.version || moddate != curfile.LastWriteTimeUtc)
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
#if __ANDROID__
                        using (Stream s = assets.Open(fullsourcepath))
#elif __IOS__
                        using (Stream s = File.OpenRead(fullsourcepath))
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
                                if (curlength == sfile.length)
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
                    catch
                    {
                        /* Likely asset at fullsourcepath did not exist */
                    }
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

        public void GetOffs(out int a, out int e, out int r, out int gen_tile, out int hit_tile, out int ui_tile, out int spell_tile, out int skill_tile, out int buff_tile, out int cursor_off)
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
        public int GetTileAnimationIndexFromGlyph(int glyph)
        {
            return get_tile_animation_index_from_glyph(glyph);
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

            return maybe_get_animated_tile(ntile, tile_animation_idx, play_type, interval_counter,
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

        public void SaveAndRestoreSavedGame()
        {
            LibSaveAndRestoreSavedGame();
        }
        public void TallyRealTime()
        {
            LibTallyRealTime();
        }

        public bool ValidateSaveFile(string filename, out string res_str)
        {
            byte[] buffer = new byte[256 * 4];
            int res = LibValidateSaveFile(filename, buffer);
            res_str = Encoding.UTF8.GetString(buffer);
            return res != 0;
        }

        public int StartGnollHack(ClientGame clientGame)
        {
            string filesdir = GetGnollHackPath();
            bool allowbones = Preferences.Get("AllowBones", true);
            ulong runflags = (ulong)(clientGame.WizardMode ? RunGnollHackFlags.WizardMode : 0) |
                (ulong)(App.FullVersionMode ? RunGnollHackFlags.FullVersion : 0) |
                (ulong)(clientGame.ModernMode ? RunGnollHackFlags.ModernMode : 0) |
                (ulong)(clientGame.CasualMode ? RunGnollHackFlags.CasualMode : 0) |
                (ulong)(allowbones ? 0 : RunGnollHackFlags.DisableBones);
            string lastusedplname = Preferences.Get("LastUsedPlayerName", "");

            return RunGnollHack(
                filesdir,
                "",
                "",
                lastusedplname,
                runflags,
                0,
                0,
                clientGame.ClientCallback_InitWindows,
                clientGame.ClientCallback_PlayerSelection,
                clientGame.ClientCallback_AskName,
                clientGame.ClientCallback_get_nh_event,
                clientGame.ClientCallback_ExitWindows,
                clientGame.ClientCallback_VoidConstCharDummy,
                clientGame.ClientCallback_VoidVoidDummy,
                clientGame.ClientCallback_CreateGHWindow,
                clientGame.ClientCallback_ClearGHWindow,
                clientGame.ClientCallback_DisplayGHWindow,
                clientGame.ClientCallback_DestroyGHWindow,
                clientGame.ClientCallback_Curs,
                clientGame.ClientCallback_PutStrEx,
                clientGame.ClientCallback_PutStrEx2,
                clientGame.ClientCallback_PutStrEx,
                clientGame.ClientCallback_VoidConstCharBooleanDummy,
                clientGame.ClientCallback_StartMenu,
                clientGame.ClientCallback_AddMenu,
                clientGame.ClientCallback_AddExtendedMenu,
                clientGame.ClientCallback_EndMenu,
                clientGame.ClientCallback_SelectMenu,
                clientGame.ClientCallback_VoidIntDummy, /* no need for X-specific handling */
                clientGame.ClientCallback_VoidVoidDummy,
                clientGame.ClientCallback_VoidVoidDummy,
                clientGame.ClientCallback_VoidVoidDummy,
                /* If clipping is on */
                clientGame.ClientCallback_Cliparound,
                /* If positionbar is on */
                clientGame.ClientCallback_VoidCharDummy,
                clientGame.ClientCallback_PrintGlyph,
                clientGame.ClientCallback_IssueGuiCommand,
                clientGame.ClientCallback_RawPrint,
                clientGame.ClientCallback_RawPrintBold,
                clientGame.ClientCallback_nhgetch,
                clientGame.ClientCallback_nh_poskey,
                clientGame.ClientCallback_VoidVoidDummy,
                clientGame.ClientCallback_IntVoidDummy,
                clientGame.ClientCallback_YnFunction,
                clientGame.ClientCallback_GetLine,
                clientGame.ClientCallback_IntVoidDummy,
                clientGame.ClientCallback_VoidIntDummy,
                clientGame.ClientCallback_DelayOutput,
                clientGame.ClientCallback_DelayOutputMilliseconds,
                clientGame.ClientCallback_DelayOutputIntervals,

                clientGame.ClientCallback_VoidVoidDummy,
                clientGame.ClientCallback_VoidIntDummy,
                clientGame.ClientCallback_VoidIntDummy,
                clientGame.ClientCallback_CharVoidDummy,

                clientGame.ClientCallback_VoidVoidDummy,
                clientGame.ClientCallback_VoidVoidDummy,
                clientGame.ClientCallback_OutRip,
                clientGame.ClientCallback_PreferenceUpdate,
                clientGame.ClientCallback_GetMsgHistory,
                clientGame.ClientCallback_PutMsgHistory,
                clientGame.ClientCallback_StatusInit,
                clientGame.ClientCallback_StatusFinish,
                clientGame.ClientCallback_StatusEnable,
                clientGame.ClientCallback_StatusUpdate,
                clientGame.ClientCallback_BooleanVoidDummy,
                clientGame.ClientCallback_VoidVoidDummy,
                clientGame.ClientCallback_VoidUlongDummy,
                clientGame.ClientCallback_OpenSpecialView,
                clientGame.ClientCallback_StopAllSounds,
                clientGame.ClientCallback_PlayImmediateSound,
                clientGame.ClientCallback_PlayOccupationAmbient,
                clientGame.ClientCallback_PlayEffectAmbient,
                clientGame.ClientCallback_SetEffectAmbientVolume,
                clientGame.ClientCallback_PlayMusic,
                clientGame.ClientCallback_PlayLevelAmbient,
                clientGame.ClientCallback_PlayEnvironmentAmbient,
                clientGame.ClientCallback_AdjustGeneralVolumes,
                clientGame.ClientCallback_AddAmbientSound,
                clientGame.ClientCallback_DeleteAmbientSound,
                clientGame.ClientCallback_SetAmbientSoundVolume,
                clientGame.ClientCallback_ClearContextMenu,
                clientGame.ClientCallback_AddContextMenu,
                clientGame.ClientCallback_UpdateStatusButton,
                clientGame.ClientCallback_ToggleAnimationTimer,
                clientGame.ClientCallback_DisplayFloatingText,
                clientGame.ClientCallback_DisplayScreenText,
                clientGame.ClientCallback_DisplayPopupText,
                clientGame.ClientCallback_DisplayGUIEffect,
                clientGame.ClientCallback_UpdateCursor,
                clientGame.ClientCallback_UIHasInput,
                clientGame.ClientCallback_ExitHack,
                clientGame.ClientCallback_GetCwd,
                clientGame.ClientCallback_MessageBox,
                //clientGame.ClientCallback_VoidIntDummy,
                //clientGame.ClientCallback_VoidIntDummy,

                clientGame.ClientCallback_FreeMemory,
                clientGame.ClientCallback_ReportPlayerName,
                clientGame.ClientCallback_ReportPlayTime,
                clientGame.ClientCallback_SendObjectData,
                clientGame.ClientCallback_SendMonsterData
                );

        }
    }
}