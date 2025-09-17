using project.BL.Models;
using project.DAL.Entities;

namespace project.BL.Facades.Interfaces;

public interface ITagFacade : IFacadeDetailOnly<TagEntity, TagDetailModel>
{
    Task<IEnumerable<TagDetailModel>> GetAsyncUser(Guid userId);
}