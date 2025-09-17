using project.App.ViewModels;

namespace project.App.Views;

public partial class MainView
{
    public MainView(MainViewModel viewModel)
        : base(viewModel)
    {
        InitializeComponent();
    }
}