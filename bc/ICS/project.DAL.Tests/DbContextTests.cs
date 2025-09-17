using System.Diagnostics;
using Microsoft.EntityFrameworkCore;
using project.DAL.Entities;
using project.DAL.Tests.Seeds;

namespace project.DAL.Tests;

public class DbContextTests : DbContextTestsBase
{
    [Fact]
    public async Task AddNewUser()
    {
        var user = UserSeeds.UserSeed();
            
        ProjectDbContextSUT.Users.Add(user);
        await ProjectDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        var dbEntity = await dbx.Users.SingleAsync(i => i.Id == user.Id);

        Assert.Equal(dbEntity.FullName, user.FullName);
        Assert.Equal(dbEntity.UserName, user.UserName);
    }
        
    [Fact]
    public async Task AddNewTag()
    {
        var user = UserSeeds.UserSeed();
        var tag = TagSeeds.TagSeed() with { UserId = user.Id };

        ProjectDbContextSUT.Tags.Add(tag);
        ProjectDbContextSUT.Users.Add(user);
        await ProjectDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        var dbEntity = await dbx.Tags.SingleAsync(i => i.Id == tag.Id);

        Assert.Equal(dbEntity.Name, tag.Name);
        Assert.Equal(dbEntity.Color, tag.Color);
    }
        
    [Fact]
    public async Task AddNewProject()
    {
        var project = ProjectSeeds.ProjectSeed();
            
        ProjectDbContextSUT.Projects.Add(project);
        await ProjectDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        var dbEntity = await dbx.Projects.SingleAsync(i => i.Id == project.Id);

        Assert.Equal(dbEntity.Name, project.Name);
        Assert.Equal(dbEntity.Description, project.Description);
    }

    [Fact]
    public async Task AddNewUserWithTodo()
    {
        var user = UserSeeds.UserSeed();

        var todo = TodoSeeds.TodoSeed() with
        {
            UserId = user.Id
        };

        ProjectDbContextSUT.Todos.Add(todo);
        ProjectDbContextSUT.Users.Add(user);
        await ProjectDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        var dbTodo = await dbx.Todos.SingleAsync(i => i.Id == todo.Id);

        Assert.Equal(dbTodo.Name, todo.Name);
        Assert.Equal(dbTodo.Date, todo.Date);
        Assert.Equal(dbTodo.Finished, todo.Finished);
    }

    [Fact]
    public async Task AddNewProjectWithActivity()
    {
        var user = UserSeeds.UserSeed();
            
        var project = ProjectSeeds.ProjectSeed();

        var activity = ActivitySeeds.ActivitySeed() with
        {
            User = user,
            UserId = user.Id,
            Project = project,
            ProjectId = project.Id
        };

        ProjectDbContextSUT.Users.Add(user);
        ProjectDbContextSUT.Projects.Add(project);
        ProjectDbContextSUT.Activities.Add(activity);
        await ProjectDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        var dbActivity = await dbx.Activities.SingleAsync(i => i.Id == activity.Id);
        var dbProject = await dbx.Projects.SingleAsync(i => i.Id == project.Id);
        var dbUser = await dbx.Users.SingleAsync(i => i.Id == user.Id);

        Assert.Equal(user.Id, dbActivity.UserId);
        Debug.Assert(dbActivity.Project != null, "dbActivity.Project != null");
        Assert.Equal(project.Id, dbActivity.Project.Id);
        Assert.Equal(user.Id, dbActivity.UserId);
        Assert.Equal(project.Id, dbActivity.ProjectId);
        Assert.Equal(dbProject.Id, dbActivity.ProjectId);
        Assert.Equal(dbUser.Id, user.Id);
    }

