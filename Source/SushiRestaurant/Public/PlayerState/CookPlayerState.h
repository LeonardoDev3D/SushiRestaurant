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
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Score")
	int32 PlayerScore = 0;
	
	UFUNCTION(BlueprintCallable, Category="Score")
	void AddScore(int32 Points);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
