using Microsoft.IdentityModel.Tokens;
using project.BL.Facades;
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

public class UserProjectFacadeTests : FacadeTestsBase
{
    private readonly UserFacade _userFacade;
    private readonly ProjectFacade _projectFacade;
    private readonly UserProjectFacade _userProjectFacade;
    public UserProjectFacadeTests(ITestOutputHelper output) : base(output)
    {
        _userFacade = new UserFacade(UnitOfWorkFactory, UserModelMapper);
        _projectFacade = new ProjectFacade(UnitOfWorkFactory, ProjectModelMapper);
        _userProjectFacade = new UserProjectFacade(UnitOfWorkFactory);
    }


    [Fact]
    public async Task AddUserToProject_Succeeds()
    {
        // Arrange
        var userModel = UserSeeds.UserSeed();
        var projectModel = ProjectSeeds.ProjectSeed();

        // Act
        var returnedUser = await _userFacade.SaveAsync(userModel);
        var returnedProject = await _projectFacade.SaveAsync(projectModel);

        var DbUser = await _userFacade.GetAsync(returnedUser.Id);
        var DbProject = await _projectFacade.GetAsync(returnedProject.Id);

        Assert.NotNull(DbUser);
        Assert.NotNull(DbProject);

        var _userListModel = new UserListModel()
        {
            Id = DbUser.Id,
            UserName = DbUser.UserName
        };

        // Assert not bound
        Assert.True(DbProject.Users.IsNullOrEmpty());

        // Bind
        await _userProjectFacade.SaveAsync(DbUser.Id, DbProject.Id);

        // Update
        DbProject = await _projectFacade.GetAsync(DbProject.Id);
        Assert.NotNull(DbProject);

        // Assert bound
        Assert.True(DbProject.Users.Any());
        DeepAssert.Equal(DbProject.Users.First(), _userListModel);
    }


    [Fact]
    public async Task AddMoreUsersToProject_Success()
    {
        // Arrange
        var userModel1 = UserSeeds.UserSeed();
        var userModel2 = UserSeeds.UserSeed();
        var userModel3 = UserSeeds.UserSeed();
        var projectModel = ProjectSeeds.ProjectSeed();

        // Act
        var returnedUser1 = await _userFacade.SaveAsync(userModel1);
        var returnedUser2 = await _userFacade.SaveAsync(userModel2);
        var returnedUser3 = await _userFacade.SaveAsync(userModel3);
        var returnedProject = await _projectFacade.SaveAsync(projectModel);

        var DbUser1 = await _userFacade.GetAsync(returnedUser1.Id);
        var DbUser2 = await _userFacade.GetAsync(returnedUser2.Id);
        var DbUser3 = await _userFacade.GetAsync(returnedUser3.Id);
        var DbProject = await _projectFacade.GetAsync(returnedProject.Id);

        Assert.NotNull(DbUser1);
        Assert.NotNull(DbUser2);
        Assert.NotNull(DbUser3);
        Assert.NotNull(DbProject);

        var _userListModel1 = new UserListModel()
        {
            Id = DbUser1.Id,
            UserName = DbUser1.UserName
        };

        var _userListModel2 = new UserListModel()
        {
            Id = DbUser2.Id,
            UserName = DbUser2.UserName
        };

        var _userListModel3 = new UserListModel()
        {
            Id = DbUser3.Id,
            UserName = DbUser3.UserName
        };

        var _userListModelNotBonded = new UserListModel()
        {
            Id = Guid.NewGuid(),
            UserName = "Ales Bejr"
        };

        // Assert not bound
        Assert.True(DbProject.Users.IsNullOrEmpty());

        // Bind
        await _userProjectFacade.SaveAsync(DbUser1.Id, DbProject.Id);
        await _userProjectFacade.SaveAsync(DbUser2.Id, DbProject.Id);
        await _userProjectFacade.SaveAsync(DbUser3.Id, DbProject.Id);

        // Update
        DbProject = await _projectFacade.GetAsync(DbProject.Id);
        Assert.NotNull(DbProject);


        // Assert bound
        Assert.True(DbProject.Users.Any());
        Assert.Equal(3, DbProject.Users.Count);

        Assert.Contains(_userListModel1, DbProject.Users);
        Assert.Contains(_userListModel2, DbProject.Users);
        Assert.Contains(_userListModel3, DbProject.Users);
        Assert.DoesNotContain(_userListModelNotBonded, DbProject.Users);
    }