    [Fact]
    public async Task AddTwoUsersDifferentProject()
    {
        var user1 = UserSeeds.UserSeed();
        var user2 = UserSeeds.UserSeed();
        
        var project1 = ProjectSeeds.ProjectSeed();
        var project2 = ProjectSeeds.ProjectSeed();

        var activity1 = ActivitySeeds.ActivitySeed() with
        {
            User = null,
            UserId = user1.Id,
            Project = null,
            ProjectId = project1.Id
        };
        var activity2 = ActivitySeeds.ActivitySeed() with
        {
            User = null,
            UserId = user2.Id,
            Project = null,
            ProjectId = project2.Id
        };
            
        ProjectDbContextSUT.Users.Add(user1);
        ProjectDbContextSUT.Users.Add(user2);
        ProjectDbContextSUT.Projects.Add(project1);
        ProjectDbContextSUT.Projects.Add(project2);
        ProjectDbContextSUT.Activities.Add(activity1);
        ProjectDbContextSUT.Activities.Add(activity2);
        await ProjectDbContextSUT.SaveChangesAsync();
            
        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        var dbActivity1 = await dbx.Activities.SingleAsync(i => i.Id == activity1.Id);
        var dbActivity2 = await dbx.Activities.SingleAsync(i => i.Id == activity2.Id);
        var dbProject1 = await dbx.Projects.SingleAsync(i => i.Id == project1.Id);
        var dbProject2 = await dbx.Projects.SingleAsync(i => i.Id == project2.Id);
        var dbUser1 = await dbx.Users.SingleAsync(i => i.Id == user1.Id);
        var dbUser2 = await dbx.Users.SingleAsync(i => i.Id == user2.Id);
            
        Assert.Equal(dbActivity1.UserId, user1.Id);
        Assert.Equal(dbActivity2.UserId, user2.Id);
        Assert.Equal(dbUser1.Id, user1.Id);
        Assert.Equal(dbUser2.Id, user2.Id);

        Assert.NotEqual(dbProject1.Id, dbProject2.Id);
        Assert.NotEqual(dbUser1.Id, dbUser2.Id);
        Assert.NotEqual(dbUser1.Id, dbActivity2.Id);
        Assert.NotEqual(dbUser2.Id, dbActivity1.Id);
    }
        
    [Fact]
    public async Task AddTwoActivitiesToProject()
    {
        var user = UserSeeds.UserSeed();
            
        var project = ProjectSeeds.ProjectSeed();

        var activity1 = ActivitySeeds.ActivitySeed() with
        {
            User = user,
            UserId = user.Id,
            Project = project,
            ProjectId = project.Id
        };
        var activity2 = ActivitySeeds.ActivitySeed() with
        {
            User = user,
            UserId = user.Id,
            Project = project,
            ProjectId = project.Id
        };

        ProjectDbContextSUT.Users.Add(user);
        ProjectDbContextSUT.Projects.Add(project);
        ProjectDbContextSUT.Activities.Add(activity1);
        ProjectDbContextSUT.Activities.Add(activity2);
        await ProjectDbContextSUT.SaveChangesAsync();
            
        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        var dbActivity1 = await dbx.Activities.SingleAsync(i => i.Id == activity1.Id);
        var dbActivity2 = await dbx.Activities.SingleAsync(i => i.Id == activity2.Id);
        var dbProject = await dbx.Projects.SingleAsync(i => i.Id == project.Id);
        var dbUser = await dbx.Users.SingleAsync(i => i.Id == user.Id);
            
        Assert.Equal(dbActivity1.UserId, dbActivity2.UserId);
        Assert.Equal(dbProject.Id, activity1.ProjectId);
        Assert.Equal(dbProject.Id, activity2.ProjectId);
        Assert.Equal(dbUser.Id, user.Id);
            
        Assert.NotEqual(dbActivity1.Id, activity2.Id);
    }

