using Microsoft.EntityFrameworkCore;
using Microsoft.IdentityModel.Tokens;
using project.DAL.Entities;
using project.DAL.Mappers;
using project.DAL.Repositories;
using project.DAL.Tests.Seeds;

namespace project.DAL.Tests;

public class RepositoryTests : DbContextTestsBase
{
    [Fact]
    public async Task RepositoryGetTest()
    {
        var user = UserSeeds.UserSeed();
        ProjectDbContextSUT.Users.Add(user);
        await ProjectDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        var Mapper = new UserEntityMapper();
        var Repo = new Repository<UserEntity>(dbx, Mapper);

        var dbFromRepo = Repo.Get();

        var dbFromRepoList = dbFromRepo.ToList();
        Assert.Equal(dbFromRepoList.Count, dbx.Users.Count());
        Assert.Equal(dbFromRepoList.First().FullName, dbx.Users.First().FullName);
        Assert.Equal(dbFromRepoList.First().UserName, dbx.Users.First().UserName);
    }
    [Fact]
    public async Task RepositoryExistsTest()
    {
        var userOutOfDbx = UserSeeds.UserSeed();
        var userInDbx = UserSeeds.UserSeed();
        ProjectDbContextSUT.Users.Add(userInDbx);
        await ProjectDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        var Mapper = new UserEntityMapper();
        var Repo = new Repository<UserEntity>(dbx, Mapper);

        var trueCase = await Repo.ExistsAsync(userInDbx);
        var falseCase = await Repo.ExistsAsync(userOutOfDbx);

        Assert.True(trueCase);
        Assert.False(falseCase);
    }
    [Fact]
    public async Task RepositoryInsertTest()
    {
        var user = UserSeeds.UserSeed();

        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        var Mapper = new UserEntityMapper();
        var Repo = new Repository<UserEntity>(dbx, Mapper);

        await Repo.InsertAsync(user);
        await dbx.SaveChangesAsync();

        var dbxUserList = dbx.Users.ToList();
        Assert.Equal(user.FullName, dbxUserList[0].FullName);
        Assert.Equal(user.UserName, dbxUserList[0].UserName);
    }
    [Fact]
    public async Task RepositoryUpdateTest()
    {
        var user = UserSeeds.UserSeed();
        ProjectDbContextSUT.Users.Add(user);
        await ProjectDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        var Mapper = new UserEntityMapper();
        var Repo = new Repository<UserEntity>(dbx, Mapper);

        user.UserName = "Test";
        await Repo.UpdateAsync(user);

        var dbUser = await dbx.Users.SingleAsync(i => i.Id == user.Id);

        Assert.Equal(user.UserName, dbUser.UserName);
    }
    [Fact]
    public async Task RepositoryDeleteTest()
    {
        var user = UserSeeds.UserSeed();
        ProjectDbContextSUT.Users.Add(user);
        await ProjectDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        var Mapper = new UserEntityMapper();
        var Repo = new Repository<UserEntity>(dbx, Mapper);

        Repo.Delete(user.Id);
        await dbx.SaveChangesAsync();

        var userList = dbx.Users.ToList();
        Assert.True(userList.IsNullOrEmpty());
    }
}