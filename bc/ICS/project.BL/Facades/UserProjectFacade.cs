using Microsoft.EntityFrameworkCore;
using project.BL.Facades.Interfaces;
using project.BL.Mappers.Interfaces;
using project.DAL.Entities;
using project.DAL.Mappers;
using project.DAL.Repositories;
using project.DAL.UnitOfWork;

namespace project.BL.Facades;


public class UserProjectFacade : IUserProjectFacade
{
    private readonly IUnitOfWorkFactory _unitOfWorkFactory;
    public UserProjectFacade(IUnitOfWorkFactory unitOfWorkFactory)
    {
        _unitOfWorkFactory = unitOfWorkFactory;
    }
    
    public async Task SaveAsync(Guid userId, Guid projectId)
    {
        UserProjectListEntity bindingEntity = new()
        {
            Id = Guid.NewGuid(),
            UserId = userId,
            ProjectId = projectId
        };

        await using IUnitOfWork uow = _unitOfWorkFactory.Create();
        IRepository<UserProjectListEntity> repository = uow.GetRepository<UserProjectListEntity, UserProjectListEntityMapper>();

        await repository.InsertAsync(bindingEntity);

        await uow.CommitAsync();
    }

    public async Task DeleteAsync(Guid userId, Guid projectId)
    {
        await using IUnitOfWork uow = _unitOfWorkFactory.Create();

        IQueryable<UserProjectListEntity> query = uow.GetRepository<UserProjectListEntity, UserProjectListEntityMapper>().Get();

        UserProjectListEntity bindingEntity = await query.SingleAsync(i => i.UserId == userId && i.ProjectId == projectId);

        try
        {
            uow.GetRepository<UserProjectListEntity, UserProjectListEntityMapper>().Delete(bindingEntity.Id);
            await uow.CommitAsync().ConfigureAwait(false);
        }
        catch (DbUpdateException e)
        {
            throw new InvalidOperationException("Entity deletion failed.", e);
        }
    }
}