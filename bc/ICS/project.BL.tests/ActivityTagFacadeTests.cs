using Azure;
using project.BL.Facades;
using project.BL.Facades.Interfaces;
using project.BL.tests.ModelSeeds;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xunit.Abstractions;

namespace project.BL.tests;

public class ActivityTagFacadeTests : FacadeTestsBase
{
    private readonly UserFacade _userFacade;
    private readonly ActivityFacade _activityFacade;
    private readonly TagFacade _tagFacade;
    private readonly ActivityTagFacade _activityTagFacade;

    public ActivityTagFacadeTests(ITestOutputHelper output) : base(output)
    {
        _userFacade = new UserFacade(UnitOfWorkFactory, UserModelMapper);
        _activityFacade = new ActivityFacade(UnitOfWorkFactory, ActivityModelMapper);
        _tagFacade = new TagFacade(UnitOfWorkFactory, TagModelMapper);
        _activityTagFacade = new ActivityTagFacade(UnitOfWorkFactory);
    }

    [Fact]
    public async Task AddTagToActivity_Success()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity = ActivitySeeds.ActivitySeed();
        var tag = TagSeeds.TagSeed();

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity = await _activityFacade.SaveAsync(activity, returnedUser.Id, null);
        var returnedTag = await _tagFacade.SaveAsync(tag, returnedUser.Id);

        // Act
        var DbActivity = await _activityFacade.GetAsync(returnedActivity.Id);
        var DbTag = await _tagFacade.GetAsync(returnedTag.Id);
        Assert.NotNull(DbActivity);
        Assert.NotNull(DbTag);
        Assert.NotNull(DbActivity.Tags);

        // Assert not bound
        Assert.Empty(DbActivity.Tags);

        // Bind
        await _activityTagFacade.SaveAsync(DbActivity.Id, DbTag.Id);

        // Update
        DbActivity = await _activityFacade.GetAsync(DbActivity.Id);
        DbTag = await _tagFacade.GetAsync(DbTag.Id);
        Assert.NotNull(DbActivity);
        Assert.NotNull(DbTag);
        Assert.NotNull(DbActivity.Tags);

