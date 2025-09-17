using project.App.ViewModels;

namespace project.App.Views;

public partial class AddUserView
{
    public AddUserView(AddUserViewModel viewModel)
        : base(viewModel)
    {
        InitializeComponent();
    }
}