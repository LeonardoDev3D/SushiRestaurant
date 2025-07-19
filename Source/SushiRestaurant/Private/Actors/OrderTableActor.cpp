// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/OrderTableActor.h"

#include "Components/BoxComponent.h"

// Sets default values
AOrderTableActor::AOrderTableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	OrderTableCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("OrderTableCollision"));
}

void AOrderTableActor::Server_DeliverOrder_Implementation(AFoodActor* Food)
{
	
}

// Called when the game starts or when spawned
void AOrderTableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOrderTableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

