// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CookGameEnums.generated.h"
/**
 * 
 */
UENUM(BlueprintType)
enum class EIngredientType : uint8
{
	Avocado,
	Crabsticks,
	Cucumber,
	Ebi,
	Eel,
	Fish,
	FishFillet,
	Flounder,
	Mackerel,
	Nori,
	Octopus,
	Rice,
	Salmon,
	SeaUrchin,
	MAX
};

UENUM(BlueprintType)
enum class EFoodType : uint8
{
	Dango,
	Wasabi,
	SalmonNigiri,
	EbiNigiri,
	OctopusNigiri,
	Ramen,
	SeaUrchinRoll,
	Slop
};

UENUM(BlueprintType)
enum class EWorkstationState : uint8
{
	StandBy,     // No ingredients
	Adding,      // Receiving ingredients
	Processing,  // Making the food
	Ready        // Dish ready
};