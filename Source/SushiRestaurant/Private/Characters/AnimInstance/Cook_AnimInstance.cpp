// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AnimInstance/Cook_AnimInstance.h"

void UCook_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (TryGetPawnOwner())
	{
		CharacterPtr = Cast<ACookCharacter>(TryGetPawnOwner());
	}
}

void UCook_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (DeltaSeconds == 0.0f)
	{
		// Prevent update on the first frame (potential division by zero)
		return;
	}
	
	if (!CharacterPtr)
	{
		return;
	}

	/******************************************
		Do Every Frame
	******************************************/
	UpdateMovement(DeltaSeconds);
}

void UCook_AnimInstance::UpdateMovement(float DeltaSeconds)
{
	Speed = CharacterPtr -> GetVelocity().Size2D();
	bIsMoving = Speed > 0.0f;
}
