using project.DAL.Entities;

namespace project.DAL.Mappers;

public interface IEntityIDMapper<in TEntity>
    where TEntity : IEntityID
{
    void MapToExistingEntity(TEntity existingEntity, TEntity newEntity);
}