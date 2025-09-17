using Microsoft.Extensions.DependencyInjection;
using project.BL.Facades;
using project.BL.Facades.Interfaces;
using project.BL.Mappers.Interfaces;

namespace project.BL;

public static class BLInstaller
{
    public static IServiceCollection AddBLServices(this IServiceCollection services)
    {
        services.AddSingleton<IUserProjectFacade, UserProjectFacade>();
        services.Scan(selector => selector
            .FromAssemblyOf<BusinessLogic>()
            .AddClasses(filter => filter.AssignableTo(typeof(IFacade<,,>)))
            .AsMatchingInterface()
            .WithSingletonLifetime()
        );

        services.Scan(selector => selector
            .FromAssemblyOf<BusinessLogic>()
            .AddClasses(filter => filter.AssignableTo(typeof(IFacadeDetailOnly<,>)))
            .AsMatchingInterface()
            .WithSingletonLifetime()
        );

        services.Scan(selector => selector
            .FromAssemblyOf<BusinessLogic>()
            .AddClasses(filter => filter.AssignableTo(typeof(IModelMapper<,,>)))
            .AsMatchingInterface()
            .WithSingletonLifetime()
        );

        services.Scan(selector => selector
            .FromAssemblyOf<BusinessLogic>()
            .AddClasses(filter => filter.AssignableTo(typeof(IModelMapperDetailOnly<,>)))
            .AsMatchingInterface()
            .WithSingletonLifetime()
        );

        return services;
    }
}
