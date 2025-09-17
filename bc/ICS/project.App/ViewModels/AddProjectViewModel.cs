using CommunityToolkit.Mvvm.Input;
using project.App.Messages;
using project.App.Services.Interfaces;
using project.BL.Facades;
using project.BL.Facades.Interfaces;
using project.BL.Models;

namespace project.App.ViewModels;

[QueryProperty(nameof(UserId), nameof(UserId))]
public partial class AddProjectViewModel : ViewModelBase
{
    private readonly IProjectFacade _projectFacade;
    private readonly INavigationService _navigationService;

    public Guid UserId { get; set; }
    public ProjectDetailModel Project { get; set; } = ProjectDetailModel.Empty;
    public AddProjectViewModel(
        IMessengerService messengerService,
       IProjectFacade projectFacade,
       INavigationService navigationService) : base(messengerService)
    {
        _projectFacade = projectFacade;
        _navigationService = navigationService;
    }

    [RelayCommand]
    public async Task SaveProjectAsync()
    {
        await _projectFacade.SaveAsync(Project);
        messengerService.Send(new ProjectAddMessage());
        await _navigationService.GoToAsync<ProjectListViewModel>(
            new Dictionary<string, object?> { [nameof(ProjectListViewModel.UserId)] = UserId });
    }
    public async void Receive(ProjectAddMessage message)
    {
        await LoadDataAsync();
    }

    public async void Receive(ProjectDeleteMessage message)
    {
        await LoadDataAsync();
    }

}