    [Fact]
    public async Task RemoveUserFromProject_Success()
    {
        // Arrange
        var userModel = UserSeeds.UserSeed();
        var projectModel = ProjectSeeds.ProjectSeed();

        // Act
        var returnedUser = await _userFacade.SaveAsync(userModel);
        var returnedProject = await _projectFacade.SaveAsync(projectModel);

        var DbUser = await _userFacade.GetAsync(returnedUser.Id);
        var DbProject = await _projectFacade.GetAsync(returnedProject.Id);

        Assert.NotNull(DbUser);
        Assert.NotNull(DbProject);

        // Bind
        await _userProjectFacade.SaveAsync(DbUser.Id, DbProject.Id);

        DbProject = await _projectFacade.GetAsync(DbProject.Id);
        Assert.NotNull(DbProject);

        // Assert bound
        Assert.True(DbProject.Users.Any());

        // Unbind
        await _userProjectFacade.DeleteAsync(DbUser.Id, DbProject.Id);

        DbProject = await _projectFacade.GetAsync(DbProject.Id);
        Assert.NotNull(DbProject);

        // Assert unbound
        Assert.True(DbProject.Users.IsNullOrEmpty());
    }


    [Fact]
    public async Task DeleteNonExistingBond_fail()
    {
        // Arrange
        var userModel = UserSeeds.UserSeed();
        var projectModel = ProjectSeeds.ProjectSeed();

        // Act & Assert
        await Assert.ThrowsAsync<InvalidOperationException>(async () => await _userProjectFacade.DeleteAsync(userModel.Id, projectModel.Id));
    }


    [Fact]
    public async Task MoreUsersMoreProjects()
    {
        // Arrange
        var userModel1 = UserSeeds.UserSeed();
        var userModel2 = UserSeeds.UserSeed();
        var userModel3 = UserSeeds.UserSeed();
        var userModel4 = UserSeeds.UserSeed();
        var userModel5 = UserSeeds.UserSeed();
        var projectModel1 = ProjectSeeds.ProjectSeed();
        var projectModel2 = ProjectSeeds.ProjectSeed();
        var projectModel3 = ProjectSeeds.ProjectSeed();

        // Act
        var returnedUser1 = await _userFacade.SaveAsync(userModel1);
        var returnedUser2 = await _userFacade.SaveAsync(userModel2);
        var returnedUser3 = await _userFacade.SaveAsync(userModel3);
        var returnedUser4 = await _userFacade.SaveAsync(userModel4);
        var returnedUser5 = await _userFacade.SaveAsync(userModel5);
        var returnedProject1 = await _projectFacade.SaveAsync(projectModel1);
        var returnedProject2 = await _projectFacade.SaveAsync(projectModel2);
        var returnedProject3 = await _projectFacade.SaveAsync(projectModel3);

        var DbUser1 = await _userFacade.GetAsync(returnedUser1.Id);
        var DbUser2 = await _userFacade.GetAsync(returnedUser2.Id);
        var DbUser3 = await _userFacade.GetAsync(returnedUser3.Id);
        var DbUser4 = await _userFacade.GetAsync(returnedUser4.Id);
        var DbUser5 = await _userFacade.GetAsync(returnedUser5.Id);
        var DbProject1 = await _projectFacade.GetAsync(returnedProject1.Id);
        var DbProject2 = await _projectFacade.GetAsync(returnedProject2.Id);
        var DbProject3 = await _projectFacade.GetAsync(returnedProject3.Id);

        Assert.NotNull(DbUser1);
        Assert.NotNull(DbUser2);
        Assert.NotNull(DbUser3);
        Assert.NotNull(DbUser4);
        Assert.NotNull(DbUser5);
        Assert.NotNull(DbProject1);
        Assert.NotNull(DbProject2);
        Assert.NotNull(DbProject3);

        var _userListModel1 = new UserListModel()
        {
            Id = DbUser1.Id,
            UserName = DbUser1.UserName
        };

        var _userListModel2 = new UserListModel()
        {
            Id = DbUser2.Id,
            UserName = DbUser2.UserName
        };

        var _userListModel3 = new UserListModel()
        {
            Id = DbUser3.Id,
            UserName = DbUser3.UserName
        };

        var _userListModel4 = new UserListModel()
        {
            Id = DbUser4.Id,
            UserName = DbUser4.UserName
        };

        var _userListModel5 = new UserListModel()
        {
            Id = DbUser5.Id,
            UserName = DbUser5.UserName
        };

        var _userListModelNotBonded = new UserListModel()
        {
            Id = Guid.NewGuid(),
            UserName = "Ales Bejr"
        };

        // Bind
        await _userProjectFacade.SaveAsync(DbUser1.Id, DbProject1.Id);
        await _userProjectFacade.SaveAsync(DbUser2.Id, DbProject2.Id);
        await _userProjectFacade.SaveAsync(DbUser3.Id, DbProject3.Id);
        await _userProjectFacade.SaveAsync(DbUser4.Id, DbProject1.Id);
        await _userProjectFacade.SaveAsync(DbUser5.Id, DbProject1.Id);

        // Update
        DbProject1 = await _projectFacade.GetAsync(DbProject1.Id);
        DbProject2 = await _projectFacade.GetAsync(DbProject2.Id);
        DbProject3 = await _projectFacade.GetAsync(DbProject3.Id);
        Assert.NotNull(DbProject1);
        Assert.NotNull(DbProject2);
        Assert.NotNull(DbProject3);


        // Assert bound
        Assert.True(DbProject1.Users.Any());
        Assert.True(DbProject2.Users.Any());
        Assert.True(DbProject3.Users.Any());

        Assert.Equal(3, DbProject1.Users.Count);
        Assert.Single(DbProject2.Users);
        Assert.Single(DbProject3.Users);

        Assert.Contains(_userListModel1, DbProject1.Users);
        Assert.Contains(_userListModel4, DbProject1.Users);
        Assert.Contains(_userListModel5, DbProject1.Users);
        Assert.DoesNotContain(_userListModel2, DbProject1.Users);
        Assert.DoesNotContain(_userListModel3, DbProject1.Users);

        Assert.Contains(_userListModel2, DbProject2.Users);
        Assert.DoesNotContain(_userListModel1, DbProject2.Users);
        Assert.DoesNotContain(_userListModel3, DbProject2.Users);
        Assert.DoesNotContain(_userListModel4, DbProject2.Users);
        Assert.DoesNotContain(_userListModel5, DbProject2.Users);

        Assert.Contains(_userListModel3, DbProject3.Users);
        Assert.DoesNotContain(_userListModel1, DbProject3.Users);
        Assert.DoesNotContain(_userListModel2, DbProject3.Users);
        Assert.DoesNotContain(_userListModel4, DbProject3.Users);
        Assert.DoesNotContain(_userListModel5, DbProject3.Users);

        Assert.DoesNotContain(_userListModelNotBonded, DbProject1.Users);
        Assert.DoesNotContain(_userListModelNotBonded, DbProject2.Users);
        Assert.DoesNotContain(_userListModelNotBonded, DbProject3.Users);
    }


