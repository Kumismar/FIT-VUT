using Microsoft.EntityFrameworkCore;
using project.BL.Facades.Interfaces;
using project.BL.Mappers.Interfaces;
using project.BL.Models;
using project.DAL.Entities;
using project.DAL.Mappers;
using project.DAL.UnitOfWork;

namespace project.BL.Facades;

public class TodoFacade :
    FacadeBaseDetailOnly<TodoEntity, TodoDetailModel, TodoEntityMapper>, ITodoFacade
{
    private readonly ITodoModelMapper _todoModelMapper;
    public TodoFacade(
        IUnitOfWorkFactory unitOfWorkFactory,
        ITodoModelMapper modelMapper)
        : base(unitOfWorkFactory, modelMapper)
    {
        _todoModelMapper = modelMapper;
    }

    public async Task<IEnumerable<TodoDetailModel>> GetAsyncUser(Guid userId)
    {
        await using IUnitOfWork uow = UnitOfWorkFactory.Create();
        List<TodoEntity> entities = await uow
            .GetRepository<TodoEntity, TodoEntityMapper>()
            .Get()
            .Where(i => i.UserId == userId)
            .ToListAsync();

        return _todoModelMapper.MapToDetailModel(entities);
    }
}