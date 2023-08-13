using GnollHackX;

namespace GnollHackM;

public partial class App : Application
{
	public App()
	{
		InitializeComponent();
        GHApp.Initialize();

		MainPage = new AppShell();
	}

    protected override void OnStart()
    {
        base.OnStart();
        GHApp.OnStart();
    }

    protected override void OnSleep()
    {
        base.OnSleep();
        GHApp.OnSleep();
    }

    protected override void OnResume()
    {
        base.OnResume();
        GHApp.OnResume();
    }
}
