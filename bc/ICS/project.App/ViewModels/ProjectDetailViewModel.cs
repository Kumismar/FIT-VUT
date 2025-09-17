using CommunityToolkit.Mvvm.Input;
using project.App.Services.Interfaces;
using project.BL.Facades;
using project.BL.Facades.Interfaces;
using project.BL.Models;
using System.Collections.ObjectModel;

namespace project.App.ViewModels;

[QueryProperty(nameof(UserId), nameof(UserId))]
[QueryProperty(nameof(ProjectId), nameof(ProjectId))]
public partial class ProjectDetailViewModel : ViewModelBase
{
    private readonly IUserProjectFacade _userProjectFacade;
    private readonly INavigationService _navigationService;
    public Guid ProjectId { get; set; }
    public Guid UserId { get; set; }


    public ProjectDetailViewModel(IMessengerService messengerService,
       IUserProjectFacade userProjectFacade,
       INavigationService navigationService) : base(messengerService)
    {
        _userProjectFacade = userProjectFacade;
        _navigationService = navigationService;
    }

    [RelayCommand]
    private async void AddUserToProject()
    {
        await _userProjectFacade.SaveAsync(UserId, ProjectId);
    }
}
