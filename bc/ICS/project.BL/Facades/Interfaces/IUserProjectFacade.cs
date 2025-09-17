using project.DAL.Entities;

namespace project.BL.Facades.Interfaces;

public interface IUserProjectFacade
{
    Task SaveAsync(Guid userId, Guid projectId);
    Task DeleteAsync(Guid userId, Guid projectId);
}