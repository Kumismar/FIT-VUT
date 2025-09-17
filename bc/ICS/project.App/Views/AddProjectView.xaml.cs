using project.App.ViewModels;

namespace project.App.Views;

public partial class AddProjectView : ContentPageBase
{
    public AddProjectView(AddProjectViewModel viewModel) : base(viewModel)
    {
        InitializeComponent();
    }
}