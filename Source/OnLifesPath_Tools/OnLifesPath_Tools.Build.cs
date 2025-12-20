using UnrealBuildTool;

public class OnLifesPath_Tools : ModuleRules
{
    public OnLifesPath_Tools(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        OptimizeCode = CodeOptimization.Never;
        bUseUnity = false;
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "OnLifesPath"
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
                "AssetRegistry", 
                "EditorFramework",
                "UnrealEd",              
                "LevelEditor"
            }
        );
    }
}