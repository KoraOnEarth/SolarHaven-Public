// Fill out your copyright notice in the Description page of Project Settings.

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
			"UMG", 
			"SHCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"EnhancedInput",
			"SHUI",
			"SHComponents", 
			"SHSubsystems"
		});
	}
}
