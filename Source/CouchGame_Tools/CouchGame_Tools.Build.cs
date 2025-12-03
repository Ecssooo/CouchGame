using UnrealBuildTool;

public class CouchGame_Tools : ModuleRules
{
    public CouchGame_Tools(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        OptimizeCode = CodeOptimization.Never;
        bUseUnity = false;
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CouchGame"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "ToolMenus",
                "UnrealEd",         
                "KismetCompiler",    
                "Kismet",            
                "AssetRegistry"
            }
        );
    }
}