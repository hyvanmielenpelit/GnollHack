using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;
using System.IO;
#if GNH_MAUI
using GnollHackM;
using System.Runtime.CompilerServices;
using System.Threading;
#else
using Xamarin.Forms;
using Xamarin.Essentials;
#endif
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using GnollHackX;
using System.Diagnostics;
using System.Globalization;

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
#elif GNH_AVALONIA && LINUX
        public const string dll = @"libgnollhacklinux.so";
        public const string library = "gnollhacklinux";
#elif GNH_AVALONIA && MACOS
        public const string dll = @"libgnollhackmac.dylib";
        public const string library = "gnollhackmac";
#elif GNH_AVALONIA
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
#if GNH_MAUI
        public static extern unsafe int RunGnollHack(
#else
        public static extern int RunGnollHack(
#endif
            [MarshalAs(UnmanagedType.LPStr)] string gnhdir,
            [MarshalAs(UnmanagedType.LPStr)] string cmdlineargs,
            [MarshalAs(GHConstants.GHStringMarshallingType)] string engrave_quicktext,
            [MarshalAs(GHConstants.GHStringMarshallingType)] string last_used_player_name,
            int right_mouse_button,
            int middle_mouse_button,
            int engrave_quickstyle,
            int reserved_int,
            ulong runflags,
            ulong foundmanuals,
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
#if GNH_MAUI
            delegate* unmanaged[Stdcall]<int, IntPtr, int, int, int, void> callback_putstr_ex,
            delegate* unmanaged[Stdcall]<int, IntPtr, IntPtr, IntPtr, int, int, int, void> callback_putstr_ex2,
            delegate* unmanaged[Stdcall]<int, IntPtr, int, int, int, void> callback_putmixed_ex,
#else
            PutStrExColorCallback callback_putstr_ex,
            PutStrEx2ColorCallback callback_putstr_ex2,
            PutStrExColorCallback callback_putmixed_ex,
#endif
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
#if GNH_MAUI
            delegate* unmanaged[Stdcall]<int, int, int, int, int, int, int, ulong, IntPtr, void> callback_print_glyph,
            delegate* unmanaged[Stdcall]<int, int, int, IntPtr, void> callback_issue_gui_command,
#else
            PrintGlyphCallback callback_print_glyph,
            IssueGuiCommandCallback callback_issue_gui_command,
#endif
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
#if GNH_MAUI
            delegate* unmanaged[Stdcall]<int, IntPtr, IntPtr, byte, void> callback_status_enablefield,
            delegate* unmanaged[Stdcall]<int, IntPtr, long, int, int, int, IntPtr, void> callback_status_update,
#else
            StatusEnableFieldCallback callback_status_enablefield,
            StatusUpdateCallback callback_status_update,
#endif
            CanSuspendYesCallback callback_can_suspend_yes,
            VoidVoidCallback callback_stretch_window,
            VoidUlongCallback callback_set_animation_timer_interval,
            OpenSpecialViewCallback callback_open_special_view,
            StopAllSoundsCallback callback_stop_all_sounds,
#if GNH_MAUI
            delegate* unmanaged[Stdcall]<int, IntPtr, int, double, double, IntPtr, IntPtr, int, int, int, uint, uint, int> callback_play_immediate_ghsound,
#else
            PlayImmediateSoundCallback callback_play_immediate_ghsound,
#endif
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
#if GNH_MAUI
            delegate* unmanaged[Stdcall]<void> callback_clear_context_menu,
            delegate* unmanaged[Stdcall]<int, int, int, int, IntPtr, IntPtr, int, int, void> callback_add_context_menu,
#else
            VoidVoidCallback callback_clear_context_menu,
            AddContextMenuCallback callback_add_context_menu,
#endif
            UpdateStatusButtonCallback callback_update_status_button,
#if GNH_MAUI
            delegate* unmanaged[Stdcall]<int, int, int, int, int, int, ulong, void> callback_toggle_animation_timer,
            delegate* unmanaged[Stdcall]<int, int, IntPtr, int, int, int, ulong, void> callback_display_floating_text,
#else
            ToggleAnimationTimerCallback callback_toggle_animation_timer,
            DisplayFloatingTextCallback callback_display_floating_text,
#endif
            DisplayScreenTextCallback callback_display_screen_text,
            DisplayPopupTextCallback callback_display_popup_text,
            DisplayGUIEffectCallback callback_display_gui_effect,
            UpdateCursorCallback callback_update_cursor,
#if GNH_MAUI
            delegate* unmanaged[Stdcall]<int> callback_ui_has_input,
#else
            UIHasInputCallback callback_ui_has_input,
#endif
#if GNH_MAUI
            delegate* unmanaged[Stdcall]<int, void> callback_exit_hack,
#else
            VoidIntCallback callback_exit_hack,
#endif
            CharVoidCallback callback_getcwd,
            IntCharCharUintCallback callback_messagebox,

            FreeMemoryCallback callback_free_memory,
            ReportPlayerNameCallback callback_report_player_name,
            ReportPlayTimeCallback callback_report_play_time,
#if GNH_MAUI
            delegate* unmanaged[Stdcall]<int, int, IntPtr, int, int, IntPtr, ulong, void> callback_send_object_data,
            delegate* unmanaged[Stdcall]<int, int, int, IntPtr, ulong, void> callback_send_monster_data,
#else
            SendObjectDataCallback callback_send_object_data,
            SendMonsterDataCallback callback_send_monster_data,
#endif
            SendEngravingDataCallback callback_send_engraving_data
        );

#if GNH_MAUI
        /* The game whose engine is running; the engine calls back only on that game's thread. Set by
           StartGnollHack, cleared by NativeExitHack, and by StartGnollHack when RunGnollHack returns early */
        private static GHGame _callbackGame;
        private static int _callbackFailureReported;

        /* Reports only the first callback failure of each game: a failing callback can fail on every map cell */
        private static void ReportCallbackException(Exception ex, [CallerMemberName] string memberName = "")
        {
            if (Interlocked.Exchange(ref _callbackFailureReported, 1) == 0)
                GHApp.ReportCaughtException(ex, memberName);
        }

        /* The last callback of a game: the engine thread then ends inside native code, so RunGnollHack does not return */
        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvStdcall) })]
        private static void NativeExitHack(int status)
        {
            GHGame game = _callbackGame;
            try
            {
                game?.ClientCallback_ExitHack(status);
            }
            catch (Exception ex)
            {
                ReportCallbackException(ex);
            }
            finally
            {
                if (game != null)
                    Interlocked.CompareExchange(ref _callbackGame, null, game);
            }
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvStdcall) })]
        private static void NativePutStrEx(int win_id, IntPtr str, int attributes, int color, int append)
        {
            try
            {
                _callbackGame?.ClientCallback_PutStrEx(win_id, GHUtils.PtrToEngineString(str), attributes, color, append);
            }
            catch (Exception ex)
            {
                ReportCallbackException(ex);
            }
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvStdcall) })]
        private static void NativePutStrEx2(int win_id, IntPtr str, IntPtr attributes_ptr, IntPtr colors_ptr, int attr, int color, int append)
        {
            try
            {
                _callbackGame?.ClientCallback_PutStrEx2(win_id, GHUtils.PtrToEngineString(str), attributes_ptr, colors_ptr, attr, color, append);
            }
            catch (Exception ex)
            {
                ReportCallbackException(ex);
            }
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvStdcall) })]
        private static void NativePrintGlyph(int winHandle, int x, int y, int glyph, int bkglyph, int symbol, int ocolor, ulong special, IntPtr layers_ptr)
        {
            try
            {
                _callbackGame?.ClientCallback_PrintGlyph(winHandle, x, y, glyph, bkglyph, symbol, ocolor, special, layers_ptr);
            }
            catch (Exception ex)
            {
                ReportCallbackException(ex);
            }
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvStdcall) })]
        private static void NativeIssueGuiCommand(int cmd_id, int cmd_param, int cmd_param2, IntPtr cmd_str)
        {
            try
            {
                _callbackGame?.ClientCallback_IssueGuiCommand(cmd_id, cmd_param, cmd_param2, GHUtils.PtrToEngineString(cmd_str));
            }
            catch (Exception ex)
            {
                ReportCallbackException(ex);
            }
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvStdcall) })]
        private static void NativeStatusEnable(int fieldidx, IntPtr nm, IntPtr fmt, byte enable)
        {
            try
            {
                _callbackGame?.ClientCallback_StatusEnable(fieldidx, GHUtils.PtrToEngineString(nm), GHUtils.PtrToEngineString(fmt), enable);
            }
            catch (Exception ex)
            {
                ReportCallbackException(ex);
            }
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvStdcall) })]
        private static void NativeStatusUpdate(int fieldidx, IntPtr text, long condbits, int cng, int percent, int color, IntPtr condcolorptr)
        {
            try
            {
                _callbackGame?.ClientCallback_StatusUpdate(fieldidx, GHUtils.PtrToEngineString(text), condbits, cng, percent, color, condcolorptr);
            }
            catch (Exception ex)
            {
                ReportCallbackException(ex);
            }
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvStdcall) })]
        private static int NativePlayImmediateSound(int ghsound, IntPtr eventPath, int bankid, double eventVolume, double soundVolume, IntPtr parameterNames, IntPtr parameterValues, int arraysize, int sound_type, int play_group, uint dialogue_mid, uint play_flags)
        {
            try
            {
                GHGame game = _callbackGame;
                if (game == null)
                    return 1;

                /* Fresh arrays on every call: recordings keep them by reference */
                string[] names = null;
                float[] values = null;
                if (parameterNames != IntPtr.Zero)
                {
                    names = new string[GHConstants.MaxSoundParameters];
                    int i;
                    for (i = 0; i < GHConstants.MaxSoundParameters; i++)
                        names[i] = GHUtils.PtrToEngineString(Marshal.ReadIntPtr(parameterNames, i * IntPtr.Size));
                }
                if (parameterValues != IntPtr.Zero)
                {
                    values = new float[GHConstants.MaxSoundParameters];
                    Marshal.Copy(parameterValues, values, 0, GHConstants.MaxSoundParameters);
                }
                return game.ClientCallback_PlayImmediateSound(ghsound, GHUtils.PtrToEngineString(eventPath), bankid, eventVolume, soundVolume, names, values, arraysize, sound_type, play_group, dialogue_mid, play_flags);
            }
            catch (Exception ex)
            {
                ReportCallbackException(ex);
                return 1;
            }
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvStdcall) })]
        private static void NativeClearContextMenu()
        {
            try
            {
                _callbackGame?.ClientCallback_ClearContextMenu();
            }
            catch (Exception ex)
            {
                ReportCallbackException(ex);
            }
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvStdcall) })]
        private static void NativeAddContextMenu(int cmd_def_char, int cmd_cur_char, int style, int glyph, IntPtr cmd_text, IntPtr target_text, int attr, int color)
        {
            try
            {
                _callbackGame?.ClientCallback_AddContextMenu(cmd_def_char, cmd_cur_char, style, glyph, GHUtils.PtrToEngineString(cmd_text), GHUtils.PtrToEngineString(target_text), attr, color);
            }
            catch (Exception ex)
            {
                ReportCallbackException(ex);
            }
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvStdcall) })]
        private static void NativeToggleAnimationTimer(int timertype, int timerid, int state, int x, int y, int layer, ulong tflags)
        {
            try
            {
                _callbackGame?.ClientCallback_ToggleAnimationTimer(timertype, timerid, state, x, y, layer, tflags);
            }
            catch (Exception ex)
            {
                ReportCallbackException(ex);
            }
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvStdcall) })]
        private static void NativeDisplayFloatingText(int x, int y, IntPtr text, int style, int attr, int color, ulong tflags)
        {
            try
            {
                _callbackGame?.ClientCallback_DisplayFloatingText(x, y, GHUtils.PtrToEngineString(text), style, attr, color, tflags);
            }
            catch (Exception ex)
            {
                ReportCallbackException(ex);
            }
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvStdcall) })]
        private static int NativeUIHasInput()
        {
            try
            {
                GHGame game = _callbackGame;
                return game != null ? game.ClientCallback_UIHasInput() : 0;
            }
            catch (Exception ex)
            {
                ReportCallbackException(ex);
                return 0;
            }
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvStdcall) })]
        private static void NativeSendObjectData(int x, int y, IntPtr otmp_ptr, int cmdtype, int where, IntPtr otypdata_ptr, ulong oflags)
        {
            try
            {
                _callbackGame?.ClientCallback_SendObjectData(x, y, otmp_ptr, cmdtype, where, otypdata_ptr, oflags);
            }
            catch (Exception ex)
            {
                ReportCallbackException(ex);
            }
        }

        [UnmanagedCallersOnly(CallConvs = new[] { typeof(CallConvStdcall) })]
        private static void NativeSendMonsterData(int cmdtype, int x, int y, IntPtr monster_data_ptr, ulong oflags)
        {
            try
            {
                _callbackGame?.ClientCallback_SendMonsterData(cmdtype, x, y, monster_data_ptr, oflags);
            }
            catch (Exception ex)
            {
                ReportCallbackException(ex);
            }
        }