    [Fact]
    public async Task AddTwoUsersToProject()
    {
        var user1 = UserSeeds.UserSeed();
        var user2 = UserSeeds.UserSeed();

        var project = ProjectSeeds.ProjectSeed() with
        {
            Users = new List<UserProjectListEntity>()
        };

        var user1InProject = new UserProjectListEntity
        {
            Id = Guid.NewGuid(),
            ProjectId = project.Id,
            Project = project,
            UserId = user1.Id,
            User = user1
        };
        var user2InProject = new UserProjectListEntity
        {
            Id = Guid.NewGuid(),
            ProjectId = project.Id,
            Project = project,
            UserId = user2.Id,
            User = user2
        };
            
        project.Users.Add(user1InProject);
        project.Users.Add(user2InProject);
        user1.Projects.Add(user1InProject);
        user2.Projects.Add(user2InProject);

        ProjectDbContextSUT.Users.Add(user1);
        ProjectDbContextSUT.Users.Add(user2);
        ProjectDbContextSUT.Projects.Add(project);
        await ProjectDbContextSUT.SaveChangesAsync();
            
        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        var dbProject = await dbx.Projects.Include(i => i.Users).SingleAsync(i => i.Id == project.Id);
        var dbUser1 = await dbx.Users.Include(i => i.Projects).SingleAsync(i => i.Id == user1.Id);
        var dbUser2 = await dbx.Users.Include(i => i.Projects).SingleAsync(i => i.Id == user2.Id);

        Assert.NotEqual(dbUser1.Id, dbUser2.Id);
        DeepAssert.Equal(project.Users, dbProject.Users);
        DeepAssert.Equal(project, dbProject);
    }

    [Fact]
    public async Task AddTwoActivitiesToProjectCollection()
    {
        var user = UserSeeds.UserSeed();
            
        var project = ProjectSeeds.ProjectSeed() with
        {
            Activities = new List<ActivityEntity>()
        };

        var activity1InProject = new ActivityEntity
        {
            Id = Guid.NewGuid(),
            DateTimeFrom = default,
            DateTimeTo = default,
            Name = "Activity 1",
            Color = 16711935,
            Project = project,
            ProjectId = project.Id,
            User = user,
            UserId = user.Id
        };
        var activity2InProject = new ActivityEntity
        {
            Id = Guid.NewGuid(),
            DateTimeFrom = default,
            DateTimeTo = default,
            Name = "Activity 2",
            Color = 16738740,
            Project = project,
            ProjectId = project.Id,
            User = user,
            UserId = user.Id
        };
            
        project.Activities.Add(activity1InProject);
        project.Activities.Add(activity2InProject);

        ProjectDbContextSUT.Users.Add(user);
        ProjectDbContextSUT.Projects.Add(project);
        await ProjectDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        var dbProject = await dbx.Projects.Include(i => i.Activities).SingleAsync(i => i.Id == project.Id);
        var dbActivity1 = await dbx.Activities.Include(i => i.Project).SingleAsync(i => i.Id == activity1InProject.Id);
        var dbActivity2 = await dbx.Activities.Include(i => i.Project).SingleAsync(i => i.Id == activity2InProject.Id);
            
        Assert.Equal(activity1InProject.User.Id, activity2InProject.User.Id);
        Assert.Equal(activity1InProject.ProjectId, activity2InProject.ProjectId);
            
        Assert.NotEqual(dbActivity1.Id, dbActivity2.Id);
            
        DeepAssert.Equal(project.Activities.Count, dbProject.Activities.Count);
    }

    [Fact]
    public async Task AddTwoActivitiesToTwoTags()
    {
        var user = UserSeeds.UserSeed();
        var tag1 = TagSeeds.TagSeed() with { UserId = user.Id };
        var tag2 = TagSeeds.TagSeed() with { UserId = user.Id };
        
        var activity1 = ActivitySeeds.ActivitySeed() with
        {
            User = user,
            UserId = user.Id
        };
        var activity2 = ActivitySeeds.ActivitySeed() with
        {
            User = user,
            UserId = user.Id
        };
            
        var tagActivity1 = new ActivityTagListEntity
        {
            Id = Guid.NewGuid(),
            ActivityId = activity1.Id,
            TagId = tag1.Id
        };
        var tagActivity2 = new ActivityTagListEntity
        {
            Id = Guid.NewGuid(),
            ActivityId = activity2.Id,
            TagId = tag2.Id
        };
            
        activity1.Tags.Add(tagActivity1);
        activity2.Tags.Add(tagActivity2);
        tag1.Activities.Add(tagActivity1);
        tag2.Activities.Add(tagActivity2);

        ProjectDbContextSUT.Users.Add(user);
        ProjectDbContextSUT.Tags.Add(tag1);
        ProjectDbContextSUT.Tags.Add(tag2);
        ProjectDbContextSUT.Activities.Add(activity1);
        ProjectDbContextSUT.Activities.Add(activity2);
        await ProjectDbContextSUT.SaveChangesAsync();
            
        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        var dbActivity1 = await dbx.Activities.Include(i => i.Tags).Include(i=>i.User).SingleAsync(i => i.Id == activity1.Id);
        var dbActivity2 = await dbx.Activities.Include(i => i.Tags).SingleAsync(i => i.Id == activity2.Id);
        var dbTag1 = await dbx.Tags.Include(i => i.Activities).SingleAsync(i => i.Id == tag1.Id);
        var dbTag2 = await dbx.Tags.Include(i => i.Activities).SingleAsync(i => i.Id == tag2.Id);
            
        Assert.Equal(activity1.Id, dbActivity1.Id);
        Assert.NotEqual(dbActivity1.Id, dbActivity2.Id);
        Assert.NotEqual(dbTag1.Id, dbTag2.Id);
            
        DeepAssert.Equal(activity1, dbActivity1);
    }

