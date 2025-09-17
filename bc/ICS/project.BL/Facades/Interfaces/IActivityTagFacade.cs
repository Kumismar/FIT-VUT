using project.DAL.Entities;

namespace project.BL.Facades.Interfaces;

public interface IActivityTagFacade
{
    Task SaveAsync(Guid activityId, Guid tagId);
    Task DeleteAsync(Guid activityId, Guid tagId);

}