// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/IngredientActor.h"
#include "Library/Enums/CookGameEnums.h"
#include "IngredientsSpawnerActor.generated.h"

UCLASS()
class SUSHIRESTAURANT_API AIngredientsSpawnerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIngredientsSpawnerActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawner")
	TSubclassOf<AIngredientActor> IngredientClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawner")
	float SpawnInterval = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawner")
	EIngredientType IngredientTypeToSpawn = EIngredientType::Avocado;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawner")
	int32 TotalToSpawn = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawner")
	FVector2D RandomSpawnOffset = FVector2D::ZeroVector;

	UPROPERTY(Replicated)
	int32 SpawnedCount = 0;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	FTimerHandle SpawnTimer;

	
	void TrySpawnIngredient();

	
	int32 CheckExistingIngredients() const;

	UFUNCTION(Server, Reliable)
	void Server_SpawnIngredient(FVector Location, EIngredientType InIngredientType);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
