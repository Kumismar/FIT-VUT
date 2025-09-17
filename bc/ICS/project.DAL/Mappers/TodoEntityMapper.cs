using project.DAL.Entities;

namespace project.DAL.Mappers;

public class TodoEntityMapper : IEntityIDMapper<TodoEntity>
{
    public void MapToExistingEntity(TodoEntity existingEntity, TodoEntity newEntity)
    {
        existingEntity.Name = newEntity.Name;
        existingEntity.Date = newEntity.Date;
        existingEntity.Finished= newEntity.Finished;
    }
}