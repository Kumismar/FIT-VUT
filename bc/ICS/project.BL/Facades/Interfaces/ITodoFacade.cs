using project.BL.Models;
using project.DAL.Entities;

namespace project.BL.Facades.Interfaces;

public interface ITodoFacade : IFacadeDetailOnly<TodoEntity, TodoDetailModel>
{
    Task<IEnumerable<TodoDetailModel>> GetAsyncUser(Guid userId);
}