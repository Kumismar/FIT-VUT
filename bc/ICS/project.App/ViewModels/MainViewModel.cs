using CommunityToolkit.Mvvm.Input;
using project.BL.Models;
using project.App.Services.Interfaces;
using project.BL.Facades.Interfaces;
using CommunityToolkit.Mvvm.Messaging;
using project.App.Messages;
using System.Collections.ObjectModel;

namespace project.App.ViewModels;

public partial class MainViewModel : ViewModelBase, 
    IRecipient<UserAddMessage>,
    IRecipient<UserDeleteMessage>
{
    private IUserFacade _userFacade { get; init; }
    private INavigationService _navigationService { get; init; }
    public Guid Id { get; set; }
    public ObservableCollection<UserListModel> Users { get; set; } 
    
    public MainViewModel(
        IMessengerService messengerService,
        IUserFacade userFacade,
        INavigationService navigationService
        ) : base(messengerService)
    {
        _userFacade = userFacade;
        _navigationService = navigationService;
    }

    [RelayCommand]
    private async void GoToAddUser()
    {
        await _navigationService.GoToAsync("main/newUser");
    }

    [RelayCommand]
    private async void GoToActivities(Guid Id)
    {
        await _navigationService.GoToAsync<MenuViewModel>(
            new Dictionary<string, object?> { [nameof(MenuViewModel.UserId)] = Id });
    }

    protected override async Task LoadDataAsync()
    {
        var tmpUsers = await _userFacade.GetAsync();
        Users = tmpUsers.ToObservableCollection();
    }

    public async void Receive(UserAddMessage message)
    {
        await LoadDataAsync();
    }

    public async void Receive(UserDeleteMessage message)
    {
        await LoadDataAsync();
    }
}