using project.BL.Facades;
using project.BL.tests.ModelSeeds;
using project.BL.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xunit.Abstractions;
using project.DAL.Tests;
using project.BL.Facades.Interfaces;
using System.Drawing;
using Microsoft.EntityFrameworkCore.Query.Internal;

namespace project.BL.tests;
public class TagFacadeTests : FacadeTestsBase
{
    private readonly TagFacade _tagFacade;
    private readonly UserFacade _userFacade;
    public TagFacadeTests(ITestOutputHelper output) : base(output)
    {
        _userFacade = new UserFacade(UnitOfWorkFactory, UserModelMapper);
        _tagFacade = new TagFacade(UnitOfWorkFactory, TagModelMapper);
    }


    [Fact]
    public async Task CreateTag_Success()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var tag = TagSeeds.TagSeed();

        // Act
        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedTag = await _tagFacade.SaveAsync(tag, returnedUser.Id);

        var DbTag = await _tagFacade.GetAsync(returnedTag.Id);

        // Assert
        DeepAssert.Equal(returnedTag, DbTag);
    }


    [Fact]
    public async Task DeleteTag_GetById_IsNull()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var tag = TagSeeds.TagSeed();

        // Act
        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedTag = await _tagFacade.SaveAsync(tag, returnedUser.Id);

        await _tagFacade.DeleteAsync(returnedTag.Id);

        var DbTag = await _tagFacade.GetAsync(returnedTag.Id);

        // Assert
        Assert.Null(DbTag);
    }


    [Fact]
    public async Task DeleteTag_ByDeletingUser_GetById_IsNull()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var tag = TagSeeds.TagSeed();

        // Act
        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedTag = await _tagFacade.SaveAsync(tag, returnedUser.Id);

        await _userFacade.DeleteAsync(returnedUser.Id);

        var DbTag = await _tagFacade.GetAsync(returnedTag.Id);

        // Assert
        Assert.Null(DbTag);
    }


    [Fact]
    public async Task DeleteNonExistingTag_Exception()
    {
        // Arrange
        var tag = TagSeeds.TagSeed();

        // Act & Assert
        await Assert.ThrowsAsync<InvalidOperationException>(async () => await _tagFacade.DeleteAsync(tag.Id));
    }


    [Fact]
    public async Task OneUser_MoreTags_GetAll()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var tag1 = TagSeeds.TagSeed();
        var tag2 = TagSeeds.TagSeed();
        var tag3 = TagSeeds.TagSeed();

        // Act
        var returnedUser = await _userFacade.SaveAsync(user);
        var retTag1 = await _tagFacade.SaveAsync(tag1, returnedUser.Id);
        var retTag2 = await _tagFacade.SaveAsync(tag2, returnedUser.Id);
        var retTag3 = await _tagFacade.SaveAsync(tag3, returnedUser.Id);

        var DbTagList = await _tagFacade.GetAsyncUser(returnedUser.Id);

        // Assert
        Assert.Contains(retTag1, DbTagList);
        Assert.Contains(retTag2, DbTagList);
        Assert.Contains(retTag3, DbTagList);

        Assert.DoesNotContain(tag1, DbTagList);
    }


    [Fact]
    public async Task MoreUsers_MoreTags()
    {
        // Arrange
        var user1 = UserSeeds.UserSeed();
        var user2 = UserSeeds.UserSeed();
        var user3 = UserSeeds.UserSeed();
        var tag1 = TagSeeds.TagSeed();
        var tag2 = TagSeeds.TagSeed();
        var tag3 = TagSeeds.TagSeed();
        var tag4 = TagSeeds.TagSeed();
        var tag5 = TagSeeds.TagSeed();

        // Act
        var returnedUser1 = await _userFacade.SaveAsync(user1);
        var returnedUser2 = await _userFacade.SaveAsync(user2);
        var returnedUser3 = await _userFacade.SaveAsync(user3);

        var retTag1 = await _tagFacade.SaveAsync(tag1, returnedUser1.Id);
        var retTag2 = await _tagFacade.SaveAsync(tag2, returnedUser2.Id);
        var retTag3 = await _tagFacade.SaveAsync(tag3, returnedUser3.Id);
        var retTag4 = await _tagFacade.SaveAsync(tag4, returnedUser1.Id);
        var retTag5 = await _tagFacade.SaveAsync(tag5, returnedUser1.Id);

        var TagListUser1 = await _tagFacade.GetAsyncUser(returnedUser1.Id);
        var TagListUser2 = await _tagFacade.GetAsyncUser(returnedUser2.Id);
        var TagListUser3 = await _tagFacade.GetAsyncUser(returnedUser3.Id);

        // Assert
        Assert.Contains(retTag1, TagListUser1);
        Assert.Contains(retTag4, TagListUser1);
        Assert.Contains(retTag5, TagListUser1);
        Assert.DoesNotContain(retTag2, TagListUser1);
        Assert.DoesNotContain(retTag3, TagListUser1);

        Assert.Contains(retTag2, TagListUser2);
        Assert.DoesNotContain(retTag1, TagListUser2);
        Assert.DoesNotContain(retTag3, TagListUser2);
        Assert.DoesNotContain(retTag4, TagListUser2);
        Assert.DoesNotContain(retTag5, TagListUser2);

        Assert.Contains(retTag3, TagListUser3);
        Assert.DoesNotContain(retTag1, TagListUser3);
        Assert.DoesNotContain(retTag2, TagListUser3);
        Assert.DoesNotContain(retTag4, TagListUser3);
        Assert.DoesNotContain(retTag5, TagListUser3);
    }


    [Fact]
    public async Task EmptyTagList()
    {
        // Arrange
        var user = UserSeeds.UserSeed();

        // Act
        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedTagListUser = await _tagFacade.GetAsyncUser(returnedUser.Id);

        //Assert
        Assert.Empty(returnedTagListUser);
    }


    [Fact]
    public async Task UpdateTag_Correct()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var tag = TagSeeds.TagSeed();

        // Act
        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedTag = await _tagFacade.SaveAsync(tag, returnedUser.Id);

        var DbTag = await _tagFacade.GetAsync(returnedTag.Id);

        Assert.NotNull(DbTag);

        DbTag.Name = "Divocina";

        // Update
        await _tagFacade.SaveAsync(DbTag, returnedUser.Id);

        var UpdatedDbTag = await _tagFacade.GetAsync(DbTag.Id);

        // Assert
        DeepAssert.Equal(DbTag, UpdatedDbTag);
    }


    [Fact]
    public async Task TagColor_Works()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var tag = TagSeeds.TagSeed();

        tag.Color = Color.FromArgb(Color.Fuchsia.ToArgb());//deez colors are shaiet

        // Act
        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedTag = await _tagFacade.SaveAsync(tag, returnedUser.Id);

        var DbTag = await _tagFacade.GetAsync(returnedTag.Id);
        Assert.NotNull(DbTag);

        // Assert
        Assert.Equal(DbTag.Color, tag.Color);
    }

}

