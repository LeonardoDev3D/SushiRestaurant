// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CookGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SUSHIRESTAURANT_API ACookGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	ACookGameMode();

	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	/** Create new order */
	UFUNCTION(BlueprintCallable, Category="Orders")
	void SpawnOrder();

protected:

	FTimerHandle FirstOrderSpawnTimer;
	
	FTimerHandle OrderSpawnTimer;
	
	UPROPERTY(EditDefaultsOnly, Category="Orders")
	float OrderInterval = 15.0f;

	UPROPERTY(EditDefaultsOnly, Category="Orders")
	float FirstOrderSpawnDelay = 15.0f;
	
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
	UPROPERTY()
	TArray<AActor*> PlayerStartPoints;
	
};
