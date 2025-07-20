// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/WorkstationActor.h"

#include "Actors/FoodActor.h"
#include "Actors/IngredientActor.h"
#include "Characters/CookCharacter.h"
#include "Library/Structs/CookGameStructs.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
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

void AWorkstationActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	
}



void AWorkstationActor::OnStateChanged_Implementation(EWorkstationState newState)
{
	// Used on Blueprint
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
	DOREPLIFETIME(AWorkstationActor, FinalFoodActor);
}

void AWorkstationActor::Server_AddIngredient_Implementation(AIngredientActor* Ingredient)
{
  
	if (!Ingredient) return; // Check if ingredient is valid -> Avoid null actors

	if (CurrentState == EWorkstationState::Processing || CurrentState == EWorkstationState::Ready) return; // Check if the workstation is in use

	// If the number of ingredients on workstation is lass than 2 (Limit), new ingredients can be added
	if (CurrentIngredients.Num() < 2)
	{
		// Add ingredients
		CurrentIngredients.Add(Ingredient);

		// Verify if the workstation accepts attach ingredients or not 
		if (AttachIngredients)
		{
			Multicast_AttachIngredient(Ingredient);
		}
		else
		{
			Ingredient->Destroy();
		}
		
		SetState(EWorkstationState::Adding);
	}
}

void AWorkstationActor::Multicast_AttachIngredient_Implementation(AIngredientActor* Ingredient)
{
	// Attach ingredients on workstation, case if the workstation permits this action
	Ingredient->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Ingredient->AttachToActor(this,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	if (CurrentIngredients.Num() > 1)
	{
		Ingredient-> SetActorLocation(CurrentIngredients[CurrentIngredients.Num()-1]->GetActorLocation() + FVector(0,0,40.0f));
	}
	else
	{
		Ingredient-> SetActorLocation(GetActorLocation() + FVector(0,0,20.0f));
	}
}

void AWorkstationActor::Server_ProcessIngredient_Implementation()
{
	if (CurrentState != EWorkstationState::Adding) return;

	SetState(EWorkstationState::Processing);
	
	GetWorld()->GetTimerManager().SetTimer(ProcessingTimer, this, &AWorkstationActor::FinishProcessing, 5.0f, false);
	
}

void AWorkstationActor::Server_CollectDish_Implementation(class ACookCharacter* Player)
{
	// Verify if player can collect dish
	if (!FinalFoodActor && CurrentState != EWorkstationState::Ready) return;
	if (Player->GetHeldItem()) return;
	
	Player -> GrabItem(FinalFoodActor);
	FinalFoodActor = nullptr;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Take food!"));
	SetState(EWorkstationState::StandBy);
}

EFoodType AWorkstationActor::DetermineDishResult() const
{
	// Collect the added ingredients
	TArray<EIngredientType> Types;
	for (AIngredientActor* Ing : CurrentIngredients)
	{
		if (Ing)
			Types.Add(Ing->IngredientType);
	}

	// Puts all ingredients in a right order
	Types.Sort();

	// Find the recipes in a list
	for (const FFoodRecipe& Recipe : RecipesList)
	{
		if (Recipe.RequiredIngredients.Num() == Types.Num())
		{
			TArray<EIngredientType> SortedReq = Recipe.RequiredIngredients;
			SortedReq.Sort();

			bool bMatch = true;
			for (int32 i = 0; i < Types.Num(); i++)
			{
				if (Types[i] != SortedReq[i])
				{
					bMatch = false;
					break;
				}
			}

			if (bMatch)
				return Recipe.FoodResult;
		}
	}

	// No one recept found will cook a Slop food
	return EFoodType::Slop;
}

void AWorkstationActor::FinishProcessing()
{
	// Get the food result based on recepts 
	EFoodType FoodResult = DetermineDishResult();

	if (AttachIngredients)
	{
		for (AActor* Ingredient : CurrentIngredients)
		{
			if (IsValid(Ingredient))
			{
				Ingredient->Destroy();
			}
		}	
	}

	// Clear ingredients and set cooked food ready
	CurrentIngredients.Empty();
	
	SetState(EWorkstationState::Ready);

	Server_SpawnFood(GetActorLocation() + FVector(0,0,50),FoodResult);
	
}

void AWorkstationActor::Server_SpawnFood_Implementation(FVector Location, EFoodType InFoodType)
{
	// Make Spawn Tranforms
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(Location);
	SpawnTransform.SetRotation(FRotator::ZeroRotator.Quaternion());

	// Begin Spawn
	AFoodActor* FinalDish = Cast<AFoodActor>(
		UGameplayStatics::BeginDeferredActorSpawnFromClass(
			this,
			FoodClass,
			SpawnTransform
		)
	);

	if (FinalDish)
	{
		FinalDish->FoodType = InFoodType;
		
		FinalDish->OnFoodSpawned();
		
		UGameplayStatics::FinishSpawningActor(FinalDish, SpawnTransform);

		FinalFoodActor = FinalDish;
	}

	
}

void AWorkstationActor::OnRep_WorkstationState()
{
	
}

void AWorkstationActor::SetState(EWorkstationState NewState)
{
	CurrentState = NewState;
	OnStateChanged(CurrentState);
	OnRep_WorkstationState();
}