#endif

        [DllImport(PlatformConstants.dll)]
        public static extern int LibInitializeTileData();
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
#if GNH_MAUI
        /* Pointer parameters keep the signature blittable, so ReadyToRun precompiles the call instead of the runtime
           generating a marshalling stub, which iOS would interpret */
        [DllImport(PlatformConstants.dll)]
        private static extern unsafe int LibMaybeGetAnimatedTile(int ntile, int tile_animation_idx, int play_type, long interval_counter,
            int* frame_idx_ptr, int* main_tile_idx_ptr, sbyte* mapAnimated, int* autodraw_ptr);
#else
        [DllImport(PlatformConstants.dll)]
        public static extern int LibMaybeGetAnimatedTile(int ntile, int tile_animation_idx, int play_type, long interval_counter,
            out int frame_idx_ptr, out int main_tile_idx_ptr, out sbyte mapAnimated, ref int autodraw_ptr);
#endif
        [DllImport(PlatformConstants.dll)]
        public static extern int LibZapGlyphToCornerGlyph(int adjglyph, ulong adjflags, int source_dir);
        [DllImport(PlatformConstants.dll)]
        public static extern void LibSetPetMID(uint m_id);
        [DllImport(PlatformConstants.dll)]
        public static extern int LibChmod([MarshalAs(UnmanagedType.LPStr)] string filename, uint mode);
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
        public static extern int LibGetMaxMajorConsultations([MarshalAs(UnmanagedType.LPStr)] string filename);
        [DllImport(PlatformConstants.dll)]
        public static extern int LibIsDebug();
        [DllImport(PlatformConstants.dll)]
        public static extern int LibValidateSaveFile([MarshalAs(UnmanagedType.LPStr)] string filename,[MarshalAs(UnmanagedType.LPArray), Out] byte[] out_buffer);
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetSaveFileInfo([MarshalAs(UnmanagedType.LPStr)] string filename,ref ulong out_version, ref ulong out_compat, ref uint out_save_flags, ref long out_time_stamp, ref int out_glyph, ref int out_gui_glyph, ref long out_umoves);
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetSaveFileDescription([MarshalAs(UnmanagedType.LPStr)] string filename,[MarshalAs(UnmanagedType.LPArray), Out] byte[] out_char_desc, int char_desc_len, [MarshalAs(UnmanagedType.LPArray), Out] byte[] out_loc_desc, int loc_desc_len, [MarshalAs(UnmanagedType.LPArray), Out] byte[] out_mode_desc, int mode_desc_len);
        [DllImport(PlatformConstants.dll)]
        public static extern int LibCheckCurrentFileDescriptor([MarshalAs(UnmanagedType.LPStr)] string dir);
        [DllImport(PlatformConstants.dll)]
        public static extern int LibReportFileDescriptors();
        [DllImport(PlatformConstants.dll)]
        public static extern int LibIncreaseFileDescriptorLimitToAtLeast(int min_cur);
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetFileDescriptorLimit(int is_max_limit);
        [DllImport(PlatformConstants.dll)]
        public static extern IntPtr LibGenerateAiSnapshot();
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetCharacterClickAction();
        [DllImport(PlatformConstants.dll)]
        public static extern void LibSetCharacterClickAction(int new_value);
        [DllImport(PlatformConstants.dll)]
        public static extern void LibSetMetricSystem(int new_value);
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetGetPositionArrows();
        [DllImport(PlatformConstants.dll)]
        public static extern void LibSetGetPositionArrows(int new_value);
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetDiceAsRanges();
        [DllImport(PlatformConstants.dll)]
        public static extern void LibSetDiceAsRanges(int new_value);
        [DllImport(PlatformConstants.dll)]
        public static extern void LibSetDamageFormula(int new_value);
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetWornShowsEquipment();
        [DllImport(PlatformConstants.dll)]
        public static extern void LibSetWornShowsEquipment(int new_value);
        [DllImport(PlatformConstants.dll)]
        public static extern void LibSetNoPetsPreference(int new_value);
        [DllImport(PlatformConstants.dll)]
        public static extern void LibSetAutoDig(int new_value);
        [DllImport(PlatformConstants.dll)]
        public static extern void LibSetIgnoreStopping(int new_value);
        [DllImport(PlatformConstants.dll)]
        public static extern void LibSetSpellHotbarAutoAdd(int on);
        [DllImport(PlatformConstants.dll)]
        public static extern void LibSetSpellHotbarAutoAddInitial(int on);
        [DllImport(PlatformConstants.dll)]
        public static extern int LibGetMouseCommand(int is_middle);
        [DllImport(PlatformConstants.dll)]
        public static extern void LibSetMouseCommand(int new_value, int is_middle);

        [DllImport(PlatformConstants.dll)]
        public static extern void LibSetEngraveQuickText([MarshalAs(GHConstants.GHStringMarshallingType)] string newValue);
        [DllImport(PlatformConstants.dll)]
        public static extern void LibSetEngraveQuickStyle(int newValue);

        [DllImport(PlatformConstants.dll)]
        public static extern IntPtr LibGetEventPathForGHSound(int ghsound);
        [DllImport(PlatformConstants.dll)]
        public static extern float LibGetVolumeForGHSound(int ghsound);

        [DllImport(PlatformConstants.dll)]
        public static extern void LibSetExitHack(int newValue);

        [DllImport(PlatformConstants.dll)]
        public static extern void LibExitGnhThread(int used_exit_hack_code);
        [DllImport(PlatformConstants.dll)]
        public static extern void LibTerminateGnollHack(int used_exit_hack_code);
        [DllImport(PlatformConstants.dll)]
        public static extern IntPtr LibGetCommandFunctionPointer(int cmd);
        [DllImport(PlatformConstants.dll)]
        public static extern void LibUpdateGameMusic();

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
                using (StreamReader sr = new StreamReader(fileStream))
