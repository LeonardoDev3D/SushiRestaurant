// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/CookGameMode.h"

#include "GameFramework/PlayerStart.h"
#include "Library/Enums/CookGameEnums.h"
#include "GameState/CookGameState.h"
#include "Kismet/GameplayStatics.h"

ACookGameMode::ACookGameMode()
{
	
}

#if WITH_EDITOR
void ACookGameMode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// Limits first delay on order interval to prevent the first order spawn after the next orders
	if (FirstOrderSpawnDelay > OrderInterval)
	{
		FirstOrderSpawnDelay = OrderInterval;
	}
}
#endif

void ACookGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Create a first order
	GetWorld()->GetTimerManager().SetTimer(FirstOrderSpawnTimer, this, &ACookGameMode::SpawnOrder, FirstOrderSpawnDelay, false);
	
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

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("New order created for the table %d"),TableIndex));

	// --- DEBUG FOOD ---

	/*
	FString IngredientList;
	for (EFoodType Ingredient : NewOrder.RequiredFoods)
	{
		FString Name = StaticEnum<EIngredientType>()->GetValueAsString(Ingredient);
		IngredientList += Name + TEXT(" ");
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,FString::Printf(TEXT("Table %d Did Order: %s"), TableIndex, *IngredientList));
	*/

}

AActor* ACookGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	

	// Store detected player starts
	if (PlayerStartPoints.Num() == 0)
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStartPoints);
	}

	// Get Player Index
	int32 PlayerIndex = GameState.Get() ? GameState->PlayerArray.IndexOfByKey(Player->PlayerState) : 0;
	if (PlayerStartPoints.IsValidIndex(PlayerIndex))
	{
		return PlayerStartPoints[PlayerIndex];
	}

	// Get default location when the player start is not detected
	return Super::ChoosePlayerStart_Implementation(Player);
}
