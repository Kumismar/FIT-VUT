using CommunityToolkit.Mvvm.Input;
using project.App.Services.Interfaces;

namespace project.App;

public partial class AppShell : Shell
{
    private readonly INavigationService _navigationService;
    public AppShell(INavigationService navigationService)
    {
        _navigationService = navigationService;
        InitializeComponent();
    }
}