using System.Collections.ObjectModel;
using System.Drawing;
using Microsoft.EntityFrameworkCore.Metadata.Internal;
using Microsoft.IdentityModel.Tokens;
using project.BL.Mappers.Interfaces;
using project.BL.Models;
using project.DAL.Entities;

namespace project.BL.Mappers;

public class TagModelMapper : ModelMapperBaseDetailOnly<TagEntity, TagDetailModel>,
    ITagModelMapper
{
    public override TagDetailModel MapToDetailModel(TagEntity? entity)
        => entity is null
            ? TagDetailModel.Empty
            : new TagDetailModel
            {
                Id = entity.Id,
                Name = entity.Name,
                Color = Color.FromArgb(entity.Color)
            };

    public override TagEntity MapToEntity(TagDetailModel model)
    {
        throw new NotSupportedException();
    }
    public override TagEntity MapToEntity(TagDetailModel model, Guid userGuid)
        => new()
        {
            Id = model.Id,
            Name = model.Name,
            Color = model.Color.ToArgb(),
            UserId = userGuid
        };

    public TagDetailModel MapToDetailModel(ActivityTagListEntity entity) =>
        entity.Tag is null
            ? TagDetailModel.Empty
            : new TagDetailModel
            {
                Id = entity.Tag.Id,
                Name = entity.Tag.Name,
                Color = Color.FromArgb(entity.Tag.Color)
            };
    
    public IEnumerable<TagDetailModel> MapToDetailModel(IEnumerable<ActivityTagListEntity> entities)
    {
        var activityTagListEntities = entities.ToList();
        return activityTagListEntities.IsNullOrEmpty() ? 
            Enumerable.Empty<TagDetailModel>() : 
            activityTagListEntities.Select(MapToDetailModel);
    }
}

    