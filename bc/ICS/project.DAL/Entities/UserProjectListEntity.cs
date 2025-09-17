namespace project.DAL.Entities;

public record UserProjectListEntity : IEntityID
{
    public required Guid Id { get; set; }
    public required Guid ProjectId { get; set; }
    public ProjectEntity? Project { get; set; }
    public required Guid UserId { get; set; }
    public UserEntity? User { get; set; }
}