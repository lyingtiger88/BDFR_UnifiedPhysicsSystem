using UnrealBuildTool;

public class BDFRPhysicsCore : ModuleRules
{
    public BDFRPhysicsCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "DeveloperSettings"
            }
        );
    }
}
