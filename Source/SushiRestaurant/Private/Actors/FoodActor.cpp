// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/FoodActor.h"

// Sets default values
AFoodActor::AFoodActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
}

// Called when the game starts or when spawned
void AFoodActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFoodActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFoodActor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
}

