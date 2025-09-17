using Microsoft.EntityFrameworkCore;
using project.BL.Facades.Interfaces;
using project.BL.Mappers.Interfaces;
using project.BL.Models;
using project.DAL.Entities;
using project.DAL.Mappers;
using project.DAL.UnitOfWork;

namespace project.BL.Facades;

public class TagFacade :
    FacadeBaseDetailOnly<TagEntity, TagDetailModel, TagEntityMapper>, ITagFacade
{
    private readonly ITagModelMapper _tagModelMapper;
    public TagFacade(
        IUnitOfWorkFactory unitOfWorkFactory,
        ITagModelMapper modelMapper)
        : base(unitOfWorkFactory, modelMapper)
    {
        _tagModelMapper = modelMapper;
    }

    public async Task<IEnumerable<TagDetailModel>> GetAsyncUser(Guid userId)
    {
        await using IUnitOfWork uow = UnitOfWorkFactory.Create();
        List<TagEntity> entities = await uow
            .GetRepository<TagEntity, TagEntityMapper>()
            .Get()
            .Where(i => i.UserId == userId)
            .ToListAsync();

        return _tagModelMapper.MapToDetailModel(entities);
    }
}