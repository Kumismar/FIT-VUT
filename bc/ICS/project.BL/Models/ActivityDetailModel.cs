using System.Collections.ObjectModel;
using System.Drawing;

namespace project.BL.Models;

public record ActivityDetailModel : ModelBase
{
    public required string Name { get; set; }
    public required DateTime DateTimeFrom { get; set; }
    public required DateTime DateTimeTo { get; set; }
    public required Color Color { get; set; }
    public string? Description { get; set; }
    public ObservableCollection<TagDetailModel> Tags { get; set; } = new();
    public ProjectListModel? Project { get; set; }
    public required Guid UserId { get; set; }
    public static ActivityDetailModel Empty => new()
    {
        Id = Guid.Empty,
        Name = string.Empty,
        DateTimeFrom = default,
        DateTimeTo = default,
        Color = Color.Black,
        UserId = Guid.Empty
    };
}