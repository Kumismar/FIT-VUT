using System.Collections.ObjectModel;

namespace project.BL.Models;

public record ProjectDetailModel : ModelBase
{
    public required string Name { get; set; }
    public required string Description { get; set; }
    public ObservableCollection<UserListModel> Users { get; init; } = new();

    public static ProjectDetailModel Empty => new()
    {
        Id = Guid.Empty,
        Name = string.Empty,
        Description = string.Empty
    };
}