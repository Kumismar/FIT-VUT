using Microsoft.EntityFrameworkCore.Design;

namespace project.DAL.Factories;

public class DesignTimeDbContextFactory : IDesignTimeDbContextFactory<ProjectDbContext>
{
    private const string connectionString = $"database;Cache=Shared";
    private readonly DbContextSqLiteFactory _dbContextSqLiteFactory;

    public DesignTimeDbContextFactory() 
    {
        _dbContextSqLiteFactory = new DbContextSqLiteFactory(connectionString);
    }
    public ProjectDbContext CreateDbContext(string[] args) => _dbContextSqLiteFactory.CreateDbContext();
}