#elif GNH_AVALONIA
                string fullsourcepath = Path.Combine(assetsourcedir, txtfile);
                using Stream fileStream = File.OpenRead(Path.Combine(GHAvaloniaPaths.AssetDirectory, fullsourcepath));
                using (StreamReader sr = new StreamReader(fileStream))
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
#elif GNH_AVALONIA
            return GHAvaloniaPaths.DataDirectory;
#else
            return ".";
#endif
        }


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
                string[] ghdirlist = { GHConstants.SaveDirectory, GHConstants.DumplogDirectory, GHConstants.SnapshotDirectory, GHConstants.AiDirectory };
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

                foreach (string txtfile in GHApp.TxtFilesList)
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
#elif GNH_AVALONIA
                    string fullsourcepath = Path.Combine(assetsourcedir, txtfile);
                    using Stream fileStream = File.OpenRead(Path.Combine(GHAvaloniaPaths.AssetDirectory, fullsourcepath));
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

                string versionKey = "";
                try
                {
                    string versionId = GetVersionId();
                    if (!string.IsNullOrEmpty(versionId))
                        versionKey = versionId + "|" + GHApp.GetPortVersionString() + "|" + GHApp.GetPortBuildString();
                }
                catch (Exception ex)
                {
                    System.Diagnostics.Debug.WriteLine(ex.Message);
                }

                foreach (string binfile in GHApp.BinFilesList)
                {
                    string fulltargetpath = Path.Combine(filesdir, binfile);
                    string stampkey = "BinFile_" + binfile + "_Stamp";
#if !DEBUG
                    /* Debug builds always copy, so dat/ edits take effect without a version change */
                    if (IsBinFileCurrent(fulltargetpath, stampkey, versionKey))
                        continue;
#endif

#if __IOS__
                    string extension = Path.GetExtension(binfile);
                    if (extension != null && extension.Length > 0)
                        extension = extension.Substring(1); /* Remove . from the start */
                    string fname = Path.GetFileNameWithoutExtension(binfile);
                    string fullsourcepath = NSBundle.MainBundle.PathForResource(fname, extension, assetsourcedir);
                    Stream src = File.OpenRead(fullsourcepath);
#elif __ANDROID__
                    string fullsourcepath = Path.Combine(assetsourcedir, binfile);
                    Stream src = assets.Open(fullsourcepath);
#elif WINDOWS
                    string fullsourcepath = Path.Combine(assetsourcedir, binfile);
                    Stream src = await FileSystem.Current.OpenAppPackageFileAsync(fullsourcepath);
#elif GNH_AVALONIA
                    string fullsourcepath = Path.Combine(assetsourcedir, binfile);
                    Stream src = File.OpenRead(Path.Combine(GHAvaloniaPaths.AssetDirectory, fullsourcepath));
#else
                    string fullsourcepath = Path.Combine(assetsourcedir, binfile);
                    Stream src = File.OpenRead(fullsourcepath);
#endif
                    /* Copied whole to a temporary file first, so an interrupted copy never replaces a good one */
                    string temppath = fulltargetpath + ".tmp";
                    using (src)
                    using (FileStream dst = new FileStream(temppath, FileMode.Create, FileAccess.Write))
                    {
                        src.CopyTo(dst);
                        dst.Flush(true);
                    }
                    if (File.Exists(fulltargetpath))
                        File.Delete(fulltargetpath);
                    File.Move(temppath, fulltargetpath);
                    if (versionKey != "")
                        Preferences.Set(stampkey, GetBinFileStamp(fulltargetpath, versionKey));
                }
            }
            catch (Exception)
            {
                await Task.Delay(5);
            }
        }

        /* The version key, length and last write time of a bin file, as stored after copying it */
        private static string GetBinFileStamp(string filepath, string versionKey)
        {
            FileInfo fileinfo = new FileInfo(filepath);
            return versionKey + "|" + fileinfo.Length.ToString(CultureInfo.InvariantCulture)
                + "|" + fileinfo.LastWriteTimeUtc.Ticks.ToString(CultureInfo.InvariantCulture);
        }

        /* True if the bin file exists and its stamp matches the one stored when it was copied for this version key */
        private static bool IsBinFileCurrent(string filepath, string stampkey, string versionKey)
        {
            try
            {
                if (versionKey == "" || !File.Exists(filepath))
                    return false;
                return Preferences.Get(stampkey, "") == GetBinFileStamp(filepath, versionKey);
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine(ex.Message);
                return false;
            }
        }

        public async Task InitializeFilesInSettings(Secrets settings)
        {
            if (settings == null)
                return;

            string filesdir = GetGnollHackPath();
#if __ANDROID__
            AssetManager assets = MainActivity.StaticAssets;
#endif
            foreach (SecretsDirectory sdir in settings.directories)
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
            foreach (SecretsFile sfile in settings.files)
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
#elif GNH_AVALONIA
                    using (Stream s = File.OpenRead(Path.Combine(GHAvaloniaPaths.AssetDirectory, fullsourcepath)))
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
                            try
                            {
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
                            }
                            catch (Exception ex)
                            {
                                System.Diagnostics.Debug.WriteLine(ex);
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

        public void InitializeTileData()
        {
            LibInitializeTileData();
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

#if GNH_MAUI
            int frame_idx = 0, main_tile_idx = 0, autodraw = autodraw_ptr;
            sbyte map_animated = 0; /* The engine reads this before writing it; every caller passes a new zero */
            int res;
            unsafe
            {
                res = LibMaybeGetAnimatedTile(ntile, tile_animation_idx, play_type, interval_counter,
                    &frame_idx, &main_tile_idx, &map_animated, &autodraw);
            }
            frame_idx_ptr = frame_idx;
            main_tile_idx_ptr = main_tile_idx;
            mapAnimated = map_animated;
            autodraw_ptr = autodraw;
            return res;
#else
            return LibMaybeGetAnimatedTile(ntile, tile_animation_idx, play_type, interval_counter,
                out frame_idx_ptr, out main_tile_idx_ptr, out mapAnimated, ref autodraw_ptr);
#endif
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
            string ret = GHUtils.PtrToEngineString(resptr);
            return ret;
        }
        public string GetVersionId()
        {
            IntPtr resptr = LibGetVersionId();
            string ret = GHUtils.PtrToEngineString(resptr);
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
            string ret = GHUtils.PtrToEngineString(resptr);
            return ret;
        }
        public string DumplogDateString(long startdate)
        {
            IntPtr resptr = LibDumplogDateString(startdate);
            string ret = GHUtils.PtrToEngineString(resptr);
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
#if WINDOWS || WINDOWS_AVALONIA
            if (!GHWindowsCodePage.CanMarshalPath(filename))
            {
                GHApp.MaybeWriteGHLog("Chmod: path cannot be passed in code page " + GHWindowsCodePage.AnsiCodePage);
                return -1;
            }
#endif
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

        public int GetMaxMajorConsultations()
        {
            string filesdir = GetGnollHackPath();
#if WINDOWS || WINDOWS_AVALONIA
            if (!GHWindowsCodePage.CanMarshalPath(filesdir))
            {
                GHApp.MaybeWriteGHLog("GetMaxMajorConsultations: path cannot be passed in code page " + GHWindowsCodePage.AnsiCodePage);
                return 0;
            }
#endif
            return LibGetMaxMajorConsultations(filesdir);
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
            GHApp.AddSentryBreadcrumb("ValidateSaveFile: " + (filename != null ? filename : "null"), GHConstants.SentryGnollHackGeneralCategoryName);

#if WINDOWS || WINDOWS_AVALONIA
            if (!GHWindowsCodePage.CanMarshalPath(filename))
            {
                GHApp.MaybeWriteGHLog("ValidateSaveFile: path cannot be passed in code page " + GHWindowsCodePage.AnsiCodePage);
                res_str = "The file path contains characters that cannot be passed to the game engine on this system.";
                return false;
            }
#endif
            byte[] buffer = new byte[256 * 4];
            Array.Clear(buffer, 0, buffer.Length);
            int res = LibValidateSaveFile(filename, buffer);
            if (buffer[0] == 0)
                res_str = "";
            else
                res_str = Encoding.UTF8.GetString(buffer);
            if (!string.IsNullOrEmpty(res_str))
            {
                int index = res_str.IndexOf('\0');
                if (index >= 0)
                    res_str = res_str.Remove(index); 
            }
            return res != 0;
        }

        public bool GetSaveFileInfo(string filename, out ulong version, out ulong compat, out uint saveFlags, out long timeStamp, out int glyph, out int guiGlyph, out long turnCount)
        {
            GHApp.AddSentryBreadcrumb("GetSaveFileInfo: " + (filename != null ? filename : "null"), GHConstants.SentryGnollHackGeneralCategoryName);

#if WINDOWS || WINDOWS_AVALONIA
            if (!GHWindowsCodePage.CanMarshalPath(filename))
            {
                GHApp.MaybeWriteGHLog("GetSaveFileInfo: path cannot be passed in code page " + GHWindowsCodePage.AnsiCodePage);
                version = 0;
                compat = 0;
                saveFlags = 0;
                timeStamp = 0;
                glyph = 0;
                guiGlyph = 0;
                turnCount = 0;
                return false;
            }
#endif

            ulong ver = 0;
            ulong comp = 0;
            uint flags = 0;
            long stamp = 0;
            int gl = 0;
            int gg = 0;
            long tc = 0;
            int res = LibGetSaveFileInfo(filename, ref ver, ref comp, ref flags, ref stamp, ref gl, ref gg, ref tc);
            version = ver;
            compat = comp;
            saveFlags = flags;
            timeStamp = stamp;
            glyph = gl;
            guiGlyph = gg;
            turnCount = tc;
            return res != 0;
        }

        public bool GetSaveFileDescription(string filename, out string characterDesc, out string locationDesc, out string modeDesc)
        {
            GHApp.AddSentryBreadcrumb("GetSaveFileDescription: " + (filename != null ? filename : "null"), GHConstants.SentryGnollHackGeneralCategoryName);

            characterDesc = "";
            locationDesc = "";
            modeDesc = "";

#if WINDOWS || WINDOWS_AVALONIA
            if (!GHWindowsCodePage.CanMarshalPath(filename))
            {
                GHApp.MaybeWriteGHLog("GetSaveFileDescription: path cannot be passed in code page " + GHWindowsCodePage.AnsiCodePage);
                return false;
            }
#endif

            byte[] charBuf = new byte[512];
            byte[] locBuf = new byte[512];
            byte[] modeBuf = new byte[512];

            int res = LibGetSaveFileDescription(filename, charBuf, charBuf.Length, locBuf, locBuf.Length, modeBuf, modeBuf.Length);
            if (res != 0)
            {
                characterDesc = Encoding.UTF8.GetString(charBuf);
                int idx = characterDesc.IndexOf('\0');
                if (idx >= 0) characterDesc = characterDesc.Remove(idx);
                characterDesc = characterDesc.Trim();

                locationDesc = Encoding.UTF8.GetString(locBuf);
                idx = locationDesc.IndexOf('\0');
                if (idx >= 0) locationDesc = locationDesc.Remove(idx);
                locationDesc = locationDesc.Trim();

                modeDesc = Encoding.UTF8.GetString(modeBuf);
                idx = modeDesc.IndexOf('\0');
                if (idx >= 0) modeDesc = modeDesc.Remove(idx);
                modeDesc = modeDesc.Trim();

                return true;
            }
            return false;
        }

        public int CheckCurrentFileDescriptor()
        {
            string filesdir = GetGnollHackPath();
#if WINDOWS || WINDOWS_AVALONIA
            if (!GHWindowsCodePage.CanMarshalPath(filesdir))
            {
                GHApp.MaybeWriteGHLog("CheckCurrentFileDescriptor: path cannot be passed in code page " + GHWindowsCodePage.AnsiCodePage);
                return -4;
            }
#endif
            return LibCheckCurrentFileDescriptor(filesdir);
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
        public string GenerateAiSnapshot()
        {
            IntPtr resptr = LibGenerateAiSnapshot();
            if (resptr == IntPtr.Zero)
                return null;
            string ret = Marshal.PtrToStringAnsi(resptr);
            return ret;
        }

        public bool GetCharacterClickAction()
        {
            return LibGetCharacterClickAction() != 0;
        }

        public void SetCharacterClickAction(bool newValue)
        {
            LibSetCharacterClickAction(newValue ? 1 : 0);
        }

        public void SetMetricSystem(bool newValue)
        {
            LibSetMetricSystem(newValue ? 1 : 0);
        }

        public bool GetGetPositionArrows()
        {
            return LibGetGetPositionArrows() != 0;
        }
        public void SetGetPositionArrows(bool newValue)
        {
            LibSetGetPositionArrows(newValue ? 1 : 0);
        }

        public bool GetDiceAsRanges()
        {
            return LibGetDiceAsRanges() != 0;
        }

        public void SetDiceAsRanges(bool newValue)
        {
            LibSetDiceAsRanges(newValue ? 1 : 0);
        }

        public void SetDamageFormula(bool newValue)
        {
            LibSetDamageFormula(newValue ? 1 : 0);
        }

        public bool GetWornShowsEquipment()
        {
            return LibGetWornShowsEquipment() != 0;
        }

        public void SetWornShowsEquipment(bool newValue)
        {
            LibSetWornShowsEquipment(newValue ? 1 : 0);
        }

        public void SetNoPetsPreference(bool newValue)
        {
            LibSetNoPetsPreference(newValue ? 1 : 0);
        }

        public void SetAutoDig(bool newValue)
        {
            LibSetAutoDig(newValue ? 1 : 0);
        }
        public void SetIgnoreStopping(bool newValue)
        {
            LibSetIgnoreStopping(newValue ? 1 : 0);
        }

        public void SetSpellHotbarAutoAdd(bool on)
        {
            LibSetSpellHotbarAutoAdd(on ? 1 : 0);
        }

        public void SetSpellHotbarAutoAddInitial(bool on)
        {
            LibSetSpellHotbarAutoAddInitial(on ? 1 : 0);
        }

        public int GetMouseCommand(bool isMiddle)
        {
            return LibGetMouseCommand(isMiddle ? 1 : 0);
        }

        public void SetMouseCommand(int newValue, bool isMiddle)
        {
            LibSetMouseCommand(newValue, isMiddle ? 1 : 0);
        }
        public void SetEngraveQuickText(string newValue)
        {
            LibSetEngraveQuickText(newValue);
        }
        public void SetEngraveQuickStyle(int newValue)
        {
            LibSetEngraveQuickStyle(newValue);
        }

        public string GetEventPathForGHSound(int ghsound)
        {
            IntPtr resptr = LibGetEventPathForGHSound(ghsound);
            string ret = GHUtils.PtrToEngineString(resptr);
            return ret;
        }

        public float GetVolumeForGHSound(int ghsound)
        {
            return LibGetVolumeForGHSound(ghsound);
        }

        public void SetExitHack(int newValue)
        {
            LibSetExitHack(newValue);
        }

        public void ExitGnhThread(exit_hack_types used_exit_hack_code)
        {
            LibExitGnhThread((int)used_exit_hack_code);
        }

        public void TerminateGnollHack(exit_hack_types used_exit_hack_code)
        {
            LibTerminateGnollHack((int)used_exit_hack_code);
        }
        public IntPtr GetCommandFunctionPointer(int cmd)
        {
            return LibGetCommandFunctionPointer(cmd);
        }

        public void UpdateGameMusic()
        {
            LibUpdateGameMusic();
        }

        public int StartGnollHack(GHGame ghGame)
        {
            GHApp.AddSentryBreadcrumb("StartGnollHack", GHConstants.SentryGnollHackGeneralCategoryName);
            GHApp.SetMirroredOptionsToDefaults();
            string filesdir = GetGnollHackPath();
#if DEBUG
            string simulatedFailure = System.Environment.GetEnvironmentVariable("GNOLLHACK_SIMULATE_START_FAILURE");
            if (simulatedFailure == "data-directory")
                return GHConstants.RunGnollHackErrorDataDirectory;
            if (simulatedFailure == "path")
                return GHConstants.RunGnollHackErrorPathNotRepresentable;
#endif
#if WINDOWS || WINDOWS_AVALONIA
            if (!GHWindowsCodePage.CanMarshalPath(filesdir))
                return GHConstants.RunGnollHackErrorPathNotRepresentable;
#endif
            bool allowbones = GHApp.AllowBones;
            bool getposarrows = GHApp.GetPositionArrows;
            bool nopet = GHApp.MirroredPetsNotGifted;
            bool characterclickaction = GHApp.MirroredCharacterClickAction;
            bool metricsystem = GHApp.MirroredMetricSystem;
            bool diceasranges = GHApp.MirroredDiceAsRanges;
            bool damageformula = GHApp.MirroredDamageFormula;
            bool wornshowsequipment = GHApp.MirroredWornShowsEquipment;
            bool autodig = GHApp.MirroredAutoDig;
            bool ignorestopping = GHApp.MirroredIgnoreStopping;
            bool defaultvikeys = GHApp.DefaultVIKeys;
            string engravetext = GHApp.MirroredEngraveQuickText;
            int rightmousebutton = GHApp.MirroredRightMouseCommand;
            int middlemousebutton = GHApp.MirroredMiddleMouseCommand;
            int engravequickstyle = GHApp.MirroredEngraveQuickStyle;
            //ulong rightmouse = (ulong)GHApp.MirroredRightMouseCommand << GHConstants.RightMouseBitIndex;
            //ulong middlemouse = (ulong)GHApp.MirroredMiddleMouseCommand << GHConstants.MiddleMouseBitIndex;
            ulong runflags = (ulong)(ghGame.WizardMode ? RunGnollHackFlags.WizardMode : 0) |
                (ulong)(GHApp.FullVersionMode ? RunGnollHackFlags.FullVersion : 0) |
                (ulong)(ghGame.ModernMode ? RunGnollHackFlags.ModernMode : 0) |
                (ulong)(ghGame.CasualMode ? RunGnollHackFlags.CasualMode : 0) |
                (ulong)(allowbones ? 0 : RunGnollHackFlags.DisableBones) |
                (ulong)(nopet ? RunGnollHackFlags.NoPet : 0) |
                (ulong)(GHApp.TournamentMode ? RunGnollHackFlags.TournamentMode : 0) |
                (ulong)(RunGnollHackFlags.SaveFileTrackingSupported) |
                (ulong)(GHApp.IsSaveFileTrackingNeeded ? RunGnollHackFlags.SaveFileTrackingNeeded : 0) |
                (ulong)(GHApp.SaveFileTracking ? RunGnollHackFlags.SaveFileTrackingOn : 0) |
                (ulong)(GHApp.IsDebug ? RunGnollHackFlags.GUIDebugMode : 0) |
                (ulong)(getposarrows ? RunGnollHackFlags.GetPositionArrows : 0) | /* Set the iflag to right value */
                (ulong)(characterclickaction ? RunGnollHackFlags.CharacterClickAction : 0) | /* Use the default; GHApp.CharacterClickAction may contain the option value from the last game */
                (ulong)(metricsystem ? RunGnollHackFlags.MetricSystem : 0) |
                (ulong)(diceasranges ? RunGnollHackFlags.DiceAsRanges : 0) |
                (ulong)(damageformula ? RunGnollHackFlags.DamageFormula : 0) |
                (ulong)(wornshowsequipment ? RunGnollHackFlags.WornShowsEquipment : 0) |
                (ulong)(autodig ? RunGnollHackFlags.AutoDig : 0) | /* Use the default; GHApp.AutoDig may contain the option value from the last game */
                (ulong)(GHApp.SpellFavoritesAutoAdd ? RunGnollHackFlags.SpellHotbarAutoAdd : 0) |
                (ulong)(GHApp.SpellFavoritesInitialAdd ? RunGnollHackFlags.SpellHotbarAutoAddInitial : 0) |
                (ulong)(ignorestopping ? RunGnollHackFlags.IgnoreStopping : 0) | /* Use the default; GHApp.IgnoreStopping may contain the option value from the last game */
                (ulong)(defaultvikeys ? RunGnollHackFlags.DefaultVIKeys : 0) | /* Use the default */
                (ulong)ghGame.StartFlags;
            ulong foundManuals = GHApp.FoundManuals;
            string lastusedplname = GHApp.TournamentMode && !ghGame.PlayingReplay ? GHApp.LastUsedTournamentPlayerName : GHApp.LastUsedPlayerName;

#if GNH_MAUI
            _callbackGame = ghGame;
            Interlocked.Exchange(ref _callbackFailureReported, 0);
            try
            {
                unsafe
                {
#endif
            return RunGnollHack(
                filesdir,
                "",
                engravetext,
                lastusedplname,
                rightmousebutton,
                middlemousebutton,
                engravequickstyle,
                0,
                runflags,
                foundManuals,
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
#if GNH_MAUI
                &NativePutStrEx,
                &NativePutStrEx2,
                &NativePutStrEx,
#else
                ghGame.ClientCallback_PutStrEx,
                ghGame.ClientCallback_PutStrEx2,
                ghGame.ClientCallback_PutStrEx,
#endif
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
#if GNH_MAUI
                &NativePrintGlyph,
                &NativeIssueGuiCommand,
#else
                ghGame.ClientCallback_PrintGlyph,
                ghGame.ClientCallback_IssueGuiCommand,
#endif
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
#if GNH_MAUI
                &NativeStatusEnable,
                &NativeStatusUpdate,
#else
                ghGame.ClientCallback_StatusEnable,
                ghGame.ClientCallback_StatusUpdate,
#endif
                ghGame.ClientCallback_BooleanVoidDummy,
                ghGame.ClientCallback_VoidVoidDummy,
                ghGame.ClientCallback_VoidUlongDummy,
                ghGame.ClientCallback_OpenSpecialView,
                ghGame.ClientCallback_StopAllSounds,
#if GNH_MAUI
                &NativePlayImmediateSound,
#else
                ghGame.ClientCallback_PlayImmediateSound,
#endif
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
#if GNH_MAUI
                &NativeClearContextMenu,
                &NativeAddContextMenu,
#else
                ghGame.ClientCallback_ClearContextMenu,
                ghGame.ClientCallback_AddContextMenu,
#endif
                ghGame.ClientCallback_UpdateStatusButton,
#if GNH_MAUI
                &NativeToggleAnimationTimer,
                &NativeDisplayFloatingText,
#else
                ghGame.ClientCallback_ToggleAnimationTimer,
                ghGame.ClientCallback_DisplayFloatingText,
#endif
                ghGame.ClientCallback_DisplayScreenText,
                ghGame.ClientCallback_DisplayPopupText,
                ghGame.ClientCallback_DisplayGUIEffect,
                ghGame.ClientCallback_UpdateCursor,
#if GNH_MAUI
                &NativeUIHasInput,
#else
                ghGame.ClientCallback_UIHasInput,
#endif
#if GNH_MAUI
                &NativeExitHack,
#else
                ghGame.ClientCallback_ExitHack,
#endif
                ghGame.ClientCallback_GetCwd,
                ghGame.ClientCallback_MessageBox,
                ghGame.ClientCallback_FreeMemory,
                ghGame.ClientCallback_ReportPlayerName,
                ghGame.ClientCallback_ReportPlayTime,
#if GNH_MAUI
                &NativeSendObjectData,
                &NativeSendMonsterData,
#else
                ghGame.ClientCallback_SendObjectData,
                ghGame.ClientCallback_SendMonsterData,
#endif
                ghGame.ClientCallback_SendEngravingData
                );
#if GNH_MAUI
                }
            }
            finally
            {
                Interlocked.CompareExchange(ref _callbackGame, null, ghGame);
            }
#endif

        }
    }
}
