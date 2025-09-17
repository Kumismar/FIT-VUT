using System.Drawing;
using project.BL.Models;

namespace project.BL.tests.ModelSeeds;

public static class TagSeeds
{
    private static int _counter = 0;
    private static int TagCounter() => ++_counter;

    public static TagDetailModel TagSeed() => new()
    {
        Id = Guid.NewGuid(),
        Name = $"Tag number {TagCounter()}",
        Color = Color.Empty
    };
}