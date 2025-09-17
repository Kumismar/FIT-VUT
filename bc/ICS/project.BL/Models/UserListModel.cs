namespace project.BL.Models;

public record UserListModel : ModelBase
{
    public required string UserName { get; set; }
    public string? ImageUrl { get; set; }
    public static UserListModel Empty => new()
    {
        Id = Guid.Empty,
        UserName = string.Empty
    };
}