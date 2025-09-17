using System.Drawing;
using project.BL.Models;

namespace project.BL.tests.ModelSeeds;

public class TodoSeeds
{
    private static int _counter = 0;
    private static int TodoCounter() => ++_counter;

    public static TodoDetailModel TodoSeed() => new()
    {
        Id = Guid.NewGuid(),
        Name = $"Todo number {TodoCounter()}",
        Date = default,
        Finished = false,
    };
}