    [Fact]
    public async Task DeleteProject()
    {
        // Arrange
        var userModel = UserSeeds.UserSeed();
        var projectModel = ProjectSeeds.ProjectSeed();

        // Act
        var returnedUser = await _userFacade.SaveAsync(userModel);
        var returnedProject = await _projectFacade.SaveAsync(projectModel);

        var DbUser = await _userFacade.GetAsync(returnedUser.Id);
        var DbProject = await _projectFacade.GetAsync(returnedProject.Id);

        Assert.NotNull(DbUser);
        Assert.NotNull(DbProject);

        var _userListModel = new UserListModel()
        {
            Id = DbUser.Id,
            UserName = DbUser.UserName
        };

        // Assert not bound
        Assert.True(DbProject.Users.IsNullOrEmpty());

        // Bind
        await _userProjectFacade.SaveAsync(DbUser.Id, DbProject.Id);

        // Update
        DbProject = await _projectFacade.GetAsync(DbProject.Id);
        Assert.NotNull(DbProject);

        // Assert bound
        Assert.True(DbProject.Users.Any());
        DeepAssert.Equal(DbProject.Users.First(), _userListModel);

        // Delete project
        await _projectFacade.DeleteAsync(DbProject.Id);

        //Update
        DbUser = await _userFacade.GetAsync(DbUser.Id);
        DbProject = await _projectFacade.GetAsync(DbProject.Id);

        // Assert
        Assert.Null(DbProject);
        Assert.NotNull(DbUser);
    }


    [Fact]
    public async Task DeleteUser()
    {
        // Arrange
        var userModel = UserSeeds.UserSeed();
        var projectModel = ProjectSeeds.ProjectSeed();

        // Act
        var returnedUser = await _userFacade.SaveAsync(userModel);
        var returnedProject = await _projectFacade.SaveAsync(projectModel);

        var DbUser = await _userFacade.GetAsync(returnedUser.Id);
        var DbProject = await _projectFacade.GetAsync(returnedProject.Id);

        Assert.NotNull(DbUser);
        Assert.NotNull(DbProject);

        var _userListModel = new UserListModel()
        {
            Id = DbUser.Id,
            UserName = DbUser.UserName
        };

        // Assert not bound
        Assert.True(DbProject.Users.IsNullOrEmpty());

        // Bind
        await _userProjectFacade.SaveAsync(DbUser.Id, DbProject.Id);

        // Update
        DbProject = await _projectFacade.GetAsync(DbProject.Id);
        Assert.NotNull(DbProject);

        // Assert bound
        Assert.True(DbProject.Users.Any());
        DeepAssert.Equal(DbProject.Users.First(), _userListModel);

        // Delete user
        await _userFacade.DeleteAsync(DbUser.Id);

        //Update
        DbUser = await _userFacade.GetAsync(DbUser.Id);
        DbProject = await _projectFacade.GetAsync(DbProject.Id);

        // Assert
        Assert.NotNull(DbProject);
        Assert.Null(DbUser);

        Assert.NotNull(DbProject.Users);
        Assert.False(DbProject.Users.Any());
    }
}
