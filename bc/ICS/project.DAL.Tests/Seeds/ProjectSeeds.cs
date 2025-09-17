using project.DAL.Entities;

namespace project.DAL.Tests.Seeds;

public static class ProjectSeeds
{
    private static int _counter = 0;
    private static int ProjectCounter() => ++_counter;
    public static ProjectEntity ProjectSeed() => new()
    {
        Id = Guid.NewGuid(),
        Name = $"Project number {ProjectCounter()}",
        Description = "random project"
    };
}