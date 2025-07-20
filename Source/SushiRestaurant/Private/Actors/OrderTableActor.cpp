// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/OrderTableActor.h"

#include "Components/BoxComponent.h"
#include "GameState/CookGameState.h"
#include "PlayerState/CookPlayerState.h"

// Sets default values
AOrderTableActor::AOrderTableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	OrderTableCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("OrderTableCollision"));
}

void AOrderTableActor::Server_DeliverOrder_Implementation(AFoodActor* Food,APlayerController* InstigatorPC)
{
	if (!Food) return;

	// Get player controller and Gamestate references
	ACookPlayerState* PS =InstigatorPC->GetPlayerState<ACookPlayerState>();
	ACookGameState* GS = GetWorld()->GetGameState<ACookGameState>();

	if (PS && GS && GS->TableOrders.IsValidIndex(TableIndex))
	{
		// Get current order from Game state
		const FOrder& CurrentOrder = GS->TableOrders[TableIndex];

		if (CurrentOrder.RequiredFoods.Contains(Food->FoodType))
		{
			// Add points whe delivers correct food on the correct table
			
			PS->AddScore(100);
			GS->ClearOrderForTable(TableIndex);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf( TEXT("Correct food! Order done %d!"), TableIndex));
		}
		else
		{
			GS->ClearOrderForTable(TableIndex);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf( TEXT("Incorrect food %d!"), TableIndex));
		}
	}
}

// Called when the game starts or when spawned
void AOrderTableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOrderTableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

