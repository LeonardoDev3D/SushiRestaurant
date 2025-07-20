// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/IngredientActor.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AIngredientActor::AIngredientActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	IngredientMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IngredientMesh"));
}

// Called when the game starts or when spawned
void AIngredientActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIngredientActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AIngredientActor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AIngredientActor,IngredientType);
}

