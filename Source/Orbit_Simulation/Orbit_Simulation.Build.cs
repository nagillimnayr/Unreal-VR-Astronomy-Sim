// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Orbit_Simulation : ModuleRules
{
	public Orbit_Simulation(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"EnhancedInput", "UI_Module", "CommonUI"
		});
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"UMG",  
			"Niagara",
			"ProceduralMeshComponent",
			"Json", 
			"JsonUtilities"
		});


		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
