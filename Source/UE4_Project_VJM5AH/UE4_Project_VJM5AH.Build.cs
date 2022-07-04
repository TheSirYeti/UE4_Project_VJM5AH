// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE4_Project_VJM5AH : ModuleRules
{
	public UE4_Project_VJM5AH(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "AIModule" });
	}
}
