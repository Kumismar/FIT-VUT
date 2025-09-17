using project.BL.Models;

namespace project.BL.tests.ModelSeeds;

public static class UserSeeds
{
    private static int _counter = 0;
    private static int UserCounter() => ++_counter;

    public static UserDetailModel UserSeed() => new()
    {
        Id = Guid.NewGuid(),
        FullName = "Random name",
        UserName = $"UserName number {UserCounter()}"
    };
}