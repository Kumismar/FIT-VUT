using project.App.Models;
using project.App.ViewModels;

namespace project.App.Services.Interfaces;

public interface INavigationService
{
    IEnumerable<RouteModel> Routes { get; }

#pragma warning disable CS8632 // The annotation for nullable reference types should only be used in code within a '#nullable' annotations context.
    Task GoToAsync<TViewModel>(IDictionary<string, object?> parameters)
#pragma warning restore CS8632 // The annotation for nullable reference types should only be used in code within a '#nullable' annotations context.
        where TViewModel : IViewModel;

    Task GoToAsync(string route);
    bool SendBackButtonPressed();
#pragma warning disable CS8632 // The annotation for nullable reference types should only be used in code within a '#nullable' annotations context.
    Task GoToAsync(string route, IDictionary<string, object?> parameters);
#pragma warning restore CS8632 // The annotation for nullable reference types should only be used in code within a '#nullable' annotations context.

    Task GoToAsync<TViewModel>()
        where TViewModel : IViewModel;
}
