namespace project.DAL.Entities;

public record TodoEntity : IEntityID
{
    public required Guid Id { get; set; }
    public required string Name { get; set; }
    public required DateOnly Date { get; set; }
    public required bool Finished { get; set; }
    public UserEntity? User { get; set; } 
    public required Guid UserId { get; set; }
}