        // Assert bound
        Assert.True(DbActivity.Tags.Any());
        Assert.Equal(DbActivity.Tags.First().Id, DbTag.Id);
    }


    [Fact]
    public async Task AddMoreTagsToActivity_Success()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity = ActivitySeeds.ActivitySeed();
        var tag1 = TagSeeds.TagSeed();
        var tag2 = TagSeeds.TagSeed();
        var tag3 = TagSeeds.TagSeed();

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity = await _activityFacade.SaveAsync(activity, returnedUser.Id, null);
        var returnedTag1 = await _tagFacade.SaveAsync(tag1, returnedUser.Id);
        var returnedTag2 = await _tagFacade.SaveAsync(tag2, returnedUser.Id);
        var returnedTag3 = await _tagFacade.SaveAsync(tag3, returnedUser.Id);

        // Act
        var DbActivity = await _activityFacade.GetAsync(returnedActivity.Id);
        var DbTag1 = await _tagFacade.GetAsync(returnedTag1.Id);
        var DbTag2 = await _tagFacade.GetAsync(returnedTag2.Id);
        var DbTag3 = await _tagFacade.GetAsync(returnedTag3.Id);
        Assert.NotNull(DbActivity);
        Assert.NotNull(DbTag1);
        Assert.NotNull(DbTag2);
        Assert.NotNull(DbTag3);
        Assert.NotNull(DbActivity.Tags);

        // Assert not bound
        Assert.Empty(DbActivity.Tags);

        // Bind
        await _activityTagFacade.SaveAsync(DbActivity.Id, DbTag1.Id);
        await _activityTagFacade.SaveAsync(DbActivity.Id, DbTag2.Id);
        await _activityTagFacade.SaveAsync(DbActivity.Id, DbTag3.Id);

        // Update
        DbActivity = await _activityFacade.GetAsync(DbActivity.Id);
        DbTag1 = await _tagFacade.GetAsync(DbTag1.Id);
        DbTag2 = await _tagFacade.GetAsync(DbTag2.Id);
        DbTag3 = await _tagFacade.GetAsync(DbTag3.Id);
        Assert.NotNull(DbActivity);
        Assert.NotNull(DbTag1);
        Assert.NotNull(DbTag2);
        Assert.NotNull(DbTag3);
        Assert.NotNull(DbActivity.Tags);

        // Assert bound
        Assert.True(DbActivity.Tags.Any());

        List<Guid> Guids = new();
        foreach (var tag in DbActivity.Tags)
        {
            Guids.Add(tag.Id);
        }

        Assert.True(Guids.Count == 3);

        Assert.Contains(DbTag1.Id, Guids);
        Assert.Contains(DbTag2.Id, Guids);
        Assert.Contains(DbTag3.Id, Guids);
    }


    [Fact]
    public async Task RemoveTagFromActivity_Success()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity = ActivitySeeds.ActivitySeed();
        var tag1 = TagSeeds.TagSeed();
        var tag2 = TagSeeds.TagSeed();

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity = await _activityFacade.SaveAsync(activity, returnedUser.Id, null);
        var returnedTag1 = await _tagFacade.SaveAsync(tag1, returnedUser.Id);
        var returnedTag2 = await _tagFacade.SaveAsync(tag2, returnedUser.Id);

        // Act
        var DbActivity = await _activityFacade.GetAsync(returnedActivity.Id);
        var DbTag1 = await _tagFacade.GetAsync(returnedTag1.Id);
        var DbTag2 = await _tagFacade.GetAsync(returnedTag2.Id);
        Assert.NotNull(DbActivity);
        Assert.NotNull(DbTag1);
        Assert.NotNull(DbTag2);
        Assert.NotNull(DbActivity.Tags);

        // Bind
        await _activityTagFacade.SaveAsync(DbActivity.Id, DbTag1.Id);
        await _activityTagFacade.SaveAsync(DbActivity.Id, DbTag2.Id);

        // Update
        DbActivity = await _activityFacade.GetAsync(DbActivity.Id);
        DbTag1 = await _tagFacade.GetAsync(DbTag1.Id);
        DbTag2 = await _tagFacade.GetAsync(DbTag2.Id);
        Assert.NotNull(DbActivity);
        Assert.NotNull(DbTag1);
        Assert.NotNull(DbTag2);
        Assert.NotNull(DbActivity.Tags);

        // Assert bound
        Assert.True(DbActivity.Tags.Any());

        List<Guid> Guids = new();
        foreach (var tag in DbActivity.Tags)
        {
            Guids.Add(tag.Id);
        }
        Assert.True(Guids.Count == 2);
        Assert.Contains(DbTag1.Id, Guids);
        Assert.Contains(DbTag2.Id, Guids);

        // Unbind
        await _activityTagFacade.DeleteAsync(DbActivity.Id, DbTag1.Id);

        // Update
        DbActivity = await _activityFacade.GetAsync(DbActivity.Id);
        DbTag1 = await _tagFacade.GetAsync(DbTag1.Id);
        DbTag2 = await _tagFacade.GetAsync(DbTag2.Id);
        Assert.NotNull(DbActivity);
        Assert.NotNull(DbTag1);
        Assert.NotNull(DbTag2);
        Assert.NotNull(DbActivity.Tags);

        // Assert unbound
        Assert.True(DbActivity.Tags.Any());
        Assert.True(DbActivity.Tags.Count == 1);
        Assert.Equal(DbActivity.Tags.First().Id, DbTag2.Id);
    }

    [Fact]
    public async Task DeleteNonExistingBond_fail()
    {
        // Arrange
        var activity = ActivitySeeds.ActivitySeed();
        var tag = TagSeeds.TagSeed();

        // Act & Assert
        await Assert.ThrowsAsync<InvalidOperationException>(async () => await _activityTagFacade.DeleteAsync(activity.Id, tag.Id));
    }

    [Fact]
    public async Task DeleteTag()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity = ActivitySeeds.ActivitySeed();
        var tag = TagSeeds.TagSeed();

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity = await _activityFacade.SaveAsync(activity, returnedUser.Id, null);
        var returnedTag = await _tagFacade.SaveAsync(tag, returnedUser.Id);

        // Act
        var DbActivity = await _activityFacade.GetAsync(returnedActivity.Id);
        var DbTag = await _tagFacade.GetAsync(returnedTag.Id);
        Assert.NotNull(DbActivity);
        Assert.NotNull(DbTag);
        Assert.NotNull(DbActivity.Tags);

        // Bind
        await _activityTagFacade.SaveAsync(DbActivity.Id, DbTag.Id);

        // Update
        DbActivity = await _activityFacade.GetAsync(DbActivity.Id);
        DbTag = await _tagFacade.GetAsync(DbTag.Id);
        Assert.NotNull(DbActivity);
        Assert.NotNull(DbTag);
        Assert.NotNull(DbActivity.Tags);

        // Assert bound
        Assert.True(DbActivity.Tags.Any());
        Assert.Equal(DbActivity.Tags.First().Id, DbTag.Id);

        // Delete
        await _tagFacade.DeleteAsync(DbTag.Id);

        // Update
        DbActivity = await _activityFacade.GetAsync(DbActivity.Id);
        DbTag = await _tagFacade.GetAsync(DbTag.Id);
        Assert.NotNull(DbActivity);
        Assert.NotNull(DbActivity.Tags);

        // Assert
        Assert.Null(DbTag);
        Assert.False(DbActivity.Tags.Any());
    }


    [Fact]
    public async Task DeleteActivity()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity = ActivitySeeds.ActivitySeed();
        var tag = TagSeeds.TagSeed();

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity = await _activityFacade.SaveAsync(activity, returnedUser.Id, null);
        var returnedTag = await _tagFacade.SaveAsync(tag, returnedUser.Id);

        // Act
        var DbActivity = await _activityFacade.GetAsync(returnedActivity.Id);
        var DbTag = await _tagFacade.GetAsync(returnedTag.Id);
        Assert.NotNull(DbActivity);
        Assert.NotNull(DbTag);
        Assert.NotNull(DbActivity.Tags);

        // Bind
        await _activityTagFacade.SaveAsync(DbActivity.Id, DbTag.Id);

        // Update
        DbActivity = await _activityFacade.GetAsync(DbActivity.Id);
        DbTag = await _tagFacade.GetAsync(DbTag.Id);
        Assert.NotNull(DbActivity);
        Assert.NotNull(DbTag);
        Assert.NotNull(DbActivity.Tags);

        // Assert bound
        Assert.True(DbActivity.Tags.Any());
        Assert.Equal(DbActivity.Tags.First().Id, DbTag.Id);

        // Delete
        await _activityFacade.DeleteAsync(DbActivity.Id);

        // Update
        DbActivity = await _activityFacade.GetAsync(DbActivity.Id);
        DbTag = await _tagFacade.GetAsync(DbTag.Id);

        // Assert
        Assert.NotNull(DbTag);
        Assert.Null(DbActivity);
    }


    [Fact]
    public async Task DeleteUser()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity = ActivitySeeds.ActivitySeed();
        var tag = TagSeeds.TagSeed();

        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedActivity = await _activityFacade.SaveAsync(activity, returnedUser.Id, null);
        var returnedTag = await _tagFacade.SaveAsync(tag, returnedUser.Id);

        // Act
        var DbActivity = await _activityFacade.GetAsync(returnedActivity.Id);
        var DbTag = await _tagFacade.GetAsync(returnedTag.Id);
        var DbUser = await _userFacade.GetAsync(returnedUser.Id);
        Assert.NotNull(DbActivity);
        Assert.NotNull(DbTag);
        Assert.NotNull(DbActivity.Tags);
        Assert.NotNull(DbUser);

        // Bind
        await _activityTagFacade.SaveAsync(DbActivity.Id, DbTag.Id);

        // Update
        DbActivity = await _activityFacade.GetAsync(DbActivity.Id);
        DbTag = await _tagFacade.GetAsync(DbTag.Id);
        Assert.NotNull(DbActivity);
        Assert.NotNull(DbTag);
        Assert.NotNull(DbActivity.Tags);

        // Assert bound
        Assert.True(DbActivity.Tags.Any());
        Assert.Equal(DbActivity.Tags.First().Id, DbTag.Id);

        // Delete
        await _userFacade.DeleteAsync(DbUser.Id);

        // Update
        DbActivity = await _activityFacade.GetAsync(DbActivity.Id);
        DbTag = await _tagFacade.GetAsync(DbTag.Id);
        DbUser = await _userFacade.GetAsync(DbUser.Id);

        // Assert
        Assert.Null(DbUser);
        Assert.Null(DbTag);
        Assert.Null(DbActivity);
    }

    //public async Task MoreActivites_MoreTags()
    //public async Task MoreUsers_MoreActivities_MoreTags()
}
