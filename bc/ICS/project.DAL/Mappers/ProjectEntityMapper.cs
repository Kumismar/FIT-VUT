using project.DAL.Entities;

namespace project.DAL.Mappers;

public class ProjectEntityMapper : IEntityIDMapper<ProjectEntity>
{
    public void MapToExistingEntity(ProjectEntity existingEntity, ProjectEntity newEntity)
    {
        existingEntity.Name= newEntity.Name;
        existingEntity.Description= newEntity.Description;
    }
}