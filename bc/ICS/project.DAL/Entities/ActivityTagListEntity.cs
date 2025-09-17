namespace project.DAL.Entities;

public record ActivityTagListEntity : IEntityID
{
    public required Guid Id { get; set; }
    public required Guid ActivityId { get; set; }
    public ActivityEntity? Activity { get; init; }
    public required Guid TagId { get; set; }
    public TagEntity? Tag { get; init; }
}