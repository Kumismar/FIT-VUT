using project.BL.Mappers.Interfaces;
using project.BL.Models;
using project.DAL.Entities;
using System.Diagnostics;
using System.Drawing;
using Microsoft.IdentityModel.Tokens;

namespace project.BL.Mappers;

public class UserModelMapper : ModelMapperBase<UserEntity, UserListModel, UserDetailModel>,
    IUserModelMapper
{
    public override UserListModel MapToListModel(UserEntity? entity)
    => entity is null ?
        UserListModel.Empty :
        new UserListModel
        {
            Id = entity.Id,
            UserName = entity.UserName,
            ImageUrl = entity.ImageUrl
        };

    public override UserEntity MapToEntity(UserDetailModel model)
        => new()
        {
            Id = model.Id,
            FullName = model.FullName,
            UserName = model.UserName,
            ImageUrl = model.ImageUrl
        };

    public override UserDetailModel MapToDetailModel(UserEntity? entity)
    {
        var activityMapper = new ActivityModelMapper();
        return entity is null
            ? UserDetailModel.Empty
            : new UserDetailModel
            {
                Id = entity.Id,
                FullName = entity.FullName,
                UserName = entity.UserName,
                ImageUrl = entity.ImageUrl,
                Activities = activityMapper.MapToListModel(entity.Activities).ToObservableCollection()
            };
    }

    public UserListModel MapToListModel(UserProjectListEntity entity) 
        => entity.User is null 
            ? UserListModel.Empty 
            : new UserListModel
            {
                Id = entity.UserId,
                UserName = entity.User.UserName,
                ImageUrl = entity.User.ImageUrl
            };
    public IEnumerable<UserListModel> MapToListModel(IEnumerable<UserProjectListEntity> entities)
    {
        var projectUserListEntities = entities.ToList();
        return projectUserListEntities.IsNullOrEmpty() ?
            Enumerable.Empty<UserListModel>() :
            projectUserListEntities.Select(MapToListModel);
    }
}