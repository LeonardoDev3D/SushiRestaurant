// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FoodActor.h"
#include "GameFramework/Actor.h"
#include "Library/Enums/CookGameEnums.h"
#include "Library/Structs/CookGameStructs.h"
#include "WorkstationActor.generated.h"

UCLASS()
class SUSHIRESTAURANT_API AWorkstationActor : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WorkstationMesh;
	
	UPROPERTY(ReplicatedUsing=OnRep_WorkstationState, BlueprintReadOnly)
	EWorkstationState CurrentState;
	
	FTimerHandle ProcessingTimer;
	
	UPROPERTY(Replicated)
	TArray<AIngredientActor*> CurrentIngredients;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WorkstationSettings")
	TArray<FFoodRecipe> RecipesList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WorkstationSettings")
	bool AttachIngredients = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WorkstationSettings")
	TSubclassOf<AFoodActor> FoodClass ;

	UPROPERTY(Replicated)
	AFoodActor* FinalFoodActor = nullptr;
public:	
	// Sets default values for this actor's properties
	AWorkstationActor();

	UFUNCTION(BlueprintPure)
	EWorkstationState GetState() const { return CurrentState; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnStateChanged(EWorkstationState newState);
	virtual void OnStateChanged_Implementation(EWorkstationState newState);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure)
	int GetCurrentAddedIngredients()const{return CurrentIngredients.Num();};
	
	UFUNCTION(Server, Reliable)		
	void Server_AddIngredient(AIngredientActor* Ingredient);
	void Server_AddIngredient_Implementation(AIngredientActor* Ingredient);

	UFUNCTION(NetMulticast, Reliable)		
	void Multicast_AttachIngredient(AIngredientActor* Ingredient);
	void Multicast_AttachIngredient_Implementation(AIngredientActor* Ingredient);
	
	UFUNCTION(Server, Reliable)
	void Server_ProcessIngredient();
	void Server_ProcessIngredient_Implementation();

	UFUNCTION(Server, Reliable)
	void Server_CollectDish(class ACookCharacter* Player);
	void Server_CollectDish_Implementation(class ACookCharacter* Player);

	UFUNCTION(Server, Reliable)
	void Server_SpawnFood(FVector Location, EFoodType InFoodType);
	void Server_SpawnFood_Implementation(FVector Location, EFoodType InFoodType);
	
protected:

	EFoodType DetermineDishResult() const;
	
	void FinishProcessing();

	UFUNCTION()					
	void OnRep_WorkstationState();

	void SetState(EWorkstationState NewState);
};
