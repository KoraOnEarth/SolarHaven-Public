// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SolarHaven : ModuleRules
{
	public SolarHaven(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"SHCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"SHCharacters", 
			"SHItems",
			"SHComponents", 
			"SHSubsystems"
		});
	}
}
