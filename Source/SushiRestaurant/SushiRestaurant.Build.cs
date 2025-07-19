// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class SushiRestaurant : ModuleRules
{
	public SushiRestaurant(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"SushiCooking",
			"SushiCooking/Variant_Platforming",
			"SushiCooking/Variant_Combat",
			"SushiCooking/Variant_Combat/AI",
			"SushiCooking/Variant_SideScrolling",
			"SushiCooking/Variant_SideScrolling/Gameplay",
			"SushiCooking/Variant_SideScrolling/AI"
		});
		
	}
}
