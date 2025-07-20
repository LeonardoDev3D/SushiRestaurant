// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Library/Enums/CookGameEnums.h"
#include "Actors/OrderTableActor.h"
#include "Library/Structs/CookGameStructs.h"
#include "CookGameState.generated.h"

/**
 * 
 */
UCLASS()
class SUSHIRESTAURANT_API ACookGameState : public AGameState
{
	GENERATED_BODY()
	
public:

	ACookGameState();
	
	UPROPERTY(ReplicatedUsing=OnRep_Orders, BlueprintReadOnly, Category="Orders")
	TArray<FOrder> TableOrders;

	UPROPERTY(BlueprintReadOnly, Category="Orders")
	TArray<AOrderTableActor*> Tables;
	
	UFUNCTION()
	void OnRep_Orders();
	
	UFUNCTION(BlueprintCallable, Category="Orders")
	void InitializeTablesAndOrders();
	
	/** Defines a order for a table */
	UFUNCTION(BlueprintCallable, Category="Orders")
	void SetOrderForTable(int32 TableIndex, const FOrder& NewOrder);

	/** Clear table's order */
	UFUNCTION(BlueprintCallable, Category="Orders")
	void ClearOrderForTable(int32 TableIndex);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Cooking")
	void OnOrderUpdated();
	virtual void OnOrderUpdated_Implementation();
	
protected:

	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;	
};
