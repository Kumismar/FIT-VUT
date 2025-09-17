using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.Messaging;
using project.App.Messages;
using project.App.Services.Interfaces;
using project.BL.Facades.Interfaces;
using project.BL.Models;
using System.Collections.ObjectModel;

namespace project.App.ViewModels;

[QueryProperty(nameof(UserId), nameof(UserId))]
public partial class TodoListViewModel : ViewModelBase,
    IRecipient<TodoAddMessage>,
    IRecipient<TodoDeleteMessage>
{
    private readonly ITodoFacade _todoFacade;
    private readonly INavigationService _navigationService;
    public Guid UserId { get; set; }
    public ObservableCollection<TodoDetailModel> Todos { get; set; } = new();
    public TodoListViewModel(IMessengerService messengerService,
        ITodoFacade todoFacade,
        INavigationService navigationService) : base(messengerService)
    {
        _navigationService = navigationService;
        _todoFacade = todoFacade;
    }
    protected override async Task LoadDataAsync()
    {
        var todos = await _todoFacade.GetAsyncUser(UserId);
        Todos = todos.ToObservableCollection();
    }

    [RelayCommand]
    private async void GoToAddTodo()
    {
        await _navigationService.GoToAsync<AddTodoViewModel>(
                new Dictionary<string, object?> { [nameof(AddTodoViewModel.UserId)] = UserId });
    }
    [RelayCommand]
    public async Task SaveAllTodos()
    {
        foreach (var Todo in Todos)
        {
            await _todoFacade.SaveAsync(Todo, UserId);
        }
    }
    [RelayCommand]
    public async Task DeleteTodo(Guid TodoID)
    {
        await _todoFacade.DeleteAsync(TodoID);
        messengerService.Send(new TodoDeleteMessage());
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