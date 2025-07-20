// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/CookPlayerState.h"

#include "Net/UnrealNetwork.h"

ACookPlayerState::ACookPlayerState()
{
}

void ACookPlayerState::AddScore(int32 Points)
{
	ServerAddScore(Points);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Score %d"),PlayerScore));
}

void ACookPlayerState::OnUpdateScore_Implementation()
{
	
}

void ACookPlayerState::OnRep_PlayerScore()
{
	OnUpdateScore(); 
}

void ACookPlayerState::ServerAddScore_Implementation(int32 Points)
{
	PlayerScore += Points;
	OnUpdateScore();
}

void ACookPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACookPlayerState, PlayerScore);
}

void ACookPlayerState::BeginPlay()
{
	Super::BeginPlay();

	OnUpdateScore();
}
