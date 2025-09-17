using System.Drawing;
using project.BL.Models;

namespace project.BL.tests.ModelSeeds;

public static class ActivitySeeds
{
    private static int _counter = 0;
    private static int ActivityCounter() => ++_counter; 
    public static ActivityDetailModel ActivitySeed() => new()
    {
        Id = Guid.NewGuid(),
        DateTimeFrom = default,
        DateTimeTo = default,
        Name = $"random activity number {ActivityCounter()}",
        Color = Color.Empty,
        UserId = Guid.Empty,
    };

}