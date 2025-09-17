using project.BL.Models;
using project.DAL.Entities;

namespace project.BL.Mappers.Interfaces;

public interface IActivityModelMapper : IModelMapper<ActivityEntity, ActivityListModel, ActivityDetailModel>
{
    ActivityEntity MapToEntity(ActivityDetailModel activity, Guid userGuid, Guid? projectGuid);
    ActivityEntity MapToEntity(ActivityDetailModel activity, Guid? projectGuid);
}