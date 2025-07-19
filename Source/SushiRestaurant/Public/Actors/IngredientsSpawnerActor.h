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
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	FTimerHandle SpawnTimer;

	UPROPERTY(Replicated)
	int32 SpawnedCount = 0;

	int32 CheckExistingIngredients() const;
	
	UFUNCTION(server, reliable)
	void Server_SpawnIngredient(FVector Location, EIngredientType InIngredientType );
    void Server_SpawnIngredient_Implementation(FVector Location, EIngredientType InIngredientType );
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
