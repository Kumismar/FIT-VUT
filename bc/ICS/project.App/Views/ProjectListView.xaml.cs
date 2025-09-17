using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using project.App.ViewModels;

namespace project.App.Views;

public partial class ProjectListView
{
    public ProjectListView(ProjectListViewModel viewModel):base(viewModel)
    {
        InitializeComponent();
    }
}