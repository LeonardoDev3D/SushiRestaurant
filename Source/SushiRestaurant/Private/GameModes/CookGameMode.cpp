// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/CookGameMode.h"
#include "Library/Enums/CookGameEnums.h"
#include "GameState/CookGameState.h"

ACookGameMode::ACookGameMode()
{
	
}

void ACookGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Create a order after game begins
	GetWorld()->GetTimerManager().SetTimer(OrderSpawnTimer, this, &ACookGameMode::SpawnOrder, OrderInterval, true);

}

void ACookGameMode::SpawnOrder()
{
	ACookGameState* GS = GetGameState<ACookGameState>();
	if (!GS || GS->Tables.Num() == 0) return;

	int32 TableIndex = FMath::RandRange(0, GS->Tables.Num() - 1);

	// Can be extendable later to do more one orders by table
	FOrder NewOrder;
	int32 RandType1 = FMath::RandRange(0, (int32)EFoodType::SeaUrchinRoll - 1);
	NewOrder.RequiredFoods = { (EFoodType)RandType1};
	
	GS->SetOrderForTable(TableIndex, NewOrder);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("New order created for the table %d"),TableIndex));

	// --- DEBUG FOOD ---
	FString IngredientList;
	for (EFoodType Ingredient : NewOrder.RequiredFoods)
	{
		FString Name = StaticEnum<EIngredientType>()->GetValueAsString(Ingredient);
		IngredientList += Name + TEXT(" ");
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,FString::Printf(TEXT("Table %d Did Order: %s"), TableIndex, *IngredientList));
}
