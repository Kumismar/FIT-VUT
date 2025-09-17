using System.Collections.ObjectModel;
using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;
using project.App.Messages;
using project.App.Services.Interfaces;
using project.BL.Facades.Interfaces;
using project.BL.Models;

namespace project.App.ViewModels;

[QueryProperty(nameof(UserId), nameof(UserId))]
public partial class ProjectListViewModel : ViewModelBase,
    IRecipient<ProjectAddMessage>,
    IRecipient<ProjectDeleteMessage>
{
    private readonly IProjectFacade _projectFacade;
    private readonly INavigationService _navigationService;
    public Guid UserId { get; set; }
    public Guid ProjectId { get; set; }

    public ObservableCollection<ProjectListModel> Projects { get; set; } = new();
    public ProjectListViewModel(IMessengerService messengerService,
        IProjectFacade projectFacade,
        INavigationService navigationService) : base(messengerService)
    {
        _navigationService = navigationService;
        _projectFacade = projectFacade;
    }

    protected override async Task LoadDataAsync()
    {
        var projects = await _projectFacade.GetAsync();
        Projects = projects.ToObservableCollection();
    }

    [RelayCommand]
    private async void GoToAddProject()
    {
        await _navigationService.GoToAsync<AddProjectViewModel>(
                new Dictionary<string, object?> { [nameof(AddProjectViewModel.UserId)] = UserId });
    }
    [RelayCommand]
    private async void GoToDetailProject()
    {
        await _navigationService.GoToAsync<ProjectDetailViewModel>(
                new Dictionary<string, object?> { [nameof(ProjectDetailViewModel.UserId)] = UserId, [nameof(ProjectDetailViewModel.ProjectId)] = ProjectId });
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
