namespace project.BL.Mappers.Interfaces;

public interface IModelMapper<TEntity, out TListModel, TDetailModel>
{
    TDetailModel MapToDetailModel(TEntity entity);
    TListModel MapToListModel(TEntity entity);
    TEntity MapToEntity(TDetailModel model);
    IEnumerable<TListModel> MapToListModel(IEnumerable<TEntity> entities);
}