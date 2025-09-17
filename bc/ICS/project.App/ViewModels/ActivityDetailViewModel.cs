using System.Diagnostics;
using CommunityToolkit.Mvvm.Input;
using project.App.Messages;
using project.App.Services.Interfaces;
using project.BL.Facades;
using project.BL.Facades.Interfaces;
using project.BL.Models;

namespace project.App.ViewModels;

[QueryProperty(nameof(ActivityId), nameof(ActivityId))]
[QueryProperty(nameof(UserId), nameof(UserId))]
public partial class ActivityDetailViewModel : ViewModelBase
{
    private readonly IActivityFacade _activityFacade;
    private readonly INavigationService _navigationService;
    public Guid ActivityId { get; set; }
    public Guid UserId { get; set; }
    public ActivityDetailModel Activity { get; set; } = ActivityDetailModel.Empty;
    public ActivityDetailViewModel(
        INavigationService navigationService,
        IMessengerService messengerService,
        IActivityFacade activityFacade)
        : base(messengerService)
    {
        _activityFacade = activityFacade;
        _navigationService = navigationService;
    }
    protected override async Task LoadDataAsync()
    {
        await base.LoadDataAsync();

        Activity = await _activityFacade.GetAsync(ActivityId);
    }

    [RelayCommand]
    public async void DeleteActivity(Guid Id)
    {
        await _activityFacade.DeleteAsync(Id);
        messengerService.Send(new ActivityDeleteMessage());
        await _navigationService.GoToAsync<ActivitiesListViewModel>(
            new Dictionary<string, object?> { [nameof(ActivitiesListViewModel.UserId)] = UserId });
    }
}