namespace project.DAL.Entities;

public record TagEntity : IEntityID
{
    public required Guid Id { get; set; }
    public required string Name { get; set; }
    public required int Color { get; set; }
    public UserEntity? User { get; set; }
    public required Guid UserId { get; set; }
    public ICollection<ActivityTagListEntity> Activities { get; init; } = new List<ActivityTagListEntity>();
}