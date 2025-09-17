using project.DAL.Entities;

namespace project.DAL.Tests.Seeds;

public static class TodoSeeds
{
    private static int _counter = 0;
    private static int TodoCounter() => ++_counter;

    public static TodoEntity TodoSeed() => new()
    {
        Id = Guid.NewGuid(),
        Name = $"Todo number {TodoCounter()}",
        Date = default,
        Finished = false,
        UserId = default
    };

}