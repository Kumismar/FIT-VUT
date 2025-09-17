using project.App.ViewModels;

namespace project.App.Views;

public partial class MenuView
{
    public MenuView(MenuViewModel viewModel)
        : base(viewModel)
    {
        InitializeComponent();
    }
}