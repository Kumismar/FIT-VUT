using project.App.ViewModels;
namespace project.App.Views;

public partial class ProjectDetailView
{
	public ProjectDetailView(ProjectDetailViewModel viewModel) : base(viewModel)
	{
		InitializeComponent();
	}
}