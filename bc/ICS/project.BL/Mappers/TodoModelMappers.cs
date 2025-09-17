using Microsoft.EntityFrameworkCore.Metadata.Internal;
using project.BL.Mappers.Interfaces;
using project.BL.Models;
using project.DAL.Entities;

namespace project.BL.Mappers;

public class TodoModelMapper : ModelMapperBaseDetailOnly<TodoEntity, TodoDetailModel>, 
    ITodoModelMapper
{
    public override TodoDetailModel MapToDetailModel(TodoEntity? entity)
        => entity is null
            ? TodoDetailModel.Empty
            : new TodoDetailModel
            {
                Id = entity.Id, 
                Name = entity.Name,
                Date = entity.Date,
                Finished = entity.Finished
            };

    public override TodoEntity MapToEntity(TodoDetailModel model)
    {
        throw new NotSupportedException();
    }

    public override TodoEntity MapToEntity(TodoDetailModel model, Guid userGuid)
        => new()
        {
            Id = model.Id,
            Name = model.Name,
            Date = model.Date,
            Finished = model.Finished,
            UserId = userGuid
        };
}