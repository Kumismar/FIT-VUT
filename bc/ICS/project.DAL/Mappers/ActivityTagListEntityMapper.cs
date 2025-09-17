using project.DAL.Entities;

namespace project.DAL.Mappers;

public class ActivityTagListEntityMapper : IEntityIDMapper<ActivityTagListEntity>
{
    public void MapToExistingEntity(ActivityTagListEntity existingEntity, ActivityTagListEntity newEntity)
    {
        existingEntity.ActivityId = newEntity.ActivityId;
        existingEntity.TagId = newEntity.TagId;
    }
}