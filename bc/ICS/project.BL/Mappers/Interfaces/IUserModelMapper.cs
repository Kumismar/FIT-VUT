using System.Collections;
using Microsoft.IdentityModel.Tokens;
using project.BL.Models;
using project.DAL.Entities;

namespace project.BL.Mappers.Interfaces;
public interface IUserModelMapper : IModelMapper<UserEntity, UserListModel, UserDetailModel>
{
    UserListModel MapToListModel(UserProjectListEntity entity);
    IEnumerable<UserListModel> MapToListModel(IEnumerable<UserProjectListEntity> entities);
}