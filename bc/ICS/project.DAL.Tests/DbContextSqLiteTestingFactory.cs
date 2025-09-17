using Microsoft.EntityFrameworkCore;

namespace project.DAL.Tests;

public class DbContextSqLiteTestingFactory : IDbContextFactory<ProjectDbContext>
{
    private readonly string _databaseName;

    public DbContextSqLiteTestingFactory(string databaseName)
    {
        _databaseName = databaseName;
    }
        
    public ProjectDbContext CreateDbContext() 
    {
        DbContextOptionsBuilder<ProjectDbContext> builder = new();
        builder.UseSqlite($"Data Source={_databaseName};Cache=Shared");
        builder.EnableSensitiveDataLogging();

        return new ProjectDbContext(builder.Options);
    }
}