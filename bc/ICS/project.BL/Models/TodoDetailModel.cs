namespace project.BL.Models;

public record TodoDetailModel : ModelBase
{
    public required string Name { get; set; }
    public required DateOnly Date { get; set; }
    public required bool Finished { get; set; }

    public static TodoDetailModel Empty => new()
    {
        Id = Guid.Empty,
        Name = string.Empty,
        Date = default,
        Finished = false
    };
}