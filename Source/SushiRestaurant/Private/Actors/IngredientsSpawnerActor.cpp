// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/IngredientsSpawnerActor.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AIngredientsSpawnerActor::AIngredientsSpawnerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
}

// Called when the game starts or when spawned
void AIngredientsSpawnerActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && IngredientClass && TotalToSpawn > 0)
	{

		// Spawn at moment of begin play
		TrySpawnIngredient();

		// Now will spawn every spawn interval 
		GetWorld()->GetTimerManager().SetTimer(
		   SpawnTimer,
		   this,
		   &AIngredientsSpawnerActor::TrySpawnIngredient,
		   SpawnInterval,
		   true 
	   );
		
		
	}
}

void AIngredientsSpawnerActor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AIngredientsSpawnerActor,SpawnedCount);
}


void AIngredientsSpawnerActor::TrySpawnIngredient()
{
	if (!HasAuthority()) return;

	int32 CurrentCount = CheckExistingIngredients();

	if (CurrentCount < TotalToSpawn)
	{
		Server_SpawnIngredient(
			GetActorLocation() +
			(GetActorForwardVector() * FMath::FRandRange(-RandomSpawnOffset.X , RandomSpawnOffset.X)) +
			(GetActorRightVector() * FMath::FRandRange(-RandomSpawnOffset.Y , RandomSpawnOffset.Y)),
			IngredientTypeToSpawn
		);
	}
}

int32 AIngredientsSpawnerActor::CheckExistingIngredients() const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AIngredientActor::StaticClass(), FoundActors);

	int32 Count = 0;
	for (AActor* Actor : FoundActors)
	{
		AIngredientActor* Ingredient = Cast<AIngredientActor>(Actor);
		if (Ingredient && Ingredient->IngredientType == IngredientTypeToSpawn)
		{
			Count++;
		}
	}

	return Count;
}

void AIngredientsSpawnerActor::Server_SpawnIngredient_Implementation(FVector Location, EIngredientType InIngredientType)
{
	
	int32 CurrentCount = CheckExistingIngredients();
	if (CurrentCount >= TotalToSpawn) return;

	// Creating Spawn transform
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(Location);
	SpawnTransform.SetRotation(FRotator::ZeroRotator.Quaternion());

	AIngredientActor* Ingredient = Cast<AIngredientActor>(
		UGameplayStatics::BeginDeferredActorSpawnFromClass(
			this,
			IngredientClass,
			SpawnTransform
		)
	);

	if (Ingredient)
	{
		Ingredient->IngredientType = InIngredientType;
		UGameplayStatics::FinishSpawningActor(Ingredient, SpawnTransform);
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Ingredients spawned: %d"), CurrentCount));
	SpawnedCount = CheckExistingIngredients();
	
}

// Called every frame
void AIngredientsSpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

