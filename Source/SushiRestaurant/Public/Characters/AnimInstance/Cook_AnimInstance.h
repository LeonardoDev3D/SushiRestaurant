// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characters/CookCharacter.h"
#include "Cook_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SUSHIRESTAURANT_API UCook_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:

	// Conditions
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Conditions")
	bool bHasMovementInput = false;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Conditions")
	bool bIsMoving = false;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Conditions")
	bool bShouldMove = false;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Conditions")
	bool IsHeldObject = false;
	
	// Get Character Speed
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Settings")
	float Speed = 0.0f;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Settings")
	float NormalizedSpeed = 0.0f;
protected:
	
	UPROPERTY()
	ACookCharacter* CharacterPtr =nullptr;

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void UpdateMovement(float DeltaSeconds);
};
