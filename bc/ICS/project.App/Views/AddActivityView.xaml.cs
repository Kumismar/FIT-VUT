using project.App.ViewModels;

namespace project.App.Views;

public partial class AddActivityView : ContentPageBase
{
	public AddActivityView(AddActivityViewModel viewModel) : base(viewModel)
	{
		InitializeComponent();
	}
}