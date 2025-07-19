// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/WorkstationActor.h"

#include "Actors/IngredientActor.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWorkstationActor::AWorkstationActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	WorkstationMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WorkstationMesh"));
}

// Called when the game starts or when spawned
void AWorkstationActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWorkstationActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWorkstationActor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AWorkstationActor::ServerProcessIngredient_Implementation(AIngredientActor* Ingredient)
{
	
	if (Ingredient)
	{
		if (IngredientsList.Contains(Ingredient->IngredientType))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Cook!"));
		}
		
	}
}

