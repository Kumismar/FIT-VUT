using Microsoft.Identity.Client;
using project.BL.Facades;
using project.BL.Models;
using project.BL.tests.ModelSeeds;
using project.DAL.Tests;
using System.Runtime.Intrinsics.X86;
using Xunit.Abstractions;

namespace project.BL.tests;

public class TodoFacadeTests : FacadeTestsBase
{
    private readonly TodoFacade _todoFacade;
    private readonly UserFacade _userFacade;
    public TodoFacadeTests(ITestOutputHelper output) : base(output)
    {
        _userFacade = new UserFacade(UnitOfWorkFactory, UserModelMapper);
        _todoFacade = new TodoFacade(UnitOfWorkFactory, TodoModelMapper);
    }

    [Fact]
    public async Task CreateTodo_Success()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var todo = TodoSeeds.TodoSeed();
        
        // Act
        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedTodo = await _todoFacade.SaveAsync(todo, returnedUser.Id);
        
        // Assert
        Assert.NotNull(returnedTodo);
        FixIds(todo, returnedTodo);
        DeepAssert.Equal(todo, returnedTodo);
    }

    [Fact]
    public async Task DeleteTodo_GetById_IsNull()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var todo = TodoSeeds.TodoSeed();
        
        // Act
        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedTodo = await _todoFacade.SaveAsync(todo, returnedUser.Id);

        await _todoFacade.DeleteAsync(returnedTodo.Id);
        var dbTodo = await _todoFacade.GetAsync(returnedTodo.Id);
        await _userFacade.DeleteAsync(returnedUser.Id);
        var dbUser = await _userFacade.GetAsync(returnedUser.Id);
        
        // Assert
        Assert.Null(dbTodo);
        Assert.Null(dbUser);
    }


    [Fact]
    public async Task DeleteNonExistingTodo_Exception()
    {
        // Arrange
        var todo = TodoSeeds.TodoSeed();

        // Act & Assert
        await Assert.ThrowsAsync<InvalidOperationException>(async () => await _todoFacade.DeleteAsync(todo.Id));
    }


    [Fact]
    public async Task AddUserWithTodos_DeleteUser_FindTodos_Fail()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var todo1 = TodoSeeds.TodoSeed();

        // Act
        var returnedUser = await _userFacade.SaveAsync(user);
        var retTodo1 = await _todoFacade.SaveAsync(todo1, returnedUser.Id);

        await _userFacade.DeleteAsync(returnedUser.Id);
        var dbTodo1 = await _todoFacade.GetAsync(retTodo1.Id);
        
        // Assert
        Assert.Null(dbTodo1);
    }

    [Fact]
    public async Task OneUser_MoreTodos_GetAll()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var todo1 = TodoSeeds.TodoSeed();
        var todo2 = TodoSeeds.TodoSeed();
        var todo3 = TodoSeeds.TodoSeed();
        var todos = new List<TodoDetailModel>
        {
            todo1, todo2, todo3
        };

        // Act
        var returnedUser = await _userFacade.SaveAsync(user);
        var retTodo1 = await _todoFacade.SaveAsync(todo1, returnedUser.Id);
        var retTodo2 = await _todoFacade.SaveAsync(todo2, returnedUser.Id);
        var retTodo3 = await _todoFacade.SaveAsync(todo3, returnedUser.Id);
        
        var returnedTodoList = await _todoFacade.GetAsyncUser(returnedUser.Id);
        var returnedTodos = returnedTodoList.ToList();
        FixListIds(todos, returnedTodos);
        
        // Assert
        DeepAssert.Equal(todos, returnedTodos);

        // Another way
        Assert.Contains(retTodo1, returnedTodoList);
        Assert.Contains(retTodo2, returnedTodoList);
        Assert.Contains(retTodo3, returnedTodoList);
    }

    [Fact]
    public async Task MoreUsers_MoreTodos()
    {
        // Arrange
        var user1 = UserSeeds.UserSeed();
        var user2 = UserSeeds.UserSeed();
        var user3 = UserSeeds.UserSeed();

        var todo1 = TodoSeeds.TodoSeed();
        var todo2 = TodoSeeds.TodoSeed();
        var todo3 = TodoSeeds.TodoSeed();
        var todo4 = TodoSeeds.TodoSeed();
        var todo5 = TodoSeeds.TodoSeed();
        
        var todosUser1 = new List<TodoDetailModel>
        {
            todo1, todo3, todo5
        };
        var todosUser2 = new List<TodoDetailModel>
        {
            todo2
        };
        var todosUser3 = new List<TodoDetailModel>
        {
            todo4
        };

        // Act
        var returnedUser1 = await _userFacade.SaveAsync(user1);
        var returnedUser2 = await _userFacade.SaveAsync(user2);
        var returnedUser3 = await _userFacade.SaveAsync(user3);
        var retTodo1 = await _todoFacade.SaveAsync(todo1, returnedUser1.Id);
        var retTodo3 = await _todoFacade.SaveAsync(todo3, returnedUser1.Id);
        var retTodo5 = await _todoFacade.SaveAsync(todo5, returnedUser1.Id);
        var retTodo2 = await _todoFacade.SaveAsync(todo2, returnedUser2.Id);
        var retTodo4 = await _todoFacade.SaveAsync(todo4, returnedUser3.Id);
        
        var returnedTodoListUser1 = await _todoFacade.GetAsyncUser(returnedUser1.Id);
        var returnedTodosUser1 = returnedTodoListUser1.ToList();
        
        var returnedTodoListUser2 = await _todoFacade.GetAsyncUser(returnedUser2.Id);
        var returnedTodosUser2 = returnedTodoListUser2.ToList();
        
        var returnedTodoListUser3 = await _todoFacade.GetAsyncUser(returnedUser3.Id);
        var returnedTodosUser3 = returnedTodoListUser3.ToList();

        // Assert
        FixListIds(todosUser1, returnedTodosUser1);
        FixListIds(todosUser2, returnedTodosUser2);
        FixListIds(todosUser3, returnedTodosUser3);
        DeepAssert.Equal(todosUser1, returnedTodosUser1);
        DeepAssert.Equal(todosUser2, returnedTodosUser2);
        DeepAssert.Equal(todosUser3, returnedTodosUser3);


        // Another way
        Assert.Contains(retTodo1, returnedTodoListUser1);
        Assert.Contains(retTodo3, returnedTodoListUser1);
        Assert.Contains(retTodo5, returnedTodoListUser1);
        Assert.DoesNotContain(retTodo2, returnedTodoListUser1);
        Assert.DoesNotContain(retTodo4, returnedTodoListUser1);

        Assert.Contains(retTodo2, returnedTodoListUser2);
        Assert.DoesNotContain(retTodo1, returnedTodoListUser2);
        Assert.DoesNotContain(retTodo3, returnedTodoListUser2);
        Assert.DoesNotContain(retTodo4, returnedTodoListUser2);
        Assert.DoesNotContain(retTodo5, returnedTodoListUser2);

        Assert.Contains(retTodo4, returnedTodoListUser3);
        Assert.DoesNotContain(retTodo1, returnedTodoListUser3);
        Assert.DoesNotContain(retTodo2, returnedTodoListUser3);
        Assert.DoesNotContain(retTodo3, returnedTodoListUser3);
        Assert.DoesNotContain(retTodo5, returnedTodoListUser3);
    }


    [Fact]
    public async Task EmptyTodoList()
    {
        // Arrange
        var user = UserSeeds.UserSeed();

        // Act
        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedTodoListUser = await _todoFacade.GetAsyncUser(returnedUser.Id);

        //Assert
        Assert.Empty(returnedTodoListUser);
    }


    [Fact]
    public async Task UpdateTodo_Correct()
    {
        // Arrange
        var user = UserSeeds.UserSeed();
        var todo = TodoSeeds.TodoSeed();

        // Act
        var returnedUser = await _userFacade.SaveAsync(user);
        var returnedTodo = await _todoFacade.SaveAsync(todo, returnedUser.Id);

        var DbTodo = await _todoFacade.GetAsync(returnedTodo.Id);

        Assert.NotNull(DbTodo);

        DbTodo.Finished = true;

        // Update
        await _todoFacade.SaveAsync(DbTodo, returnedUser.Id);

        var UpdatedDbTodo = await _todoFacade.GetAsync(DbTodo.Id);

        // Assert
        DeepAssert.Equal(UpdatedDbTodo, DbTodo);
    }


        private static void FixIds(TodoDetailModel expectedModel, TodoDetailModel returnedModel)
    {
        returnedModel.Id = expectedModel.Id;
    }
    
    private static void FixListIds(List<TodoDetailModel> expected, List<TodoDetailModel> returned)
    {
        for (int i = 0; i < returned.Count; i++)
        {
            FixIds(expected[i], returned[i]);
        }
    }
}