using project.DAL.Entities;

namespace project.DAL.Mappers;

public class ActivityEntityMapper : IEntityIDMapper<ActivityEntity>
{
    public void MapToExistingEntity(ActivityEntity existingEntity, ActivityEntity newEntity)
    {
        existingEntity.DateTimeFrom = newEntity.DateTimeFrom;
        existingEntity.DateTimeTo = newEntity.DateTimeTo;
        existingEntity.Name = newEntity.Name;
        existingEntity.Description = newEntity.Description;
        existingEntity.Color = newEntity.Color;
        existingEntity.ProjectId = newEntity.ProjectId;
    }
}