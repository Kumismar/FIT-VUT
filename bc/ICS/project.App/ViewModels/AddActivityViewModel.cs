using CommunityToolkit.Mvvm.Input;
using project.App.Messages;
using project.App.Services.Interfaces;
using project.BL.Facades.Interfaces;
using project.BL.Models;
using project.BL;

namespace project.App.ViewModels;

[QueryProperty(nameof(UserId), nameof(UserId))]
public partial class AddActivityViewModel : ViewModelBase
{
    private readonly IActivityFacade _activityFacade;
    private readonly INavigationService _navigationService;
    private readonly IAlertService _alertService;
    public Guid UserId { get; set; }
    public int ColorIndex { get; set; } = 0;

    public TimeSpan TimeFrom { get; set; }
    public TimeSpan TimeTo { get; set; }
    public ActivityDetailModel ActivityDetailModel { get; set; } = ActivityDetailModel.Empty;
	public DateTime Today { get; } = DateTime.Today;
	public DateTime LastYear { get; } = DateTime.Today.AddYears(-1);
	public DateTime NextYear { get; } = DateTime.Today.AddYears(1);
    public AddActivityViewModel(
        IMessengerService messengerService,
        IActivityFacade activityFacade,
        INavigationService navigationService,
        IAlertService alertService)
        : base(messengerService)
    {
        _activityFacade = activityFacade;
        _navigationService = navigationService;
        _alertService = alertService;
    }

    [RelayCommand]
    public async Task SaveActivityAsync()
    {
        ActivityDetailModel.DateTimeFrom += TimeFrom;
        ActivityDetailModel.DateTimeTo += TimeTo;
        if (ActivityDetailModel.Name == string.Empty)
        {
            await _alertService.DisplayAsync("Hupsik Dupsik", "Please enter activity name");
        }
        else if (ActivityDetailModel.DateTimeFrom > ActivityDetailModel.DateTimeTo)
        {
            await _alertService.DisplayAsync("Hupsik Dupsik", "Activity ends before it starts");
        }
        else
        {

            ActivityDetailModel.Color = IndexToColor(ColorIndex);
            try
            {
                await _activityFacade.SaveAsync(ActivityDetailModel, UserId, null);
                messengerService.Send(new ActivityAddMessage());
                ActivityDetailModel = ActivityDetailModel.Empty;
                await _navigationService.GoToAsync<ActivitiesListViewModel>(
                    new Dictionary<string, object?> { [nameof(ActivitiesListViewModel.UserId)] = UserId });
            }
            catch (OverlappingException)
            {
                await _alertService.DisplayAsync("Hupsik Dupsik", "Activites are overlapping");
            }
        }
    }
    private System.Drawing.Color IndexToColor(int index)
    {
        switch (index)
        {
            case 0:
                return System.Drawing.Color.Red;
            case 1:
                return System.Drawing.Color.Blue;
            case 2:
                return System.Drawing.Color.Yellow;
            case 3:
                return System.Drawing.Color.Purple;
            case 4:
                return System.Drawing.Color.Pink;
            case 5:
                return System.Drawing.Color.Orange;
            case 6:
                return System.Drawing.Color.Brown;
            default: break;
        }
        return System.Drawing.Color.Black;
    }
}

