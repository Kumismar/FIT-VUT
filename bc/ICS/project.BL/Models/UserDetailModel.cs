using System.Collections.ObjectModel;

namespace project.BL.Models;

public record UserDetailModel : ModelBase
{
    public required string UserName { get; set; }
    public required string FullName { get; set; }
    public string? ImageUrl { get; set; }
    public ObservableCollection<ActivityListModel> Activities { get; init; } = new();

    public static UserDetailModel Empty => new()
    {
        FullName = string.Empty,
        UserName = string.Empty,
        Id = Guid.Empty
    };
}