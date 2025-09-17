using System.Drawing;
using project.BL.Mappers.Interfaces;
using project.BL.Models;
using project.DAL.Entities;
using project.BL;
using System.Diagnostics;

namespace project.BL.Mappers;

public class ProjectModelMapper : ModelMapperBase<ProjectEntity, ProjectListModel, ProjectDetailModel>,
    IProjectModelMapper
{
    public override ProjectListModel MapToListModel(ProjectEntity? entity)
        => entity is null ?
        ProjectListModel.Empty :
        new ProjectListModel
        {
            Name = entity.Name,
            Id = entity.Id
        };

    public override ProjectEntity MapToEntity(ProjectDetailModel project)
        => new()
        {
            Id = project.Id,
            Name = project.Name,
            Description = project.Description
        };

    public override ProjectDetailModel MapToDetailModel(ProjectEntity? entity)
    {
        var userMapper = new UserModelMapper();
        return entity is null
            ? ProjectDetailModel.Empty
            : new ProjectDetailModel
            {
                Id = entity.Id,
                Name = entity.Name,
                Description = entity.Description,
                Users = userMapper.MapToListModel(entity.Users).ToObservableCollection()
            };
    }
}