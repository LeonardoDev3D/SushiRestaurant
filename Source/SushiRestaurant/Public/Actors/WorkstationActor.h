// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Library/Enums/CookGameEnums.h"
#include "WorkstationActor.generated.h"

UCLASS()
class SUSHIRESTAURANT_API AWorkstationActor : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WorkstationMesh;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TArray<EIngredientType> IngredientsList;
	
public:	
	// Sets default values for this actor's properties
	AWorkstationActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void ServerProcessIngredient(AIngredientActor* Ingredient);
};
