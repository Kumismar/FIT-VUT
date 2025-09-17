using System.Collections.ObjectModel;
using System.Drawing;

namespace project.BL.Models;

public record ActivityListModel : ModelBase
{
    public required string Name { get; set; }
    public required DateTime DateTimeFrom { get; set; }
    public required DateTime DateTimeTo { get; set; }
    public required Color Color { get; set; }
    public ObservableCollection<TagDetailModel> Tags { get; set; } = new();
    public ProjectListModel? Project { get; set; }
    public static ActivityListModel Empty => new()
    {
        Id = Guid.Empty,
        Name = string.Empty,
        DateTimeFrom = default,
        DateTimeTo = default,
        Color = Color.Black
    };
}