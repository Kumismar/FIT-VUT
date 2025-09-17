namespace project.DAL.Entities;

public record ProjectEntity : IEntityID
{
    public required Guid Id { get; set; }
    public required string Name { get; set; }
    public required string Description { get; set; }
    public ICollection<UserProjectListEntity> Users { get; init; } = new List<UserProjectListEntity>();
    public ICollection<ActivityEntity> Activities { get; init; } = new List<ActivityEntity>();
}