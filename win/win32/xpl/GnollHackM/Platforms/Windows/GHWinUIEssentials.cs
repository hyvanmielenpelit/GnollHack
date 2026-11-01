using GnollHackX;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Maui.ApplicationModel.DataTransfer;
using Microsoft.Maui.Hosting;
using Microsoft.Maui.Storage;

namespace GnollHackM
{
    /* Registers wrappers of Essentials' FilePicker and Share, so that a controller gets the in-app file dialog. MAUI hands
       registered implementations to the static facades when the app is built; the factories run then, and wrap the
       platform defaults the facades still hold. */
    public static class GHWinUIEssentials
    {
        public static void Register(MauiAppBuilder builder)
        {
            builder.Services.AddSingleton<IFilePicker>(services => new GHWinUIFilePicker(FilePicker.Default));
            builder.Services.AddSingleton<IShare>(services => new GHWinUIShare(Share.Default));
        }
    }
}
