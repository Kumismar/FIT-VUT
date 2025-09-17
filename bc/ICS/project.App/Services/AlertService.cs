using project.App.Services.Interfaces;

namespace project.App.Services;

public class AlertService : IAlertService
{
    public async Task DisplayAsync(string title, string message)
    {
        var displayAlert = Application.Current?.MainPage?.DisplayAlert(title, message, "OK");
        
        if (displayAlert is not null)
        {
            await displayAlert;
        }
    }

    public async Task<bool> DisplayYesOrNo(string title, string message)
    {
        var displayAlert = Application.Current?.MainPage?.DisplayAlert(title, message, "Yes", "No");
        if (displayAlert is not null)
        {
            bool answer = await displayAlert;
            return answer;
        }
        return false;
    }
}