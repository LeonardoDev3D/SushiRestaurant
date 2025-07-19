// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/TrashActor.h"

// Sets default values
ATrashActor::ATrashActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	TrashMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrashMesh"));
	
}

// Called when the game starts or when spawned
void ATrashActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrashActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

