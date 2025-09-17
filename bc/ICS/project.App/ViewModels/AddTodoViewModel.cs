using CommunityToolkit.Mvvm.Input;
using project.App.Messages;
using project.App.Services.Interfaces;
using project.BL.Facades.Interfaces;
using project.BL.Models;

namespace project.App.ViewModels;

[QueryProperty(nameof(UserId), nameof(UserId))]
public partial class AddTodoViewModel : ViewModelBase
{
    private readonly ITodoFacade _todoFacade;
    private readonly INavigationService _navigationService;
    private readonly IAlertService _alertService;
    public Guid UserId { get; set; }

    public DateTime Time { get; set; }
    public DateTime Today { get; set; } = DateTime.Today;
    public TodoDetailModel Todo { get; set; } = TodoDetailModel.Empty;

    public AddTodoViewModel(
        IMessengerService messengerService,
        ITodoFacade todoFacade, 
        INavigationService navigationService,
        IAlertService alertService) 
        : base(messengerService)
    {
        _todoFacade = todoFacade;
        _navigationService = navigationService;
        _alertService = alertService;
    }

    [RelayCommand]
    public async Task SaveTodoAsync()
    {
        Todo.Date = DateOnly.FromDateTime(Time);
        Todo.Finished = false;
        await _todoFacade.SaveAsync(Todo, UserId);
        messengerService.Send(new TodoAddMessage());
        await _navigationService.GoToAsync<TodoListViewModel>(
            new Dictionary<string, object?> { [nameof(AddTodoViewModel.UserId)] = UserId });
    }

    public async void Receive(TodoAddMessage message)
    {
        await LoadDataAsync();
    }

    public async void Receive(TodoDeleteMessage message)
    {
        await LoadDataAsync();
    }
}