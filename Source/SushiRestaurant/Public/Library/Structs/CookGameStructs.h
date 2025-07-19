// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CookGameStructs.generated.h"

USTRUCT(BlueprintType)
struct FOrder
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EFoodType> RequiredFoods;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeRemaining = 30.f; // limit time to do the order
};


USTRUCT(BlueprintType)
struct FFoodRecipe
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EIngredientType> RequiredIngredients;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFoodType FoodResult;
	

};