    [Fact]
    public async Task AddUserWithTodo_DeleteUser_TodoIsNull()
    {
        var user = UserSeeds.UserSeed();
        var todo = TodoSeeds.TodoSeed();
        user.Todos.Add(todo);

        ProjectDbContextSUT.Users.Add(user);
        ProjectDbContextSUT.Todos.Add(todo);
        await ProjectDbContextSUT.SaveChangesAsync();

        ProjectDbContextSUT.Remove(user);
        await ProjectDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        TodoEntity? dbTodo;
        try
        {
            dbTodo = await dbx.Todos.SingleAsync(i => i.Id == todo.Id);
        }
        catch (InvalidOperationException)
        {
            dbTodo = null;
        }

        Assert.Null(dbTodo);
    }
    
    [Fact]
    public async Task AddUserAndTodo_DeleteTodo_UserPersists()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var todo = TodoSeeds.TodoSeed() with
        {
            UserId = user.Id
        };
        user.Todos.Add(todo);
        
        // Act
        ProjectDbContextSUT.Users.Add(user);
        ProjectDbContextSUT.Todos.Add(todo);
        await ProjectDbContextSUT.SaveChangesAsync();

        ProjectDbContextSUT.Remove(todo);
        await ProjectDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory.CreateDbContextAsync();

        UserEntity? dbUser;
        try
        {
            dbUser = await dbx.Users.SingleAsync(i => i.Id == user.Id);
        }
        catch (InvalidOperationException)
        {
            dbUser = null;
        }

