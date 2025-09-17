using System.Drawing;

namespace project.BL.Models;

public record TagDetailModel : ModelBase
{
    public required string Name { get; set; }
    public required Color Color { get; set; }
        
    public static TagDetailModel Empty => new()
    {
        Id = Guid.Empty,
        Name = string.Empty,
        Color = Color.Empty
    };
}