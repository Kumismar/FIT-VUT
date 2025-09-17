using project.DAL.Entities;
using project.DAL.Mappers;
using project.DAL.Repositories;

namespace project.DAL.UnitOfWork;

public interface IUnitOfWork : IAsyncDisposable
{
    IRepository<TEntity> GetRepository<TEntity, TEntityMapper>()
        where TEntity : class, IEntityID
        where TEntityMapper : IEntityIDMapper<TEntity>, new();

    Task CommitAsync();
}