        // Assert
        Assert.NotNull(dbUser);
    }

    [Fact] 
    public async Task AddUserAndProject_DeleteUser_ProjectPreserved_BindingDeleted()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var project = ProjectSeeds.ProjectSeed();
        var binding = new UserProjectListEntity
        {
            Id = Guid.NewGuid(),
            ProjectId = project.Id,
            UserId = user.Id
        };
        user.Projects.Add(binding);
        project.Users.Add(binding);

        // Act
        ProjectDbContextSUT.Users.Add(user);
        ProjectDbContextSUT.Projects.Add(project);
        ProjectDbContextSUT.UPLists.Add(binding);
        await ProjectDbContextSUT.SaveChangesAsync();

        ProjectDbContextSUT.Users.Remove(user);
        await ProjectDbContextSUT.SaveChangesAsync();
        
        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        var dbProject = await dbx.Projects.SingleAsync(i => i.Id == project.Id);
        UserProjectListEntity? dbBinding;
        try
        {
            dbBinding = await dbx.UPLists.SingleAsync(i => i.Id == binding.Id);
        }
        catch (InvalidOperationException)
        {
            dbBinding = null;
        }
        
        UserEntity? dbUser;
        try
        {
            dbUser = await dbx.Users.SingleAsync(i => i.Id == user.Id);
        }
        catch (InvalidOperationException)
        {
            dbUser = null;
        }
        
        // Assert
        Assert.Null(dbBinding);
        Assert.Null(dbUser);
        DeepAssert.Equal(project, dbProject);
    }

    [Fact]
    public async Task AddUserAndProject_DeleteProject_UserPreserved_BindingDeleted()
    {
        // Arrange
        var projectos = ProjectSeeds.ProjectSeed();
        var user = UserSeeds.UserSeed();
        var binding = new UserProjectListEntity
        {
            Id = Guid.NewGuid(),
            ProjectId = projectos.Id,
            UserId = user.Id
        };
        projectos.Users.Add(binding);
        user.Projects.Add(binding);

        // Act
        ProjectDbContextSUT.Users.Add(user);
        ProjectDbContextSUT.Projects.Add(projectos);
        ProjectDbContextSUT.UPLists.Add(binding);
        await ProjectDbContextSUT.SaveChangesAsync();

        ProjectDbContextSUT.Projects.Remove(projectos);
        await ProjectDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        var dbUseros = await dbx.Users.SingleAsync(i => i.Id == user.Id);
        UserProjectListEntity? dbBinding;
        try
        {
            dbBinding = await dbx.UPLists.SingleAsync(i => i.Id == binding.Id);
        }
        catch (InvalidOperationException)
        {
            dbBinding = null;
        }

        ProjectEntity? dbProject;
        try
        {
            dbProject = await dbx.Projects.SingleAsync(i => i.Id == projectos.Id);
        }
        catch (InvalidOperationException)
        {
            dbProject = null;
        }

        // Assert
        Assert.Null(dbBinding);
        Assert.Null(dbProject);
        DeepAssert.Equal(user, dbUseros);
    }

    [Fact]
    public async Task AddActivityAndTag_DeleteActivity_TagPreserved_BindingDeleted()
    {
        // Arrange 
        var user = UserSeeds.UserSeed();
        var tag = TagSeeds.TagSeed() with
        {
            UserId = user.Id
        };
        var activity = ActivitySeeds.ActivitySeed() with
        {
            UserId = user.Id
        };
        var binding = new ActivityTagListEntity
        {
            Id = Guid.NewGuid(),
            ActivityId = activity.Id,
            TagId = tag.Id
        };
        
        activity.Tags.Add(binding);
        tag.Activities.Add(binding);
        tag.User = user;

        // Act
        ProjectDbContextSUT.Users.Add(user);
        ProjectDbContextSUT.Activities.Add(activity);
        ProjectDbContextSUT.Tags.Add(tag);
        ProjectDbContextSUT.ATLists.Add(binding);
        await ProjectDbContextSUT.SaveChangesAsync();

        ProjectDbContextSUT.Activities.Remove(activity);
        await ProjectDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        var dbTag = await dbx.Tags.Include(i => i.User).SingleAsync(i => i.Id == tag.Id);
        
        ActivityEntity? dbActivity;
        try
        {
            dbActivity = await dbx.Activities.SingleAsync(i => i.Id == activity.Id);
        }
        catch (InvalidOperationException)
        {
            dbActivity = null;
        }

        ActivityTagListEntity? dbAT;
        try
        {
            dbAT = await dbx.ATLists.SingleAsync(i => i.Id == binding.Id);
        }
        catch (InvalidOperationException)
        {
            dbAT = null;
        }
        
        // Assert
        Assert.Null(dbActivity);
        Assert.Null(dbAT);
        DeepAssert.Equal(tag, dbTag);
    }
    
    [Fact]
    public async Task AddActivityAndTag_DeleteTag_ActivityPreserved_BindingDeleted()
    {
        // Arrange 
        var user = UserSeeds.UserSeed();
        var tag = TagSeeds.TagSeed() with
        {
            UserId = user.Id
        };
        var activity = ActivitySeeds.ActivitySeed() with
        {
            UserId = user.Id
        };
        var binding = new ActivityTagListEntity
        {
            Id = Guid.NewGuid(),
            ActivityId = activity.Id,
            TagId = tag.Id
        };
        
        activity.Tags.Add(binding);
        tag.Activities.Add(binding);

        // Act
        ProjectDbContextSUT.Users.Add(user);
        ProjectDbContextSUT.Activities.Add(activity);
        ProjectDbContextSUT.Tags.Add(tag);
        ProjectDbContextSUT.ATLists.Add(binding);
        await ProjectDbContextSUT.SaveChangesAsync();

        ProjectDbContextSUT.Tags.Remove(tag);
        await ProjectDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        var dbActivity = await dbx.Activities.Include(i => i.User).SingleAsync(i => i.Id == activity.Id);
        
        TagEntity? dbTag;
        try
        {
            dbTag = await dbx.Tags.SingleAsync(i => i.Id == tag.Id);
        }
        catch (InvalidOperationException)
        {
            dbTag = null;
        }

        ActivityTagListEntity? dbAT;
        try
        {
            dbAT = await dbx.ATLists.SingleAsync(i => i.Id == binding.Id);
        }
        catch (InvalidOperationException)
        {
            dbAT = null;
        }
        
        // Assert
        Assert.Null(dbTag);
        Assert.Null(dbAT);
        DeepAssert.Equal(activity, dbActivity);
    }

    [Fact]
    public async Task AddUserAndTag_DeleteUser_TagIsNull()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var tag = TagSeeds.TagSeed() with
        {
            UserId = user.Id
        };
        
        // Act
        ProjectDbContextSUT.Tags.Add(tag);
        ProjectDbContextSUT.Users.Add(user);
        await ProjectDbContextSUT.SaveChangesAsync();

        ProjectDbContextSUT.Remove(user);
        await ProjectDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        TagEntity? dbTag;
        try
        {
            dbTag = await dbx.Tags.SingleAsync(i => i.Id == tag.Id);
        }
        catch (InvalidOperationException)
        {
            dbTag = null;
        }
        
        // Assert
        Assert.Null(dbTag);
    }

    [Fact]
    public async Task AddUserAndTag_DeleteTag_UserPersists()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var tag = TagSeeds.TagSeed() with
        {
            UserId = user.Id
        };

        // Act
        ProjectDbContextSUT.Tags.Add(tag);
        ProjectDbContextSUT.Users.Add(user);
        await ProjectDbContextSUT.SaveChangesAsync();
        
        ProjectDbContextSUT.Remove(tag);
        await ProjectDbContextSUT.SaveChangesAsync();
        
        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        UserEntity? dbUser;
        try
        {
            dbUser = await dbx.Users.SingleAsync(i => i.Id == user.Id);
        }
        catch (InvalidOperationException)
        {
            dbUser = null;
        }
        
        // Assert
        Assert.NotNull(dbUser);
        DeepAssert.Equal(user, dbUser);
    }

    [Fact]
    public async Task AddUserAndActivity_DeleteUser_ActivityIsNull()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity = ActivitySeeds.ActivitySeed() with
        {
            UserId = user.Id
        };
        
        // Act
        ProjectDbContextSUT.Activities.Add(activity);
        ProjectDbContextSUT.Users.Add(user);
        await ProjectDbContextSUT.SaveChangesAsync();

        ProjectDbContextSUT.Remove(user);
        await ProjectDbContextSUT.SaveChangesAsync();

        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        ActivityEntity? dbActivity;
        try
        {
            dbActivity = await dbx.Activities.SingleAsync(i => i.Id == activity.Id);
        }
        catch (InvalidOperationException)
        {
            dbActivity = null;
        }
        
        // Assert
        Assert.Null(dbActivity);
    }
    
    [Fact]
    public async Task AddUserAndActivity_DeleteActivity_UserPersists()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var activity = ActivitySeeds.ActivitySeed() with
        {
            UserId = user.Id
        };

        // Act
        ProjectDbContextSUT.Activities.Add(activity);
        ProjectDbContextSUT.Users.Add(user);
        await ProjectDbContextSUT.SaveChangesAsync();
        
        ProjectDbContextSUT.Remove(activity);
        await ProjectDbContextSUT.SaveChangesAsync();
        
        await using var dbx = await DbContextFactory.CreateDbContextAsync();
        UserEntity? dbUser;
        try
        {
            dbUser = await dbx.Users.SingleAsync(i => i.Id == user.Id);
        }
        catch (InvalidOperationException)
        {
            dbUser = null;
        }
        
        // Assert
        Assert.NotNull(dbUser);
        DeepAssert.Equal(user, dbUser);
    }
}