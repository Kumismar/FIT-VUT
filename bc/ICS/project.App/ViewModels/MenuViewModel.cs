using CommunityToolkit.Mvvm.Input;
using project.App.Services.Interfaces;
using project.App.Views;
using project.BL.Facades;
using project.BL.Facades.Interfaces;

namespace project.App.ViewModels;

[QueryProperty(nameof(UserId), nameof(UserId))]
public partial class MenuViewModel : ViewModelBase
{
    private readonly INavigationService _navigationService;
    private readonly IAlertService _alertService;
    private readonly IUserFacade _userFacade;
    public Guid UserId { get; set; }

    public MenuViewModel(
        IMessengerService messengerService,
        INavigationService navigationService,
        IAlertService alertService, 
        IUserFacade userFacade)
        :base(messengerService)
    {
        _navigationService = navigationService;
        _alertService = alertService;
        _userFacade = userFacade;
    }

    [RelayCommand]
    private async void GoToActivitiesList()
    {
        await _navigationService.GoToAsync<ActivitiesListViewModel>(
                new Dictionary<string, object?> { [nameof(ActivitiesListViewModel.UserId)] = UserId });
    }

    [RelayCommand]
    private async void GoToTodoList()
    {
        await _navigationService.GoToAsync<TodoListViewModel>(
                new Dictionary<string, object?> { [nameof(TodoListViewModel.UserId)] = UserId });
    }
    
    [RelayCommand]
    private async void GoToProjectList()
    {
        await _navigationService.GoToAsync<ProjectListViewModel>(
                new Dictionary<string, object?> { [nameof(ProjectListViewModel.UserId)] = UserId });
    }

    [RelayCommand]
    private async void DeleteUser()
    {
        bool answer =  await _alertService.DisplayYesOrNo("Hupsik Dupsik?", "Do you really want to delete user?");
        if (answer == true)
        {
            await _userFacade.DeleteAsync(UserId);
            await _navigationService.GoToAsync<MainViewModel>();
        }
    }
}
