using Microsoft.EntityFrameworkCore;
using project.BL.Facades.Interfaces;
using project.BL.Mappers;
using project.BL.Mappers.Interfaces;
using project.BL.Models;
using project.DAL.Entities;
using project.DAL.Mappers;
using project.DAL.Repositories;
using project.DAL.UnitOfWork;

namespace project.BL.Facades;

public class ProjectFacade :
    FacadeBase<ProjectEntity, ProjectListModel, ProjectDetailModel, ProjectEntityMapper>, IProjectFacade
{
    private readonly IProjectModelMapper _projectModelMapper;
    public ProjectFacade(
        IUnitOfWorkFactory unitOfWorkFactory,
        IProjectModelMapper modelMapper)
        : base(unitOfWorkFactory, modelMapper)
    {
        _projectModelMapper = modelMapper;
    }

    public override async Task DeleteAsync(Guid id)
    {
        await using IUnitOfWork uow = UnitOfWorkFactory.Create();
        try
        {
            uow.GetRepository<ProjectEntity, ProjectEntityMapper>().Delete(id);
            await uow.CommitAsync().ConfigureAwait(false);
        }
        catch (DbUpdateException e)
        {
            throw new InvalidOperationException("Entity deletion failed.", e);
        }
    }

    public override async Task<ProjectDetailModel> SaveAsync(ProjectDetailModel model)
    {
        ProjectDetailModel result;

        GuardCollectionsAreNotSet(model);

        ProjectEntity entity = _projectModelMapper.MapToEntity(model);

        await using IUnitOfWork uow = UnitOfWorkFactory.Create();
        IRepository<ProjectEntity> repository = uow.GetRepository<ProjectEntity, ProjectEntityMapper>();

        if (await repository.ExistsAsync(entity))
        {
            ProjectEntity updatedEntity = await repository.UpdateAsync(entity);
            result = _projectModelMapper.MapToDetailModel(updatedEntity);
        }
        else
        {
            entity.Id = Guid.NewGuid();
            ProjectEntity insertedEntity = await repository.InsertAsync(entity);
            result = _projectModelMapper.MapToDetailModel(entity);
        }

        await uow.CommitAsync();

        return result;
    }

    public override async Task<ProjectDetailModel?> GetAsync(Guid id)
    {
        await using IUnitOfWork uow = UnitOfWorkFactory.Create();

        IQueryable<ProjectEntity> query = uow.GetRepository<ProjectEntity, ProjectEntityMapper>().Get();

        query = query.Include($"{nameof(ProjectEntity.Users)}.{nameof(UserProjectListEntity.User)}");

        ProjectEntity? entity = await query.SingleOrDefaultAsync(e => e.Id == id);

        return entity is null
            ? null
            : _projectModelMapper.MapToDetailModel(entity);
    }

    public override async Task<IEnumerable<ProjectListModel>> GetAsync()
    {
        await using IUnitOfWork uow = UnitOfWorkFactory.Create();

        List<ProjectEntity> entities = await uow.GetRepository<ProjectEntity, ProjectEntityMapper>().Get().ToListAsync();
        return _projectModelMapper.MapToListModel(entities);
    }
}