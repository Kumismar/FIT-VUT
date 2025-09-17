using project.App.ViewModels;

namespace project.App.Views;

public partial class ActivitiesListView
{
	public ActivitiesListView(ActivitiesListViewModel viewModel) : base(viewModel)
	{
		InitializeComponent();
	}
}