using Microsoft.IdentityModel.Tokens;
using project.BL.Facades;
using project.BL.Facades.Interfaces;
using project.BL.Models;
using project.BL.tests.ModelSeeds;
using project.DAL.Tests;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xunit.Abstractions;

namespace project.BL.tests;
public class ProjectFacadeTests : FacadeTestsBase
{
    private readonly ProjectFacade _projectFacade;
    public ProjectFacadeTests(ITestOutputHelper output) : base(output)
    {
        _projectFacade = new ProjectFacade(UnitOfWorkFactory, ProjectModelMapper);
    }

    [Fact]
    public async Task CreateProject_Success()
    {
        // Arrange
        var projectModel = ProjectSeeds.ProjectSeed();

        // Act
        var returnedProject = await _projectFacade.SaveAsync(projectModel);

        var DbProject = await _projectFacade.GetAsync(returnedProject.Id);

        // Assert
        DeepAssert.Equal(returnedProject, DbProject);
    }


    [Fact]
    public async Task UpdateProject_Success()
    {
        // Arrange
        var projectModel = ProjectSeeds.ProjectSeed();

        // Act
        var returnedProject = await _projectFacade.SaveAsync(projectModel);
        var DbProject = await _projectFacade.GetAsync(returnedProject.Id);

        Assert.NotNull(DbProject);

        DbProject.Description = "Getting netherite";
        await _projectFacade.SaveAsync(DbProject);

        var DbProjectUpdated = await _projectFacade.GetAsync(DbProject.Id);

        DeepAssert.Equal(DbProject, DbProjectUpdated);
    }


    [Fact]
    public async Task GetNonExistentProject_Null()
    {
        // Arrange
        var projectModel = ProjectSeeds.ProjectSeed();

        // Act
        var DbProject = await _projectFacade.GetAsync(projectModel.Id);

        // Assert
        Assert.Null(DbProject);
    }


    [Fact]
    public async Task DeleteProject_Success()
    {
        // Arrange
        var projectModel = ProjectSeeds.ProjectSeed();

        // Act
        var returnedProject = await _projectFacade.SaveAsync(projectModel);

        await _projectFacade.DeleteAsync(returnedProject.Id);

        var DbProject = await _projectFacade.GetAsync(returnedProject.Id);

        // Assert
        Assert.Null(DbProject);
    }


    [Fact]
    public async Task DeleteNonExistingProject_Success()
    {
        // Arrange
        var projectModel = ProjectSeeds.ProjectSeed();

        // Act & Assert
        await Assert.ThrowsAsync<InvalidOperationException>(async () => await _projectFacade.DeleteAsync(projectModel.Id));
    }


    [Fact]
    public async Task GetListModels_Correct()
    {
        // Arrange
        var projectModel1 = ProjectSeeds.ProjectSeed();
        var projectModel2 = ProjectSeeds.ProjectSeed();
        var projectModel3 = ProjectSeeds.ProjectSeed();

        // Act
        var returnedProject1 = await _projectFacade.SaveAsync(projectModel1);
        var returnedProject2 = await _projectFacade.SaveAsync(projectModel2);
        var returnedProject3 = await _projectFacade.SaveAsync(projectModel3);

        var projectList = await _projectFacade.GetAsync();

        var project1Listmodel = new ProjectListModel()
        {
            Id = returnedProject1.Id,
            Name = returnedProject1.Name
        };

        var project2Listmodel = new ProjectListModel()
        {
            Id = returnedProject2.Id,
            Name = returnedProject2.Name
        };

        var project3Listmodel = new ProjectListModel()
        {
            Id = returnedProject3.Id,
            Name = returnedProject3.Name
        };

        var projectNotInDb = new ProjectListModel()
        {
            Id = Guid.NewGuid(),
            Name = "ICSprojekt"
        };

        Assert.Contains(project1Listmodel, projectList);
        Assert.Contains(project2Listmodel, projectList);
        Assert.Contains(project3Listmodel, projectList);

        Assert.DoesNotContain(projectNotInDb, projectList);
    }


    [Fact]
    public async Task GetListModels_Empty()
    {
        var ProjectList = await _projectFacade.GetAsync();

        Assert.Empty(ProjectList);
    }
}

