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
	DOREPLIFETIME(AWorkstationActor, CurrentIngredients);
	DOREPLIFETIME(AWorkstationActor, CurrentState);
}

void AWorkstationActor::Server_AddIngredient_Implementation(AIngredientActor* Ingredient)
{
  
	if (!Ingredient) return; // Check if ingredient is valid -> Avoid null actors

	if (CurrentState == EWorkstationState::Processing || CurrentState == EWorkstationState::Ready) return; // Check if the workstation is in use
	
	if (!IngredientsList.Contains(Ingredient->IngredientType)) return; // Check if this ingredient can be used on this workstation
	
	
	if (CurrentIngredients.Num() < 2)
	{
		CurrentIngredients.Add(Ingredient);
		Ingredient->Destroy();
		SetState(EWorkstationState::Adding);
	}
}

void AWorkstationActor::Server_ProcessIngredient_Implementation()
{
	if (CurrentState != EWorkstationState::Adding) return;

	SetState(EWorkstationState::Processing);
	GetWorld()->GetTimerManager().SetTimer(ProcessingTimer, this, &AWorkstationActor::FinishProcessing, 3.0f, false);
	
}

void AWorkstationActor::Server_CollectDish_Implementation(class ACookCharacter* Player)
{
	
}

void AWorkstationActor::FinishProcessing()
{
	CurrentIngredients.Empty();
	SetState(EWorkstationState::Ready);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Finished cook!"));
}

void AWorkstationActor::OnRep_WorkstationState()
{
	
}

void AWorkstationActor::SetState(EWorkstationState NewState)
{
	CurrentState = NewState;
	OnRep_WorkstationState();
}

