using Microsoft.Extensions.Logging;
using project.App.Views;
using CommunityToolkit.Maui;
using project.BL;
using Microsoft.Extensions.Configuration;
using System.Reflection;
using project.App.Services.Interfaces;

namespace project.App;

public static class MauiProgram
{
    public static MauiApp CreateMauiApp()
    {
        var builder = MauiApp.CreateBuilder();
        builder.UseMauiApp<App>().ConfigureFonts(fonts =>
        {
            fonts.AddFont("OpenSans-Regular.ttf", "OpenSansRegular");
            fonts.AddFont("OpenSans-Semibold.ttf", "OpenSansSemibold");
            fonts.AddFont("Free-Regular-400.otf", "FAR");
            fonts.AddFont("Free-Solid-900.otf", "FAS");
        }).UseMauiCommunityToolkit();

        ConfigureAppSettings(builder);

        builder.Services
            .AddDALServices(builder.Configuration)
            .AddBLServices()
            .AddAppServices();

#if DEBUG
		builder.Logging.AddDebug();
#endif

        var app = builder.Build();

        app.Services.GetRequiredService<IDbMigrator>().Migrate();
        RegisterRouting(app.Services.GetRequiredService<INavigationService>());

        return app;
    }

    private static void ConfigureAppSettings(MauiAppBuilder builder)
    {
        var configurationBuilder = new ConfigurationBuilder();

        var assembly = Assembly.GetExecutingAssembly();
        const string appSettingsFilePath = "project.App.appSettings.json";
        using var appSettingsStream = assembly.GetManifestResourceStream(appSettingsFilePath);
        if (appSettingsStream is not null)
        {
            configurationBuilder.AddJsonStream(appSettingsStream);
        }

        var configuration = configurationBuilder.Build();
        builder.Configuration.AddConfiguration(configuration);
    }

    private static void RegisterRouting(INavigationService navigationService)
    {
        foreach (var route in navigationService.Routes)
        {
            Routing.RegisterRoute(route.Route, route.ViewType);
        }
    }
}