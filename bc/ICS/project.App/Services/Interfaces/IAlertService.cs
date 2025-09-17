namespace project.App.Services.Interfaces;

public interface IAlertService
{
    Task DisplayAsync(string title, string message);
    Task<bool> DisplayYesOrNo(string title, string message);
}
