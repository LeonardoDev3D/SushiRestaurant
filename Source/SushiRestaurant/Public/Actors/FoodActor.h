// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Library/Enums/CookGameEnums.h"
#include "FoodActor.generated.h"

UCLASS()
class SUSHIRESTAURANT_API AFoodActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFoodActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FoodActor")
	EFoodType FoodType;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Mesh)
	UStaticMeshComponent* FoodMesh;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Food")
	void OnFoodSpawned();
	virtual void OnFoodSpawned_Implementation();
};
