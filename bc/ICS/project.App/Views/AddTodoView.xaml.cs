using project.App.ViewModels;

namespace project.App.Views;

public partial class AddTodoView : ContentPageBase
{
    public AddTodoView(AddTodoViewModel viewModel) : base(viewModel)
    {
        InitializeComponent();
    }
}