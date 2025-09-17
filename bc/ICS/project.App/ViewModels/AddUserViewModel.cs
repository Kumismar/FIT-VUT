using CommunityToolkit.Mvvm.Input;
using project.App.Messages;
using project.App.Services.Interfaces;
using project.BL.Facades.Interfaces;
using project.BL.Models;

namespace project.App.ViewModels;

public partial class AddUserViewModel : ViewModelBase
{
    private readonly IUserFacade _userFacade;
    private readonly INavigationService _navigationService;
    private readonly IAlertService _alertService;
    public UserDetailModel User { get; set; } = UserDetailModel.Empty;
    public AddUserViewModel(
        IMessengerService messengerService, 
        IUserFacade userFacade,
        INavigationService navigationService, 
        IAlertService alertService)
        : base(messengerService)
    {
        _userFacade = userFacade;
        _navigationService = navigationService;
        _alertService = alertService;
    }

    [RelayCommand]
    public async Task SaveUserAsync()
    {
        if (User.UserName.Length > 3 && User.UserName.Length < 16) 
        { 
            await _userFacade.SaveAsync(User);
            messengerService.Send(new UserAddMessage());
            _navigationService.SendBackButtonPressed();
        }
        else
        {
            await _alertService.DisplayAsync("Hupsik Dupsik", "Username must have length between 4 and 15");
        }
    }
}
