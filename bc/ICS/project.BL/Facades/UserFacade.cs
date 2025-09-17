using Microsoft.EntityFrameworkCore;
using project.BL.Facades.Interfaces;
using project.BL.Mappers.Interfaces;
using project.BL.Models;
using project.DAL.Entities;
using project.DAL.Mappers;
using project.DAL.Repositories;
using project.DAL.UnitOfWork;

namespace project.BL.Facades;

public class UserFacade :
    FacadeBase<UserEntity, UserListModel, UserDetailModel, UserEntityMapper>, IUserFacade
{
    private readonly IUserModelMapper _userModelMapper;
    public UserFacade(
        IUnitOfWorkFactory unitOfWorkFactory,
        IUserModelMapper modelMapper)
        : base(unitOfWorkFactory, modelMapper)
    {
        _userModelMapper = modelMapper;
    }

    public override async Task DeleteAsync(Guid id)
    {
        await using IUnitOfWork uow = UnitOfWorkFactory.Create();
        try
        {
            uow.GetRepository<UserEntity, UserEntityMapper>().Delete(id);
            await uow.CommitAsync().ConfigureAwait(false);
        }
        catch (DbUpdateException e)
        {
            throw new InvalidOperationException("Entity deletion failed.", e);
        }
    }

    public override async Task<UserDetailModel> SaveAsync(UserDetailModel model)
    {
        UserDetailModel result;

        GuardCollectionsAreNotSet(model);

        UserEntity entity = _userModelMapper.MapToEntity(model);

        await using IUnitOfWork uow = UnitOfWorkFactory.Create();
        IRepository<UserEntity> repository = uow.GetRepository<UserEntity, UserEntityMapper>();

        if (await repository.ExistsAsync(entity)) 
        {
            UserEntity updatedEntity = await repository.UpdateAsync(entity);
            result = _userModelMapper.MapToDetailModel(updatedEntity);
        }
        else
        {
            entity.Id = Guid.NewGuid();
            UserEntity insertedEntity = await repository.InsertAsync(entity);
            result = _userModelMapper.MapToDetailModel(entity);
        }

        await uow.CommitAsync();

        return result;
    }

    public override async Task<UserDetailModel?> GetAsync(Guid id)
    {
        await using IUnitOfWork uow = UnitOfWorkFactory.Create();

        IQueryable<UserEntity> query = uow.GetRepository<UserEntity, UserEntityMapper>().Get();

        query = query.Include($"{nameof(UserEntity.Activities)}.{nameof(ActivityEntity.Project)}");
        query = query.Include($"{nameof(UserEntity.Activities)}.{nameof(ActivityEntity.Tags)}.{nameof(ActivityTagListEntity.Tag)}");

        UserEntity? entity = await query.SingleOrDefaultAsync(e => e.Id == id);

        return entity is null
            ? null
            : _userModelMapper.MapToDetailModel(entity);
    }

    public override async Task<IEnumerable<UserListModel>> GetAsync()
    {
        await using IUnitOfWork uow = UnitOfWorkFactory.Create();

        List<UserEntity> entities = await uow.GetRepository<UserEntity, UserEntityMapper>().Get().ToListAsync();
        return _userModelMapper.MapToListModel(entities);
    }
}