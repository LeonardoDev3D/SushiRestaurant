// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/WorkstationActor.h"

#include "Actors/IngredientActor.h"
#include "Library/Structs/CookGameStructs.h"
#include "Components/SceneComponent.h"
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
		if (AttachIngredients)
		{
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
		else
		{
			Ingredient->Destroy();
		}
		
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

EFoodType AWorkstationActor::DetermineDishResult() const
{
	// Collect the added ingredients
	TArray<EIngredientType> Types;
	for (AIngredientActor* Ing : CurrentIngredients)
	{
		if (Ing)
			Types.Add(Ing->IngredientType);
	}

	
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

	// No one recept found => Slop
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
	
	CurrentIngredients.Empty();
	
	SetState(EWorkstationState::Ready);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Finished cook!"));

	// Debug food name
	FString FoodResultName = StaticEnum<EFoodType>()->GetValueAsString(FoodResult);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Food is %s!"), *FoodResultName));
}

void AWorkstationActor::OnRep_WorkstationState()
{
	
}

void AWorkstationActor::SetState(EWorkstationState NewState)
{
	CurrentState = NewState;
	OnRep_WorkstationState();
}

