namespace project.BL.Mappers.Interfaces;

public interface IModelMapperDetailOnly<TEntity, TDetailModel>
{
    TDetailModel MapToDetailModel(TEntity? entity);
    TEntity MapToEntity(TDetailModel model);
    TEntity MapToEntity(TDetailModel model, Guid userGuid);
    IEnumerable<TDetailModel> MapToDetailModel(IEnumerable<TEntity> entities)
        => entities.Select(MapToDetailModel);
}