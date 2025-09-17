using project.BL.Facades;
using project.BL.Models;
using project.BL.tests.ModelSeeds;
using Xunit.Abstractions;
using project.DAL.Tests;

namespace project.BL.tests;

public class UserFacadeTests : FacadeTestsBase
{
    private readonly UserFacade _userFacade;
    public UserFacadeTests(ITestOutputHelper output) : base(output)
    {
        _userFacade = new UserFacade(UnitOfWorkFactory, UserModelMapper);
    }

    [Fact]
    public async Task CreateUser_Succeeds()
    {
        // Arrange
        var userModel = UserSeeds.UserSeed();
        
        // Act
        var idk = await _userFacade.SaveAsync(userModel);
        
        // Assert
        FixIds(userModel, idk);
        DeepAssert.Equal(userModel, idk);
    }

    [Fact]
    public async Task CreateUserModel_GetEntity_Compare_Success()
    {
        // Arrange
        var userModel = UserSeeds.UserSeed();
        
        // Act
        var userModelUpdated = await _userFacade.SaveAsync(userModel);
        var userFromDb = await _userFacade.GetAsync(userModelUpdated.Id);
        
        // Assert
        Assert.NotNull(userFromDb);
        FixIds(userModel, userModelUpdated);
        FixIds(userModel,userFromDb);
        DeepAssert.Equal(userModel, userModelUpdated);
        DeepAssert.Equal(userModel, userFromDb);
        DeepAssert.Equal(userModelUpdated, userFromDb);
    }

    [Fact]
    public async Task TwoModels_InsertFirst_SearchSecond_IsNull()
    {
        // Arrange
        var user1 = UserSeeds.UserSeed();
        var user2 = UserSeeds.UserSeed();
        
        // Act
        var user1Updated = await _userFacade.SaveAsync(user1);
        var userFromDb = await _userFacade.GetAsync(user2.Id);
        
        // Assert
        Assert.Null(userFromDb);
    }

    [Fact]
    public async Task InsertFive_FindsCorrect()
    {
        // Arrange
        var user1 = UserSeeds.UserSeed();
        var user2 = UserSeeds.UserSeed();
        var user3 = UserSeeds.UserSeed();
        var user4 = UserSeeds.UserSeed();
        var user5 = UserSeeds.UserSeed();
        
        // Act
        var user1Updated = await _userFacade.SaveAsync(user1);
        var user2Updated = await _userFacade.SaveAsync(user2);
        var user3Updated = await _userFacade.SaveAsync(user3);
        var user4Updated = await _userFacade.SaveAsync(user4);
        var user5Updated = await _userFacade.SaveAsync(user5);

        var userFromDb = await _userFacade.GetAsync(user4Updated.Id);
        
        // Assert
        Assert.NotNull(userFromDb);
        FixIds(user4, userFromDb);
        DeepAssert.Equal(user4, userFromDb);
    }

    [Fact]
    public async Task DeleteUser_Success()
    {
        // Arrange
        var userModel = UserSeeds.UserSeed();
        
        // Act
        var returnedModel = await _userFacade.SaveAsync(userModel);
        await _userFacade.DeleteAsync(returnedModel.Id);
        
        // Assert
        var shouldBeNull = await _userFacade.GetAsync(returnedModel.Id);
        Assert.Null(shouldBeNull);
    }

    [Fact]
    public async Task DeleteNonExistingUser_Exception()
    {
        // Arrange
        var userModel = UserSeeds.UserSeed();

        await Assert.ThrowsAsync<InvalidOperationException>(async () => await _userFacade.DeleteAsync(userModel.Id));
    }


    [Fact]
    public async Task InsertMoreUsers_DeleteCorrect()
    {
        // Arrange
        var user1 = UserSeeds.UserSeed();
        var user2 = UserSeeds.UserSeed();
        var user3 = UserSeeds.UserSeed();
        var user4 = UserSeeds.UserSeed();
        var user5 = UserSeeds.UserSeed();

        // Act
        var user1Updated = await _userFacade.SaveAsync(user1);
        var user2Updated = await _userFacade.SaveAsync(user2);
        var user3Updated = await _userFacade.SaveAsync(user3);
        var user4Updated = await _userFacade.SaveAsync(user4);
        var user5Updated = await _userFacade.SaveAsync(user5);

        await _userFacade.DeleteAsync(user2Updated.Id);
        var shouldBeNull = await _userFacade.GetAsync(user2Updated.Id);
        
        // Assert
        Assert.Null(shouldBeNull);
    }


    [Fact]
    public async Task UpdateUser_Correct()
    {
        // Arrange
        var userModel = UserSeeds.UserSeed();

        // Act
        var insertedModel = await _userFacade.SaveAsync(userModel);
        var dbModel = await _userFacade.GetAsync(insertedModel.Id);

        Assert.NotNull(dbModel);
        
        dbModel.FullName = "Ales Bejr";
        await _userFacade.SaveAsync(dbModel);

        var dbModelUpdated = await _userFacade.GetAsync(insertedModel.Id);

        DeepAssert.Equal(dbModel, dbModelUpdated);
    }


    [Fact]
    public async Task GetListModels_Correct()
    {
        // Arrange
        var user1 = UserSeeds.UserSeed();
        var user2 = UserSeeds.UserSeed();
        var user3 = UserSeeds.UserSeed();

        // Act
        var user1Updated = await _userFacade.SaveAsync(user1);
        var user2Updated = await _userFacade.SaveAsync(user2);
        var user3Updated = await _userFacade.SaveAsync(user3);

        var UserList = await _userFacade.GetAsync();

        var user1Listmodel = new UserListModel()
        {
            Id = user1Updated.Id,
            UserName = user1Updated.UserName
        };

        var user2Listmodel = new UserListModel()
        {
            Id = user2Updated.Id,
            UserName = user2Updated.UserName
        };

        var user3Listmodel = new UserListModel()
        {
            Id = user3Updated.Id,
            UserName = user3Updated.UserName
        };

        var userListNotInDb = new UserListModel()
        {
            Id = Guid.NewGuid(),
            UserName = "Ales Bejr"
        };

        Assert.Contains(user1Listmodel, UserList);
        Assert.Contains(user2Listmodel, UserList);
        Assert.Contains(user3Listmodel, UserList);

        Assert.DoesNotContain(userListNotInDb, UserList);
    }


    [Fact]
    public async Task GetListModels_Empty()
    {
        var UserList = await _userFacade.GetAsync();

        Assert.Empty(UserList);
    }


    private static void FixIds(UserDetailModel expectedModel, UserDetailModel returnedModel)
    {
        returnedModel.Id = expectedModel.Id;

        foreach (var activityListModel in returnedModel.Activities)
        {
            var activity = expectedModel.Activities.FirstOrDefault(i =>
                i.Name == activityListModel.Name
                && i.Color == activityListModel.Color
                && i.DateTimeFrom == activityListModel.DateTimeFrom
                && i.DateTimeTo == activityListModel.DateTimeTo
                && i.Project == activityListModel.Project);
            
            if (activity != null)
            {
                activityListModel.Id = activity.Id;
            }
        }
    }
}