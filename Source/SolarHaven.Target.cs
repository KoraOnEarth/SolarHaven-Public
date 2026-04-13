// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SolarHavenTarget : TargetRules
{
	public SolarHavenTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
		ExtraModuleNames.Add("SolarHaven");
		RegisterModulesCreatedByRider();
	}
 
	private void RegisterModulesCreatedByRider()
	{
		ExtraModuleNames.AddRange(new string[]
		{
			"SHCharacters", 
			"SHComponents", 
			"SHItems", 
			"SHCore", 
			"SHSubsystems"
		});
	}
}
