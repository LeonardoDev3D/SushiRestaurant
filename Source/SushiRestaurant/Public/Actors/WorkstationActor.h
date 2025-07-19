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
	
	UPROPERTY(ReplicatedUsing=OnRep_WorkstationState, BlueprintReadOnly)
	EWorkstationState CurrentState;
	
	FTimerHandle ProcessingTimer;
	
	UPROPERTY(Replicated)
	TArray<AIngredientActor*> CurrentIngredients;
	
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

	UFUNCTION(BlueprintPure)
	int GetCurrentAddedIngredients()const{return CurrentIngredients.Num();};
	
	UFUNCTION(Server, Reliable)		
	void Server_AddIngredient(AIngredientActor* Ingredient);
	void Server_AddIngredient_Implementation(AIngredientActor* Ingredient);
	
	UFUNCTION(Server, Reliable)
	void Server_ProcessIngredient();
	void Server_ProcessIngredient_Implementation();

	UFUNCTION(Server, Reliable)
	void Server_CollectDish(class ACookCharacter* Player);
	void Server_CollectDish_Implementation(class ACookCharacter* Player);

protected:

	void FinishProcessing();

	UFUNCTION()					
	void OnRep_WorkstationState();

	void SetState(EWorkstationState NewState);
};
