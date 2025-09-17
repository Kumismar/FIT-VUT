namespace project.DAL.Entities;

public record UserEntity : IEntityID
{
    public required Guid Id { get; set; }
    public required string FullName { get; set; }
    public required string UserName { get; set; }
    public string? ImageUrl { get; set; }
    public ICollection<UserProjectListEntity> Projects { get; init; } = new List<UserProjectListEntity>();
    public ICollection<ActivityEntity> Activities { get; init; } = new List<ActivityEntity>();
    public ICollection<TodoEntity> Todos { get; init; } = new List<TodoEntity>();
    public ICollection<TagEntity> Tags { get; init; } = new List<TagEntity>();
}