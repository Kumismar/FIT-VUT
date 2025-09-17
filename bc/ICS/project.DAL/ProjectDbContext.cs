using Microsoft.EntityFrameworkCore;
using project.DAL.Entities;

namespace project.DAL;

public class ProjectDbContext : DbContext
{
    private readonly bool _seedDemoData;

    public ProjectDbContext(DbContextOptions contextOptions, bool seedDemoData = false)
        : base(contextOptions) => _seedDemoData = seedDemoData;
    public DbSet<ActivityEntity> Activities => Set<ActivityEntity>();
    public DbSet<ActivityTagListEntity> ATLists => Set<ActivityTagListEntity>();
    public DbSet<ProjectEntity> Projects => Set<ProjectEntity>();
    public DbSet<TagEntity> Tags => Set<TagEntity>();
    public DbSet<TodoEntity> Todos => Set<TodoEntity>();
    public DbSet<UserEntity> Users => Set<UserEntity>();
    public DbSet<UserProjectListEntity> UPLists => Set<UserProjectListEntity>();

    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        base.OnModelCreating(modelBuilder);
            
        modelBuilder.Entity<UserEntity>()
            .HasMany(i => i.Projects)
            .WithOne(i => i.User)
            .OnDelete(DeleteBehavior.Cascade); 
            
        modelBuilder.Entity<UserEntity>()
            .HasMany(i => i.Todos)
            .WithOne(i => i.User)
            .OnDelete(DeleteBehavior.Restrict); 

        modelBuilder.Entity<UserEntity>()
            .HasMany(i => i.Activities)
            .WithOne(i => i.User)
            .OnDelete(DeleteBehavior.Restrict);
        
        modelBuilder.Entity<UserEntity>()
            .HasMany(i => i.Tags)
            .WithOne(i => i.User)
            .OnDelete(DeleteBehavior.Restrict);

        modelBuilder.Entity<ProjectEntity>()
            .HasMany(i => i.Users)
            .WithOne(i => i.Project)
            .OnDelete(DeleteBehavior.Cascade); 

        modelBuilder.Entity<ProjectEntity>()
            .HasMany(i => i.Activities)
            .WithOne(i => i.Project)
            .OnDelete(DeleteBehavior.Cascade); 

        modelBuilder.Entity<TagEntity>()
            .HasMany(i=>i.Activities)
            .WithOne(i => i.Tag)
            .OnDelete(DeleteBehavior.Cascade); 
        
        modelBuilder.Entity<TagEntity>()
            .HasOne(i => i.User)
            .WithMany(i => i.Tags)
            .OnDelete(DeleteBehavior.Cascade);
            
        modelBuilder.Entity<ActivityEntity>()
            .HasMany(i=>i.Tags)
            .WithOne(i => i.Activity)
            .OnDelete(DeleteBehavior.Cascade); 

        modelBuilder.Entity<ActivityEntity>()
            .HasOne(i => i.User)
            .WithMany(i => i.Activities)
            .OnDelete(DeleteBehavior.Cascade);

        modelBuilder.Entity<ActivityEntity>()
            .HasOne(i => i.Project)
            .WithMany(i => i.Activities)
            .OnDelete(DeleteBehavior.SetNull);

        modelBuilder.Entity<TodoEntity>()
            .HasOne(i=>i.User)
            .WithMany(i => i.Todos)
            .OnDelete(DeleteBehavior.Cascade);
    }
}