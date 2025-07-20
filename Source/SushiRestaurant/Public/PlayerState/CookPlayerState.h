// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CookPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SUSHIRESTAURANT_API ACookPlayerState : public APlayerState
{
	GENERATED_BODY()
public:

	ACookPlayerState();
	
	UPROPERTY(ReplicatedUsing=OnRep_PlayerScore, BlueprintReadOnly, Category="Score")
	int32 PlayerScore = 0;
	
	UFUNCTION(BlueprintCallable, Category="Score")
	void AddScore(int32 Points);

	UFUNCTION(BlueprintNativeEvent)
	void OnUpdateScore();
	virtual void OnUpdateScore_Implementation();

	UFUNCTION()
	void OnRep_PlayerScore();

	UFUNCTION(Server, Reliable)
	void ServerAddScore(int32 Points);
	void ServerAddScore_Implementation(int32 Points);
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;
};
