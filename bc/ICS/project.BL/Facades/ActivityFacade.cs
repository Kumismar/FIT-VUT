using Microsoft.EntityFrameworkCore;
using project.BL.Enums;
using project.BL.Facades.Interfaces;
using project.BL.Mappers.Interfaces;
using project.BL.Models;
using project.DAL.Entities;
using project.DAL.Mappers;
using project.DAL.Repositories;
using project.DAL.UnitOfWork;
using System.Diagnostics;

namespace project.BL.Facades;

public class ActivityFacade :
    FacadeBase<ActivityEntity, ActivityListModel, ActivityDetailModel, ActivityEntityMapper>, IActivityFacade
{
    private readonly IActivityModelMapper _modelMapper;
    public ActivityFacade(
        IUnitOfWorkFactory unitOfWorkFactory,
        IActivityModelMapper modelMapper)
        : base(unitOfWorkFactory, modelMapper)
    {
        _modelMapper = modelMapper;
    }

    public override async Task DeleteAsync(Guid id)
    {
        await using IUnitOfWork uow = UnitOfWorkFactory.Create();
        try
        {
            uow.GetRepository<ActivityEntity, ActivityEntityMapper>().Delete(id);
            await uow.CommitAsync().ConfigureAwait(false);
        }
        catch (DbUpdateException e)
        {
            throw new InvalidOperationException("Entity deletion failed.", e);
        }
    }

    public override async Task<ActivityDetailModel?> GetAsync(Guid id)
    {
        await using IUnitOfWork uow = UnitOfWorkFactory.Create();

        IQueryable<ActivityEntity> query = uow.GetRepository<ActivityEntity, ActivityEntityMapper>().Get();

        query = query.Include($"{nameof(ActivityEntity.Project)}");
        query = query.Include($"{nameof(ActivityEntity.Tags)}.{nameof(ActivityTagListEntity.Tag)}");


        ActivityEntity? entity = await query.SingleOrDefaultAsync(e => e.Id == id);

        return entity is null
            ? null
            : _modelMapper.MapToDetailModel(entity);
    }

    public async Task<ActivityDetailModel> SaveAsync(ActivityDetailModel model, Guid userId, Guid? projectId)
    {
        ActivityDetailModel result;

        GuardCollectionsAreNotSet(model);
    
        ActivityEntity entity = _modelMapper.MapToEntity(model, userId, projectId);

        await using IUnitOfWork uow = UnitOfWorkFactory.Create();
        IRepository<ActivityEntity> repository = uow.GetRepository<ActivityEntity, ActivityEntityMapper>();

        // guard for overlapping activities of an user
        DateTime ActivityFrom = entity.DateTimeFrom;
        DateTime ActivityTo = entity.DateTimeTo;

        IQueryable<ActivityEntity> query = repository
            .Get()
            .Where
                (i =>
                (i.UserId == entity.UserId) && 
                (
                ((i.DateTimeFrom <= ActivityFrom) && (i.DateTimeTo >= ActivityFrom)) || ((i.DateTimeFrom <= ActivityTo) && (i.DateTimeTo >= ActivityTo)) || 
                ((i.DateTimeFrom >= ActivityFrom) && (i.DateTimeFrom <= ActivityTo)) || ((i.DateTimeTo >= ActivityFrom) && (i.DateTimeTo <= ActivityTo))
                ));
        
        if (await repository.ExistsAsync(entity))
        {
            query = query.Where(i => i.Id != entity.Id);
        }

        List<ActivityEntity> OverlappingActivites = await query.ToListAsync();

        if (OverlappingActivites.Count > 0)
        {
            throw new OverlappingException();
        }

        if (await repository.ExistsAsync(entity)) 
        {
            ActivityEntity updatedEntity = await repository.UpdateAsync(entity);
            result = _modelMapper.MapToDetailModel(updatedEntity);
        }
        else
        {
            entity.Id = Guid.NewGuid();
            ActivityEntity insertedEntity = await repository.InsertAsync(entity);
            result = _modelMapper.MapToDetailModel(entity);
        }

        await uow.CommitAsync();

        return result;
    }

    public async Task<IEnumerable<ActivityListModel>> GetAsyncUser(Guid userId)
    {
        await using IUnitOfWork uow = UnitOfWorkFactory.Create();
        IQueryable<ActivityEntity> query = uow.GetRepository<ActivityEntity, ActivityEntityMapper>().Get().Where(i => i.UserId == userId);

        query = query.Include($"{nameof(ActivityEntity.Project)}");
        query = query.Include($"{nameof(ActivityEntity.Tags)}.{nameof(ActivityTagListEntity.Tag)}");

        List<ActivityEntity> entities = await query.ToListAsync();

        return _modelMapper.MapToListModel(entities);
    }

    public async Task<IEnumerable<ActivityListModel>> GetAsyncDateFilter(Guid userId, DateTime? from, DateTime? to)
    {
        await using IUnitOfWork uow = UnitOfWorkFactory.Create();
        IQueryable<ActivityEntity> query = uow.GetRepository<ActivityEntity, ActivityEntityMapper>().Get().Where(i => i.UserId == userId);

        if (from == null && to == null)
        {
            throw new NotSupportedException();
        }
        else if (from == null) 
        {
            query = query.Where(i => i.DateTimeTo <= to);
        }
        else if (to == null)
        {
            query = query.Where(i => i.DateTimeFrom >= from);
        }
        else
        {
            query = query.Where(i => i.DateTimeFrom >= from && i.DateTimeTo <= to);
        }

        query = query.Include($"{nameof(ActivityEntity.Project)}");
        query = query.Include($"{nameof(ActivityEntity.Tags)}.{nameof(ActivityTagListEntity.Tag)}");

        List<ActivityEntity> entities = await query.ToListAsync();

        return _modelMapper.MapToListModel(entities);
    }

    public async Task<IEnumerable<ActivityListModel>> GetAsyncIntervalFilter(Guid userId, FilterBy interval)
    {
        await using IUnitOfWork uow = UnitOfWorkFactory.Create();
        IQueryable<ActivityEntity> query = uow.GetRepository<ActivityEntity, ActivityEntityMapper>().Get().Where(i => i.UserId == userId);

        DateTime now = DateTime.Now;
        DateTime filter;

        if (interval == FilterBy.Week)
        {
            filter = now.AddDays(-7);
        }
        else if (interval == FilterBy.Month)
        {
            filter = now.AddMonths(-1);
        }
        else if (interval == FilterBy.Year)
        {
            filter = now.AddYears(-1);
        }
        else
        {
            now = now.AddMonths(-1);
            filter = now.AddMonths(-1);
        }

        query = query.Where(i => i.DateTimeFrom >= filter && i.DateTimeTo <= now);

        query = query.Include($"{nameof(ActivityEntity.Project)}");
        query = query.Include($"{nameof(ActivityEntity.Tags)}.{nameof(ActivityTagListEntity.Tag)}");

        List<ActivityEntity> entities = await query.ToListAsync();

        return _modelMapper.MapToListModel(entities);
    }

    public override Task<IEnumerable<ActivityListModel>> GetAsync()
    {
        throw new NotSupportedException();
    }

    public override Task<ActivityDetailModel> SaveAsync(ActivityDetailModel model)
    {
        throw new NotSupportedException();
    }

}