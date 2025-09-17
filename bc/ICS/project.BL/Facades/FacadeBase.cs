using System.Collections;
using System.Reflection;
using Microsoft.EntityFrameworkCore;
using project.BL.Facades.Interfaces;
using project.BL.Mappers.Interfaces;
using project.BL.Models;
using project.DAL.Entities;
using project.DAL.Mappers;
using project.DAL.Repositories;
using project.DAL.UnitOfWork;

namespace project.BL.Facades;

public abstract class
    FacadeBase<TEntity, TListModel, TDetailModel, TEntityMapper> : IFacade<TEntity, TListModel, TDetailModel>
    where TEntity : class, IEntityID
    where TListModel : IModel
    where TDetailModel : class, IModel
    where TEntityMapper : IEntityIDMapper<TEntity>, new()
{
    protected readonly IModelMapper<TEntity, TListModel, TDetailModel> ModelMapper;
    protected readonly IUnitOfWorkFactory UnitOfWorkFactory;

    protected FacadeBase(
        IUnitOfWorkFactory unitOfWorkFactory,
        IModelMapper<TEntity, TListModel, TDetailModel> modelMapper)
    {
        UnitOfWorkFactory = unitOfWorkFactory;
        ModelMapper = modelMapper;
    }

    public abstract Task DeleteAsync(Guid id);

    public abstract Task<TDetailModel?> GetAsync(Guid id);

    public abstract Task<IEnumerable<TListModel>> GetAsync();
    public abstract Task<TDetailModel> SaveAsync(TDetailModel model);

    public static void GuardCollectionsAreNotSet(TDetailModel model)
    {
        IEnumerable<PropertyInfo> collectionProperties = model
            .GetType()
            .GetProperties()
            .Where(i => typeof(ICollection).IsAssignableFrom(i.PropertyType));

        foreach (PropertyInfo collectionProperty in collectionProperties)
        {
            if (collectionProperty.GetValue(model) is ICollection { Count: > 0 })
            {
                throw new InvalidOperationException(
                    "Current BL and DAL infrastructure disallows insert or update of models with adjacent collections.");
            }
        }
    }
}