using Microsoft.IdentityModel.Tokens;
using project.BL.Facades;
using project.BL.Facades.Interfaces;
using project.BL.Models;
using project.BL.tests.ModelSeeds;
using project.DAL.Tests;
using project.BL.Enums;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xunit.Abstractions;

namespace project.BL.tests;

public class ActivityFacadeTests : FacadeTestsBase
{
    private readonly UserFacade _userFacade;
    private readonly ProjectFacade _projectFacade;
    private readonly ActivityFacade _activityFacade;

    public ActivityFacadeTests(ITestOutputHelper output) : base(output)
    {
        _userFacade = new UserFacade(UnitOfWorkFactory, UserModelMapper);
        _projectFacade = new ProjectFacade(UnitOfWorkFactory, ProjectModelMapper);
        _activityFacade = new ActivityFacade(UnitOfWorkFactory, ActivityModelMapper);
    }


    [Fact]
    public async Task CreateActivity_Success()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity = ActivitySeeds.ActivitySeed();

        // Act
        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity = await _activityFacade.SaveAsync(activity, returnedUser.Id, null);

        var DbActivity = await _activityFacade.GetAsync(returnedActivity.Id);

        // Assert
        Assert.NotNull(DbActivity);
        Assert.Null(DbActivity.Project);
        Assert.NotNull(DbActivity.Tags);
        Assert.True(DbActivity.Tags.IsNullOrEmpty());
        DeepAssert.Equal(returnedActivity, DbActivity);
    }

    
    [Fact]
    public async Task CreateActivityWithProject_Success()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity = ActivitySeeds.ActivitySeed();
        var project = ProjectSeeds.ProjectSeed();

        // Act
        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedProject = await _projectFacade.SaveAsync(project);
        var returnedActivity = await _activityFacade.SaveAsync(activity, returnedUser.Id, returnedProject.Id);

        var DbActivity = await _activityFacade.GetAsync(returnedActivity.Id);
        var DbProject = await _projectFacade.GetAsync(returnedProject.Id);
        Assert.NotNull(DbActivity);
        Assert.NotNull(DbActivity.Project);
        Assert.NotNull(DbProject);
        var projectListModel = new ProjectListModel()
        {
            Id = DbProject.Id,
            Name = DbProject.Name
        };

        // Assert
        Assert.NotNull(DbActivity.Project);
        Assert.Equal(DbProject.Id, DbActivity.Project.Id);
        DeepAssert.Equal(DbActivity.Project, projectListModel);
    }


    [Fact]
    public async Task RemoveProjectFromActivity_Success()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity = ActivitySeeds.ActivitySeed();
        var project = ProjectSeeds.ProjectSeed();

        // Act
        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedProject = await _projectFacade.SaveAsync(project);
        var returnedActivity = await _activityFacade.SaveAsync(activity, returnedUser.Id, returnedProject.Id);

        var DbActivity = await _activityFacade.GetAsync(returnedActivity.Id);
        var DbProject = await _projectFacade.GetAsync(returnedProject.Id);

        Assert.NotNull(DbActivity);
        Assert.NotNull(DbActivity.Project);
        Assert.NotNull(DbProject);

        // Assert Bonded
        Assert.Equal(DbProject.Id, DbActivity.Project.Id);

        // Remove bond
        await _activityFacade.SaveAsync(DbActivity, returnedUser.Id, null);

        // Update
        DbActivity = await _activityFacade.GetAsync(DbActivity.Id);
        DbProject = await _projectFacade.GetAsync(returnedProject.Id);

        // Assert bond removed, project still exists
        Assert.NotNull(DbActivity);
        Assert.Null(DbActivity.Project);
        Assert.NotNull(DbProject);
    }


    [Fact]
    public async Task DeleteProjectInActivity()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity = ActivitySeeds.ActivitySeed();
        var project = ProjectSeeds.ProjectSeed();

        // Act
        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedProject = await _projectFacade.SaveAsync(project);
        var returnedActivity = await _activityFacade.SaveAsync(activity, returnedUser.Id, returnedProject.Id);

        var DbActivity = await _activityFacade.GetAsync(returnedActivity.Id);
        var DbProject = await _projectFacade.GetAsync(returnedProject.Id);
        Assert.NotNull(DbProject);
        Assert.NotNull(DbActivity);
        Assert.NotNull(DbActivity.Project);
        Assert.Equal(DbProject.Id, DbActivity.Project.Id);

        await _projectFacade.DeleteAsync(DbProject.Id);

        DbActivity = await _activityFacade.GetAsync(DbActivity.Id);
        DbProject = await _projectFacade.GetAsync(DbProject.Id);
        Assert.Null(DbProject);
        Assert.NotNull(DbActivity);
        Assert.Null(DbActivity.Project);
    }


    [Fact]
    public async Task DeleteProjectInMoreActivities()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();
        var project = ProjectSeeds.ProjectSeed();

        activity1.DateTimeFrom = DateTime.Today.AddDays(-2);
        activity1.DateTimeTo = activity1.DateTimeFrom.AddHours(1);

        activity2.DateTimeFrom = DateTime.Today.AddDays(-4);
        activity2.DateTimeTo = activity2.DateTimeFrom.AddHours(1);

        // Act
        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedProject = await _projectFacade.SaveAsync(project);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser.Id, returnedProject.Id);
        var returnedActivity2 = await _activityFacade.SaveAsync(activity2, returnedUser.Id, returnedProject.Id);

        var DbActivity1 = await _activityFacade.GetAsync(returnedActivity1.Id);
        var DbActivity2 = await _activityFacade.GetAsync(returnedActivity2.Id);
        var DbProject = await _projectFacade.GetAsync(returnedProject.Id);
        Assert.NotNull(DbProject);
        Assert.NotNull(DbActivity1);
        Assert.NotNull(DbActivity2);
        Assert.NotNull(DbActivity1.Project);
        Assert.NotNull(DbActivity2.Project);
        Assert.Equal(DbProject.Id, DbActivity1.Project.Id);
        Assert.Equal(DbProject.Id, DbActivity2.Project.Id);

        await _projectFacade.DeleteAsync(DbProject.Id);

        DbActivity1 = await _activityFacade.GetAsync(DbActivity1.Id);
        DbActivity2 = await _activityFacade.GetAsync(DbActivity2.Id);
        DbProject = await _projectFacade.GetAsync(DbProject.Id);
        Assert.Null(DbProject);
        Assert.NotNull(DbActivity1);
        Assert.NotNull(DbActivity2);
        Assert.Null(DbActivity1.Project);
        Assert.Null(DbActivity2.Project);
    }


    [Fact]
    public async Task DeleteOneOfTwoProjects_MoreActivities()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();
        var project1 = ProjectSeeds.ProjectSeed();
        var project2 = ProjectSeeds.ProjectSeed();

        activity1.DateTimeFrom = DateTime.Today.AddDays(-2);
        activity1.DateTimeTo = activity1.DateTimeFrom.AddHours(1);

        activity2.DateTimeFrom = DateTime.Today.AddDays(-4);
        activity2.DateTimeTo = activity2.DateTimeFrom.AddHours(1);

        // Act
        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedProject1 = await _projectFacade.SaveAsync(project1);
        var returnedProject2 = await _projectFacade.SaveAsync(project2);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser.Id, returnedProject1.Id);
        var returnedActivity2 = await _activityFacade.SaveAsync(activity2, returnedUser.Id, returnedProject2.Id);

        var DbActivity1 = await _activityFacade.GetAsync(returnedActivity1.Id);
        var DbActivity2 = await _activityFacade.GetAsync(returnedActivity2.Id);
        var DbProject1 = await _projectFacade.GetAsync(returnedProject1.Id);
        var DbProject2 = await _projectFacade.GetAsync(returnedProject2.Id);
        Assert.NotNull(DbProject1);
        Assert.NotNull(DbProject2);
        Assert.NotNull(DbActivity1);
        Assert.NotNull(DbActivity2);
        Assert.NotNull(DbActivity1.Project);
        Assert.NotNull(DbActivity2.Project);
        Assert.Equal(DbProject1.Id, DbActivity1.Project.Id);
        Assert.Equal(DbProject2.Id, DbActivity2.Project.Id);

        await _projectFacade.DeleteAsync(DbProject1.Id);

        DbActivity1 = await _activityFacade.GetAsync(DbActivity1.Id);
        DbActivity2 = await _activityFacade.GetAsync(DbActivity2.Id);
        DbProject1 = await _projectFacade.GetAsync(DbProject1.Id);
        DbProject2 = await _projectFacade.GetAsync(DbProject2.Id);
        Assert.Null(DbProject1);
        Assert.NotNull(DbProject2);
        Assert.NotNull(DbActivity1);
        Assert.NotNull(DbActivity2);
        Assert.Null(DbActivity1.Project);
        Assert.NotNull(DbActivity2.Project);
        Assert.Equal(DbProject2.Id, DbActivity2.Project.Id);
    }


    [Fact]
    public async Task OneProject_MoreActivities()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();
        var project = ProjectSeeds.ProjectSeed();

        activity1.DateTimeFrom = new DateTime(2021, 05, 15, 18, 00, 00);
        activity1.DateTimeTo = new DateTime(2021, 05, 15, 20, 00, 00);

        activity2.DateTimeFrom = new DateTime(2021, 05, 16, 20, 30, 00);
        activity2.DateTimeTo = new DateTime(2021, 05, 16, 22, 00, 00);


        // Act
        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedProject = await _projectFacade.SaveAsync(project);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser.Id, returnedProject.Id);
        var returnedActivity2 = await _activityFacade.SaveAsync(activity2, returnedUser.Id, returnedProject.Id);

        var DbActivity1 = await _activityFacade.GetAsync(returnedActivity1.Id);
        var DbActivity2 = await _activityFacade.GetAsync(returnedActivity2.Id);
        var DbProject = await _projectFacade.GetAsync(returnedProject.Id);

        Assert.NotNull(DbProject);
        var projectListModel = new ProjectListModel()
        {
            Id = DbProject.Id,
            Name = DbProject.Name
        };

        // Assert
        Assert.NotNull(DbActivity1);
        Assert.NotNull(DbActivity1.Project);
        Assert.Equal(DbProject.Id, DbActivity1.Project.Id);
        DeepAssert.Equal(DbActivity1.Project, projectListModel);

        Assert.NotNull(DbActivity2);
        Assert.NotNull(DbActivity2.Project);
        Assert.Equal(DbProject.Id, DbActivity2.Project.Id);
        DeepAssert.Equal(DbActivity2.Project, projectListModel);


        // Remove project from one activity
        await _activityFacade.SaveAsync(DbActivity1, returnedUser.Id, null);

        // Update
        DbActivity1 = await _activityFacade.GetAsync(DbActivity1.Id);

        // Assert
        Assert.NotNull(DbActivity1);
        Assert.Null(DbActivity1.Project);

        Assert.NotNull(DbActivity2);
        Assert.NotNull(DbActivity2.Project);
        Assert.Equal(DbProject.Id, DbActivity2.Project.Id);
        DeepAssert.Equal(DbActivity2.Project, projectListModel);
    }


    [Fact]
    public async Task DeleteAcitivty()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity = ActivitySeeds.ActivitySeed();

        // Act
        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity = await _activityFacade.SaveAsync(activity, returnedUser.Id, null);

        var DbActivity = await _activityFacade.GetAsync(returnedActivity.Id);
        Assert.NotNull(DbActivity);

        await _activityFacade.DeleteAsync(returnedActivity.Id);

        // Assert
        DbActivity = await _activityFacade.GetAsync(returnedActivity.Id);
        Assert.Null(DbActivity);
    }


    [Fact]
    public async Task DeleteNonExistingAcitivty()
    {
        // Arrange
        var activity = ActivitySeeds.ActivitySeed();

        await Assert.ThrowsAsync<InvalidOperationException>(async () => await _activityFacade.DeleteAsync(activity.Id));
    }


    [Fact]
    public async Task DeleteAcitivty_WithProject()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity = ActivitySeeds.ActivitySeed();
        var project = ProjectSeeds.ProjectSeed();

        // Act
        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedProject = await _projectFacade.SaveAsync(project);
        var returnedActivity = await _activityFacade.SaveAsync(activity, returnedUser.Id, returnedProject.Id);

        var DbActivity = await _activityFacade.GetAsync(returnedActivity.Id);
        Assert.NotNull(DbActivity);
        Assert.NotNull(DbActivity.Project);

        await _activityFacade.DeleteAsync(returnedActivity.Id);

        var DbProject = await _projectFacade.GetAsync(returnedProject.Id);
        DbActivity = await _activityFacade.GetAsync(returnedActivity.Id);

        // Assert
        Assert.Null(DbActivity);
        Assert.NotNull(DbProject);
    }


    [Fact]
    public async Task DeleteUserWithAcitivty()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity = ActivitySeeds.ActivitySeed();

        // Act
        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity = await _activityFacade.SaveAsync(activity, returnedUser.Id, null);

        var DbActivity = await _activityFacade.GetAsync(returnedActivity.Id);
        Assert.NotNull(DbActivity);

        await _userFacade.DeleteAsync(returnedUser.Id);

        // Assert
        DbActivity = await _activityFacade.GetAsync(returnedActivity.Id);
        var DbUser = await _userFacade.GetAsync(returnedUser.Id);
        Assert.Null(DbUser);
        Assert.Null(DbActivity);
    }


    [Fact]
    public async Task DeleteUser_WithAcitivty_WithProject()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity = ActivitySeeds.ActivitySeed();
        var project = ProjectSeeds.ProjectSeed();

        // Act
        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedProject = await _projectFacade.SaveAsync(project);
        var returnedActivity = await _activityFacade.SaveAsync(activity, returnedUser.Id, returnedProject.Id);

        // Check
        var DbActivity = await _activityFacade.GetAsync(returnedActivity.Id);
        Assert.NotNull(DbActivity);
        Assert.NotNull(DbActivity.Project);

        // Delete
        await _userFacade.DeleteAsync(returnedUser.Id);

        // Update
        DbActivity = await _activityFacade.GetAsync(returnedActivity.Id);
        var DbUser = await _userFacade.GetAsync(returnedUser.Id);
        var DbProject = await _projectFacade.GetAsync(returnedProject.Id); 
        
        // Assert
        Assert.Null(DbUser);
        Assert.Null(DbActivity);
        Assert.NotNull(DbProject);
    }


    [Fact]
    public async Task GetList_OneActivity_DeepAssert()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity = ActivitySeeds.ActivitySeed();

        // Act
        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity = await _activityFacade.SaveAsync(activity, returnedUser.Id, null);

        // Check
        var DbActivity = await _activityFacade.GetAsync(returnedActivity.Id);
        Assert.NotNull(DbActivity);
        Assert.Null(DbActivity.Project);

        var _activityListModel = new ActivityListModel()
        {
            Id = DbActivity.Id,
            Name = DbActivity.Name,
            DateTimeFrom = DbActivity.DateTimeFrom,
            DateTimeTo = DbActivity.DateTimeTo,
            Color = DbActivity.Color,
            Tags = DbActivity.Tags,
            Project = DbActivity.Project
        };

        var activityList = await _activityFacade.GetAsyncUser(returnedUser.Id);
        Assert.True(activityList.Any());
        Assert.True(activityList.Count() == 1);
        DeepAssert.Equal(_activityListModel, activityList.First());
    }


    [Fact]
    public async Task GetList_Empty()
    {
        // Arrange
        var user = UserSeeds.UserSeed();

        // Act
        var returnedUser = await _userFacade.SaveAsync(user);

        // Assert
        var activityList = await _activityFacade.GetAsyncUser(returnedUser.Id);
        Assert.NotNull(activityList);
        Assert.True(activityList.IsNullOrEmpty());
    }


    [Fact]
    public async Task GetList_MoreActivites()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();
        var activity3 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = new DateTime(2021, 05, 15, 18, 00, 00);
        activity1.DateTimeTo = new DateTime(2021, 05, 15, 20, 00, 00);

        activity2.DateTimeFrom = new DateTime(2021, 05, 15, 20, 30, 00);
        activity2.DateTimeTo = new DateTime(2021, 05, 15, 22, 00, 00);

        activity3.DateTimeFrom = new DateTime(2021, 05, 16, 20, 30, 00);
        activity3.DateTimeTo = new DateTime(2021, 05, 16, 22, 00, 00);

        // Act
        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser.Id, null);
        var returnedActivity2 = await _activityFacade.SaveAsync(activity2, returnedUser.Id, null);
        var returnedActivity3 = await _activityFacade.SaveAsync(activity3, returnedUser.Id, null);

        var DbActivity1 = await _activityFacade.GetAsync(returnedActivity1.Id);
        var DbActivity2 = await _activityFacade.GetAsync(returnedActivity2.Id);
        var DbActivity3 = await _activityFacade.GetAsync(returnedActivity3.Id);


        var activityList = await _activityFacade.GetAsyncUser(returnedUser.Id);

        Assert.True(activityList.Any());

        List<Guid> Guids = new ();
        foreach (var activity in activityList)
        {
            Guids.Add(activity.Id);
        }

        Assert.NotNull(DbActivity1);
        Assert.NotNull(DbActivity2);
        Assert.NotNull(DbActivity3);

        Assert.Equal(3, activityList.Count());
        Assert.Contains(DbActivity1.Id, Guids);
        Assert.Contains(DbActivity2.Id, Guids);
        Assert.Contains(DbActivity3.Id, Guids);
    }

    
    [Fact]
    public async Task MoreUsers_MoreActivities()
    {
        // Arrange
        var user1 = UserSeeds.UserSeed();
        var user2 = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = new DateTime(2021, 05, 15, 18, 00, 00);
        activity1.DateTimeTo = new DateTime(2021, 05, 15, 20, 00, 00);

        activity2.DateTimeFrom = new DateTime(2021, 05, 16, 20, 30, 00);
        activity2.DateTimeTo = new DateTime(2021, 05, 16, 22, 00, 00);

        // Act
        var returnedUser1 = await _userFacade.SaveAsync(user1);
        var returnedUser2 = await _userFacade.SaveAsync(user2);
        var usr1Act1 = await _activityFacade.SaveAsync(activity1, returnedUser1.Id, null);
        var usr1Act2 = await _activityFacade.SaveAsync(activity2, returnedUser1.Id, null);
        var usr2Act1 = await _activityFacade.SaveAsync(activity1, returnedUser2.Id, null);
        var usr2Act2 = await _activityFacade.SaveAsync(activity2, returnedUser2.Id, null);

        var DbActivity1 = await _activityFacade.GetAsync(usr1Act1.Id);
        var DbActivity2 = await _activityFacade.GetAsync(usr1Act2.Id);
        var DbActivity3 = await _activityFacade.GetAsync(usr2Act1.Id);
        var DbActivity4 = await _activityFacade.GetAsync(usr2Act2.Id);


        var activityList1 = await _activityFacade.GetAsyncUser(returnedUser1.Id);
        var activityList2 = await _activityFacade.GetAsyncUser(returnedUser2.Id);

        Assert.True(activityList1.Any());
        Assert.True(activityList2.Any());

        List<Guid> Guids1 = new ();
        List<Guid> Guids2 = new ();
        foreach (var activity in activityList1)
        {
            Guids1.Add(activity.Id);
        }
        foreach (var activity in activityList2)
        {
            Guids2.Add(activity.Id);
        }

        Assert.NotNull(DbActivity1);
        Assert.NotNull(DbActivity2);
        Assert.NotNull(DbActivity3);
        Assert.NotNull(DbActivity4);

        Assert.Equal(2, activityList1.Count());
        Assert.Contains(DbActivity1.Id, Guids1);
        Assert.Contains(DbActivity2.Id, Guids1);

        Assert.Equal(2, activityList2.Count());
        Assert.Contains(DbActivity3.Id, Guids2);
        Assert.Contains(DbActivity4.Id, Guids2);
    }
    

    [Fact]
    public async Task Overlapping_Activites_Exception_1()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = new DateTime(2021, 05, 15, 18, 00, 00);
        activity1.DateTimeTo = new DateTime(2021, 05, 15, 20, 00, 00);

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser.Id, null);

        var DbActivity1 = await _activityFacade.GetAsync(returnedActivity1.Id);


        activity2.DateTimeFrom = new DateTime(2021, 05, 15, 18, 00, 00);
        activity2.DateTimeTo = new DateTime(2021, 05, 15, 20, 00, 00);


        await Assert.ThrowsAsync<OverlappingException>(async () => await _activityFacade.SaveAsync(activity2, returnedUser.Id, null));
    }


    [Fact]
    public async Task Overlapping_Activites_Exception_2()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = new DateTime(2021, 05, 15, 18, 00, 00);
        activity1.DateTimeTo = new DateTime(2021, 05, 15, 20, 00, 00);

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser.Id, null);

        var DbActivity1 = await _activityFacade.GetAsync(returnedActivity1.Id);

        activity2.DateTimeFrom = new DateTime(2021, 05, 15, 17, 00, 00);
        activity2.DateTimeTo = new DateTime(2021, 05, 15, 19, 00, 00);

        await Assert.ThrowsAsync<OverlappingException>(async () => await _activityFacade.SaveAsync(activity2, returnedUser.Id, null));
    }


    [Fact]
    public async Task Overlapping_Activites_Exception_3()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = new DateTime(2021, 05, 15, 18, 00, 00);
        activity1.DateTimeTo = new DateTime(2021, 05, 15, 20, 00, 00);

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser.Id, null);

        var DbActivity1 = await _activityFacade.GetAsync(returnedActivity1.Id);

        activity2.DateTimeFrom = new DateTime(2021, 05, 15, 19, 00, 00);
        activity2.DateTimeTo = new DateTime(2021, 05, 15, 21, 00, 00);

        await Assert.ThrowsAsync<OverlappingException>(async () => await _activityFacade.SaveAsync(activity2, returnedUser.Id, null));
    }


    [Fact]
    public async Task Overlapping_Activites_Exception_4()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = new DateTime(2021, 05, 15, 18, 00, 00);
        activity1.DateTimeTo = new DateTime(2021, 05, 15, 20, 00, 00);

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser.Id, null);

        var DbActivity1 = await _activityFacade.GetAsync(returnedActivity1.Id);


        activity2.DateTimeFrom = new DateTime(2021, 05, 15, 18, 30, 00);
        activity2.DateTimeTo = new DateTime(2021, 05, 15, 19, 30, 00);

        await Assert.ThrowsAsync<OverlappingException>(async () => await _activityFacade.SaveAsync(activity2, returnedUser.Id, null));
    }


    [Fact]
    public async Task Overlapping_Activites_Exception_5()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = new DateTime(2021, 05, 15, 18, 00, 00);
        activity1.DateTimeTo = new DateTime(2021, 05, 15, 20, 00, 00);

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser.Id, null);

        var DbActivity1 = await _activityFacade.GetAsync(returnedActivity1.Id);

        activity2.DateTimeFrom = new DateTime(2021, 05, 15, 17, 30, 00);
        activity2.DateTimeTo = new DateTime(2021, 05, 15, 20, 30, 00);

        await Assert.ThrowsAsync<OverlappingException>(async () => await _activityFacade.SaveAsync(activity2, returnedUser.Id, null));
    }


    [Fact]
    public async Task Overlapping_Activites_Update_NoException()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = new DateTime(2021, 05, 15, 18, 00, 00);
        activity1.DateTimeTo = new DateTime(2021, 05, 15, 20, 00, 00);

        activity2.DateTimeFrom = new DateTime(2021, 05, 15, 21, 00, 00);
        activity2.DateTimeTo = new DateTime(2021, 05, 15, 22, 00, 00);

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser.Id, null);
        var returnedActivity2 = await _activityFacade.SaveAsync(activity2, returnedUser.Id, null);

        var DbActivity1 = await _activityFacade.GetAsync(returnedActivity1.Id);

        Assert.NotNull(DbActivity1);
        DbActivity1.DateTimeFrom = new DateTime(2021, 05, 15, 18, 30, 00);
        DbActivity1.DateTimeTo = new DateTime(2021, 05, 15, 20, 30, 00);

        await _activityFacade.SaveAsync(DbActivity1, returnedUser.Id, null);
    }


    [Fact]
    public async Task Overlapping_Activites_Update_Exception()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = new DateTime(2021, 05, 15, 18, 00, 00);
        activity1.DateTimeTo = new DateTime(2021, 05, 15, 20, 00, 00);

        activity2.DateTimeFrom = new DateTime(2021, 05, 15, 20, 30, 00);
        activity2.DateTimeTo = new DateTime(2021, 05, 15, 22, 00, 00);

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser.Id, null);
        var returnedActivity2 = await _activityFacade.SaveAsync(activity2, returnedUser.Id, null);

        var DbActivity1 = await _activityFacade.GetAsync(returnedActivity1.Id);

        Assert.NotNull(DbActivity1);
        DbActivity1.DateTimeTo = new DateTime(2021, 05, 15, 21, 00 ,00);

        await Assert.ThrowsAsync<OverlappingException>(async () => await _activityFacade.SaveAsync(DbActivity1, returnedUser.Id, null));
    }


    [Fact]
    public async Task Overlapping_MoreUsers_Success()
    {
        // Arrange
        var user1 = UserSeeds.UserSeed();
        var user2 = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = new DateTime(2021, 05, 15, 18, 00, 00);
        activity1.DateTimeTo = new DateTime(2021, 05, 15, 20, 00, 00);

        activity2.DateTimeFrom = new DateTime(2021, 05, 15, 19, 00, 00);
        activity2.DateTimeTo = new DateTime(2021, 05, 15, 21, 00, 00);


        // Act
        var returnedUser1 = await _userFacade.SaveAsync(user1);
        var returnedUser2 = await _userFacade.SaveAsync(user2);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser1.Id, null);
        var returnedActivity2 = await _activityFacade.SaveAsync(activity2, returnedUser2.Id, null);
    }


    [Fact]
    public async Task DateFilter_NotSupported()
    {
        // Arrange
        var user = UserSeeds.UserSeed();

        var returnedUser = await _userFacade.SaveAsync(user);

        await Assert.ThrowsAsync<NotSupportedException>(async () => await _activityFacade.GetAsyncDateFilter(returnedUser.Id, null, null));
    }


    [Fact]
    public async Task DateFilter_from()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = new DateTime(2021, 05, 15, 18, 00, 00);
        activity1.DateTimeTo = new DateTime(2021, 05, 15, 20, 00, 00);

        activity2.DateTimeFrom = new DateTime(2021, 05, 16, 20, 30, 00);
        activity2.DateTimeTo = new DateTime(2021, 05, 16, 22, 00, 00);

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser.Id, null);
        var returnedActivity2 = await _activityFacade.SaveAsync(activity2, returnedUser.Id, null);


        // Asserts
        var from = new DateTime(2021, 05, 17, 00, 00, 00);
        var list = await _activityFacade.GetAsyncDateFilter(returnedUser.Id, from, null);
        Assert.False(list.Any());


        from = new DateTime(2021, 05, 16, 00, 00, 00);
        list = await _activityFacade.GetAsyncDateFilter(returnedUser.Id, from, null);
        Assert.True(list.Any());
        Assert.True(list.Count() == 1);
        Assert.Equal(returnedActivity2.Id, list.First().Id);


        from = new DateTime(2021, 05, 15, 00, 00, 00);
        list = await _activityFacade.GetAsyncDateFilter(returnedUser.Id, from, null);
        Assert.True(list.Any());
        Assert.True(list.Count() == 2);
    }


    [Fact]
    public async Task DateFilter_to()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = new DateTime(2021, 05, 15, 18, 00, 00);
        activity1.DateTimeTo = new DateTime(2021, 05, 15, 20, 00, 00);

        activity2.DateTimeFrom = new DateTime(2021, 05, 16, 20, 30, 00);
        activity2.DateTimeTo = new DateTime(2021, 05, 16, 22, 00, 00);

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser.Id, null);
        var returnedActivity2 = await _activityFacade.SaveAsync(activity2, returnedUser.Id, null);

        // Asserts
        var to = new DateTime(2021, 05, 15, 00, 00, 00);
        var list = await _activityFacade.GetAsyncDateFilter(returnedUser.Id, null, to);
        Assert.False(list.Any());


        to = new DateTime(2021, 05, 16, 00, 00, 00);
        list = await _activityFacade.GetAsyncDateFilter(returnedUser.Id, null, to);
        Assert.True(list.Any());
        Assert.True(list.Count() == 1);
        Assert.Equal(returnedActivity1.Id, list.First().Id);


        to = new DateTime(2021, 05, 17, 00, 00, 00);
        list = await _activityFacade.GetAsyncDateFilter(returnedUser.Id, null, to);
        Assert.True(list.Any());
        Assert.True(list.Count() == 2);
    }


    [Fact]
    public async Task DateFilter_from_to()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();
        var activity3 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = new DateTime(2021, 05, 15, 18, 00, 00);
        activity1.DateTimeTo = new DateTime(2021, 05, 15, 20, 00, 00);

        activity2.DateTimeFrom = new DateTime(2021, 05, 16, 20, 30, 00);
        activity2.DateTimeTo = new DateTime(2021, 05, 16, 22, 00, 00);

        activity3.DateTimeFrom = new DateTime(2021, 05, 17, 20, 30, 00);
        activity3.DateTimeTo = new DateTime(2021, 05, 17, 22, 00, 00);

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser.Id, null);
        var returnedActivity2 = await _activityFacade.SaveAsync(activity2, returnedUser.Id, null);
        var returnedActivity3 = await _activityFacade.SaveAsync(activity3, returnedUser.Id, null);

        // Asserts
        var from = new DateTime(2021, 05, 16, 00, 00, 00);
        var to = new DateTime(2021, 05, 17, 00, 00, 00);
        var list = await _activityFacade.GetAsyncDateFilter(returnedUser.Id, from, to);
        Assert.True(list.Any());
        Assert.True(list.Count() == 1);
        Assert.Equal(returnedActivity2.Id, list.First().Id);


        from = new DateTime(2021, 05, 15, 00, 00, 00);
        to = new DateTime(2021, 05, 18, 00, 00, 00);
        list = await _activityFacade.GetAsyncDateFilter(returnedUser.Id, from, to);
        Assert.True(list.Any());
        Assert.True(list.Count() == 3);
    }


    [Fact]
    public async Task DateFilter_NoActivities()
    {
        // Arrange
        var user = UserSeeds.UserSeed();

        var returnedUser = await _userFacade.SaveAsync(user);

        var from = new DateTime(2021, 05, 16, 00, 00, 00);
        var to = new DateTime(2021, 05, 17, 00, 00, 00);

        var list = await _activityFacade.GetAsyncDateFilter(returnedUser.Id, from, to);
        Assert.NotNull(list);
        Assert.True(list.IsNullOrEmpty());

        list = await _activityFacade.GetAsyncDateFilter(returnedUser.Id, from, null);
        Assert.NotNull(list);
        Assert.True(list.IsNullOrEmpty());

        list = await _activityFacade.GetAsyncDateFilter(returnedUser.Id, null, to);
        Assert.NotNull(list);
        Assert.True(list.IsNullOrEmpty());
    }


    [Fact]
    public async Task DateFilter_From_MoreUsers()
    {
        // Arrange
        var user1 = UserSeeds.UserSeed();
        var user2 = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = new DateTime(2021, 05, 15, 18, 00, 00);
        activity1.DateTimeTo = new DateTime(2021, 05, 15, 20, 00, 00);

        var returnedUser1 = await _userFacade.SaveAsync(user1);
        var returnedUser2 = await _userFacade.SaveAsync(user2);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser1.Id, null);
        var returnedActivity2 = await _activityFacade.SaveAsync(activity1, returnedUser2.Id, null);


        // Asserts
        var from = new DateTime(2021, 05, 15, 00, 00, 00);
        var list = await _activityFacade.GetAsyncDateFilter(returnedUser1.Id, from, null);
        Assert.True(list.Any());
        Assert.True(list.Count() == 1);

        list = await _activityFacade.GetAsyncDateFilter(returnedUser2.Id, from, null);
        Assert.True(list.Any());
        Assert.True(list.Count() == 1);
    }


    [Fact]
    public async Task IntervalFilter_Week_0()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = new DateTime(2023, 04, 15, 18, 00, 00);
        activity1.DateTimeTo = new DateTime(2023, 04, 15, 20, 00, 00);

        activity2.DateTimeFrom = new DateTime(2023, 04, 16, 20, 30, 00);
        activity2.DateTimeTo = new DateTime(2023, 04, 16, 22, 00, 00);

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser.Id, null);
        var returnedActivity2 = await _activityFacade.SaveAsync(activity2, returnedUser.Id, null);

        // Asserts
        var list = await _activityFacade.GetAsyncIntervalFilter(returnedUser.Id, FilterBy.Week);
        Assert.False(list.Any());
    }


    [Fact]
    public async Task IntervalFilter_Week_1()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = DateTime.Today.AddDays(-1);
        activity1.DateTimeTo = activity1.DateTimeFrom.AddHours(1);

        activity2.DateTimeFrom = new DateTime(2023, 04, 16, 20, 30, 00);
        activity2.DateTimeTo = new DateTime(2023, 04, 16, 22, 00, 00);

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser.Id, null);
        var returnedActivity2 = await _activityFacade.SaveAsync(activity2, returnedUser.Id, null);

        // Asserts
        var list = await _activityFacade.GetAsyncIntervalFilter(returnedUser.Id, FilterBy.Week);
        Assert.True(list.Any());
        Assert.True(list.Count() == 1);
        Assert.Equal(list.First().Id, returnedActivity1.Id);
    }


    [Fact]
    public async Task IntervalFilter_Week_2()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = DateTime.Today.AddDays(-1);
        activity1.DateTimeTo = activity1.DateTimeFrom.AddHours(1);

        activity2.DateTimeFrom = DateTime.Today.AddDays(-3);
        activity2.DateTimeTo = activity2.DateTimeFrom.AddHours(1);

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser.Id, null);
        var returnedActivity2 = await _activityFacade.SaveAsync(activity2, returnedUser.Id, null);

        // Asserts
        var list = await _activityFacade.GetAsyncIntervalFilter(returnedUser.Id, FilterBy.Week);
        Assert.True(list.Any());
        Assert.True(list.Count() == 2);
    }


    [Fact]
    public async Task IntervalFilter_Month_0()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = DateTime.Today.AddDays(-60);
        activity1.DateTimeTo = activity1.DateTimeFrom.AddHours(1);


        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser.Id, null);

        // Asserts
        var list = await _activityFacade.GetAsyncIntervalFilter(returnedUser.Id, FilterBy.Month);
        Assert.False(list.Any());
    }


    [Fact]
    public async Task IntervalFilter_Month_1()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = DateTime.Today.AddDays(-10);
        activity1.DateTimeTo = activity1.DateTimeFrom.AddHours(1);

        activity2.DateTimeFrom = DateTime.Today.AddMonths(-2);
        activity2.DateTimeTo = activity2.DateTimeFrom.AddHours(1);

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser.Id, null);
        var returnedActivity2 = await _activityFacade.SaveAsync(activity2, returnedUser.Id, null);

        // Asserts
        var list = await _activityFacade.GetAsyncIntervalFilter(returnedUser.Id, FilterBy.Month);
        Assert.True(list.Any());
        Assert.True(list.Count() == 1);
    }


    [Fact]
    public async Task IntervalFilter_Month_2()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = DateTime.Today.AddDays(-10);
        activity1.DateTimeTo = activity1.DateTimeFrom.AddHours(1);

        activity2.DateTimeFrom = DateTime.Today.AddDays(-11);
        activity2.DateTimeTo = activity2.DateTimeFrom.AddHours(1);

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser.Id, null);
        var returnedActivity2 = await _activityFacade.SaveAsync(activity2, returnedUser.Id, null);

        // Asserts
        var list = await _activityFacade.GetAsyncIntervalFilter(returnedUser.Id, FilterBy.Month);
        Assert.True(list.Any());
        Assert.True(list.Count() == 2);
    }


    [Fact]
    public async Task IntervalFilter_PreviousMonth_0()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = DateTime.Today.AddDays(-70);
        activity1.DateTimeTo = activity1.DateTimeFrom.AddHours(1);

        activity2.DateTimeFrom = DateTime.Today.AddDays(-10);
        activity2.DateTimeTo = activity2.DateTimeFrom.AddHours(1);

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser.Id, null);
        var returnedActivity2 = await _activityFacade.SaveAsync(activity2, returnedUser.Id, null);

        // Asserts
        var list = await _activityFacade.GetAsyncIntervalFilter(returnedUser.Id, FilterBy.PreviousMonth);
        Assert.False(list.Any());
    }


    [Fact]
    public async Task IntervalFilter_PreviousMonth_1()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();
        var activity3 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = DateTime.Today.AddDays(-10);
        activity1.DateTimeTo = activity1.DateTimeFrom.AddHours(1);

        activity2.DateTimeFrom = DateTime.Today.AddMonths(-2);
        activity2.DateTimeTo = activity2.DateTimeFrom.AddHours(1);

        activity3.DateTimeFrom = DateTime.Today.AddDays(-40);
        activity3.DateTimeTo = activity3.DateTimeFrom.AddHours(1);

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser.Id, null);
        var returnedActivity2 = await _activityFacade.SaveAsync(activity2, returnedUser.Id, null);
        var returnedActivity3 = await _activityFacade.SaveAsync(activity3, returnedUser.Id, null);

        // Asserts
        var list = await _activityFacade.GetAsyncIntervalFilter(returnedUser.Id, FilterBy.PreviousMonth);
        Assert.True(list.Any());
        Assert.True(list.Count() == 1);
    }


    [Fact]
    public async Task IntervalFilter_PreviousMonth_2()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = DateTime.Today.AddDays(-40);
        activity1.DateTimeTo = activity1.DateTimeFrom.AddHours(1);

        activity2.DateTimeFrom = DateTime.Today.AddDays(-50);
        activity2.DateTimeTo = activity2.DateTimeFrom.AddHours(1);

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser.Id, null);
        var returnedActivity2 = await _activityFacade.SaveAsync(activity2, returnedUser.Id, null);

        // Asserts
        var list = await _activityFacade.GetAsyncIntervalFilter(returnedUser.Id, FilterBy.PreviousMonth);
        Assert.True(list.Any());
        Assert.True(list.Count() == 2);
    }


    [Fact]
    public async Task IntervalFilter_Year_0()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();
        var activity3 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = DateTime.Today.AddMonths(-15);
        activity1.DateTimeTo = activity1.DateTimeFrom.AddHours(1);

        activity2.DateTimeFrom = DateTime.Today.AddDays(-400);
        activity2.DateTimeTo = activity2.DateTimeFrom.AddHours(1);

        activity3.DateTimeFrom = DateTime.Today.AddYears(-2);
        activity3.DateTimeTo = activity3.DateTimeFrom.AddHours(1);

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser.Id, null);
        var returnedActivity2 = await _activityFacade.SaveAsync(activity2, returnedUser.Id, null);
        var returnedActivity3 = await _activityFacade.SaveAsync(activity3, returnedUser.Id, null);

        // Asserts
        var list = await _activityFacade.GetAsyncIntervalFilter(returnedUser.Id, FilterBy.Year);
        Assert.False(list.Any());
    }


    [Fact]
    public async Task IntervalFilter_Year_1()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = DateTime.Today.AddDays(-10);
        activity1.DateTimeTo = activity1.DateTimeFrom.AddHours(1);

        activity2.DateTimeFrom = DateTime.Today.AddMonths(-16);
        activity2.DateTimeTo = activity2.DateTimeFrom.AddHours(1);

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser.Id, null);
        var returnedActivity2 = await _activityFacade.SaveAsync(activity2, returnedUser.Id, null);

        // Asserts
        var list = await _activityFacade.GetAsyncIntervalFilter(returnedUser.Id, FilterBy.Year);
        Assert.True(list.Any());
        Assert.True(list.Count() == 1);
    }


    [Fact]
    public async Task IntervalFilter_Year_2()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = DateTime.Today.AddDays(-40);
        activity1.DateTimeTo = activity1.DateTimeFrom.AddHours(1);

        activity2.DateTimeFrom = DateTime.Today.AddMonths(-3);
        activity2.DateTimeTo = activity2.DateTimeFrom.AddHours(1);

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity1, returnedUser.Id, null);
        var returnedActivity2 = await _activityFacade.SaveAsync(activity2, returnedUser.Id, null);

        // Asserts
        var list = await _activityFacade.GetAsyncIntervalFilter(returnedUser.Id, FilterBy.Year);
        Assert.True(list.Any());
        Assert.True(list.Count() == 2);
    }


    [Fact]
    public async Task IntervalFilter_NoActivities()
    {
        // Arrange
        var user = UserSeeds.UserSeed();


        var returnedUser = await _userFacade.SaveAsync(user);

        // Asserts
        var list = await _activityFacade.GetAsyncIntervalFilter(returnedUser.Id, FilterBy.Week);
        Assert.False(list.Any());

        list = await _activityFacade.GetAsyncIntervalFilter(returnedUser.Id, FilterBy.Month);
        Assert.False(list.Any());

        list = await _activityFacade.GetAsyncIntervalFilter(returnedUser.Id, FilterBy.PreviousMonth);
        Assert.False(list.Any());

        list = await _activityFacade.GetAsyncIntervalFilter(returnedUser.Id, FilterBy.Year);
        Assert.False(list.Any());
    }


    [Fact]
    public async Task IntervalFilter_Future()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity = ActivitySeeds.ActivitySeed();

        activity.DateTimeFrom = DateTime.Today.AddDays(3);
        activity.DateTimeTo = activity.DateTimeFrom.AddHours(1);

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity1 = await _activityFacade.SaveAsync(activity, returnedUser.Id, null);

        // Asserts
        var list = await _activityFacade.GetAsyncIntervalFilter(returnedUser.Id, FilterBy.Week);
        Assert.False(list.Any());

        list = await _activityFacade.GetAsyncIntervalFilter(returnedUser.Id, FilterBy.Month);
        Assert.False(list.Any());

        list = await _activityFacade.GetAsyncIntervalFilter(returnedUser.Id, FilterBy.PreviousMonth);
        Assert.False(list.Any());

        list = await _activityFacade.GetAsyncIntervalFilter(returnedUser.Id, FilterBy.Year);
        Assert.False(list.Any());
    }


    [Fact]
    public async Task IntervalFilter_From_MoreUsers()
    {
        // Arrange
        var user1 = UserSeeds.UserSeed();
        var user2 = UserSeeds.UserSeed();
        var activity1 = ActivitySeeds.ActivitySeed();
        var activity2 = ActivitySeeds.ActivitySeed();
        var activity3 = ActivitySeeds.ActivitySeed();
        var activity4 = ActivitySeeds.ActivitySeed();
        var activity5 = ActivitySeeds.ActivitySeed();
        var activity6 = ActivitySeeds.ActivitySeed();

        activity1.DateTimeFrom = DateTime.Today.AddDays(-1);
        activity1.DateTimeTo = activity1.DateTimeFrom.AddHours(1);

        activity2.DateTimeFrom = DateTime.Today.AddDays(-14);
        activity2.DateTimeTo = activity2.DateTimeFrom.AddHours(1);

        activity3.DateTimeFrom = DateTime.Today.AddDays(-40);
        activity3.DateTimeTo = activity3.DateTimeFrom.AddHours(1);

        activity4.DateTimeFrom = DateTime.Today.AddMonths(-3);
        activity4.DateTimeTo = activity4.DateTimeFrom.AddHours(1);

        activity5.DateTimeFrom = DateTime.Today.AddMonths(-20);
        activity5.DateTimeTo = activity5.DateTimeFrom.AddHours(1);

        activity6.DateTimeFrom = DateTime.Today.AddDays(2);
        activity6.DateTimeTo = activity6.DateTimeFrom.AddHours(1);

        var returnedUser1 = await _userFacade.SaveAsync(user1);
        var returnedUser2 = await _userFacade.SaveAsync(user2);

        var User1Act1 = await _activityFacade.SaveAsync(activity1, returnedUser1.Id, null);
        var User1Act2 = await _activityFacade.SaveAsync(activity3, returnedUser1.Id, null);
        var User1Act3 = await _activityFacade.SaveAsync(activity5, returnedUser1.Id, null);
        var User1Act4 = await _activityFacade.SaveAsync(activity6, returnedUser1.Id, null);

        var User2Act1 = await _activityFacade.SaveAsync(activity2, returnedUser2.Id, null);
        var User2Act2 = await _activityFacade.SaveAsync(activity4, returnedUser2.Id, null);
        var User2Act3 = await _activityFacade.SaveAsync(activity5, returnedUser2.Id, null);
        var User2Act4 = await _activityFacade.SaveAsync(activity6, returnedUser2.Id, null);



        // Asserts
        var U1list = await _activityFacade.GetAsyncIntervalFilter(returnedUser1.Id, FilterBy.Week);
        var U2list = await _activityFacade.GetAsyncIntervalFilter(returnedUser2.Id, FilterBy.Week);
        Assert.True(U1list.Any());
        Assert.True(U1list.Count() == 1);
        Assert.False(U2list.Any());

        U1list = await _activityFacade.GetAsyncIntervalFilter(returnedUser1.Id, FilterBy.Month);
        U2list = await _activityFacade.GetAsyncIntervalFilter(returnedUser2.Id, FilterBy.Month);
        Assert.True(U1list.Any());
        Assert.True(U1list.Count() == 1);
        Assert.True(U2list.Any());
        Assert.True(U2list.Count() == 1);

        U1list = await _activityFacade.GetAsyncIntervalFilter(returnedUser1.Id, FilterBy.PreviousMonth);
        U2list = await _activityFacade.GetAsyncIntervalFilter(returnedUser2.Id, FilterBy.PreviousMonth);
        Assert.True(U1list.Any());
        Assert.True(U1list.Count() == 1);
        Assert.False(U2list.Any());

        U1list = await _activityFacade.GetAsyncIntervalFilter(returnedUser1.Id, FilterBy.Year);
        U2list = await _activityFacade.GetAsyncIntervalFilter(returnedUser2.Id, FilterBy.Year);
        Assert.True(U1list.Any());
        Assert.True(U1list.Count() == 2);
        Assert.True(U2list.Any());
        Assert.True(U2list.Count() == 2);
    }
}

