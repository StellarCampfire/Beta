// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Beta : ModuleRules
{
	public Beta(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "UMG" });

		PublicIncludePaths.AddRange(new string[] {
            "Beta",
            "Beta/TemperatureEngine"
        });
    }
}
