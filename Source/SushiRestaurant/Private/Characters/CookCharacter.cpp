// Fill out your copyright notice in the Description page of Project Settings.



#include "SushiRestaurant/Public/Characters/CookCharacter.h"

#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Actors/IngredientActor.h"
#include "Actors/OrderTableActor.h"
#include "Actors/TrashActor.h"
#include "Actors/WorkstationActor.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ACookCharacter::ACookCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(70.f, 180.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->MaxWalkSpeed = 1000.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	
}

// Called when the game starts or when spawned
void ACookCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACookCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void ACookCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ACookCharacter::Interact()
{
	FVector Start = GetActorLocation();
	FVector End = Start + (GetActorForwardVector() * 65.0f);

	// Box Size
	FVector BoxHalfSize = FVector(130.f, GetCapsuleComponent()->GetScaledCapsuleRadius() * 0.75f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight());

	// Box rotation based on character rotation
	FRotator Orientation = GetActorRotation();
	Server_TraceInteract(Start,End,BoxHalfSize,Orientation);
	
}

void ACookCharacter::DiscardIngredients()
{
	// If is a food, don't drop
	AFoodActor* FoodActor = Cast<AFoodActor>(HeldItem);
	if (!FoodActor)
	{
		Server_DropItem(HeldItem);
	}
	
}


void ACookCharacter::Server_TraceInteract_Implementation(FVector InStart, FVector InEnd, FVector Extent, FRotator Rotation)
{
	Multicast_TraceInteract(InStart,InEnd,Extent,Rotation);
}

void ACookCharacter::Multicast_TraceInteract_Implementation(FVector InStart, FVector InEnd, FVector Extent,
	FRotator Rotation)
{
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	// Box trace
	if (GetWorld()->SweepSingleByChannel(
			Hit,
			InStart,
			InEnd,
			Rotation.Quaternion(),
			InteractionTraceChannel,
			FCollisionShape::MakeBox(Extent),
			Params))
	{
		// Store actors
		AWorkstationActor* Workstation = Cast<AWorkstationActor>(Hit.GetActor());
		ATrashActor* Trash = Cast<ATrashActor>(Hit.GetActor());
		AOrderTableActor* Table = Cast<AOrderTableActor>(Hit.GetActor());
		
		if (!HeldItem)
		{
			// Grab ingredient
			if (AIngredientActor* Ingredient = Cast<AIngredientActor>(Hit.GetActor()))
			{
				GrabItem(Ingredient);
			}
			if (Workstation)
			{
				// Collect read food if the same is done
				Workstation-> Server_CollectDish(this);
			}
			
		}
		else
		{
			
			if (Workstation)
			{
				// Add ingredients on workstation
				if (AIngredientActor*Ingredient = Cast<AIngredientActor>(HeldItem))
				{
					if (Workstation->GetCurrentAddedIngredients() < 2)
					{
						Workstation-> Server_AddIngredient(Ingredient);
						HeldItem = nullptr;
					}
					
				}
				
			}

			if (Trash)
			{
				// throw in the trash
				if (HeldItem)
				{
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Trash!"));
					HeldItem -> Destroy();
					HeldItem = nullptr;
				}
			
			}

			if (Table)
			{
				// Delivery to table
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Interact with table!"));
				AFoodActor* HeldFood = Cast<AFoodActor>(HeldItem);
				Server_DeliveryToTable(Table,HeldFood);
			}
			
		}

		if (Workstation)
		{
			// When the player puts the right ingredients, begin cook
			if (Workstation->GetCurrentAddedIngredients() == 2)
			{
				Workstation-> Server_ProcessIngredient();
			}
		}
		// Draw debug box
		
	}

	//DrawDebugBox(GetWorld(),InEnd,Extent,Rotation.Quaternion(),FColor::Red,false,2.0f);
		
}

void ACookCharacter::GrabItem(AActor* InItem)
{
	// Grab ingredient
	if (HeldItem)return;
	
	if (AActor* HeldActor = InItem)
	{
		HeldActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, HeldSocketName);
		HeldActor-> SetActorEnableCollision(false);
		HeldItem = HeldActor;
	}
}

void ACookCharacter::Server_DropItem_Implementation(AActor* InItem)
{
	Multicast_DropItem(InItem);
}

void ACookCharacter::Multicast_DropItem_Implementation(AActor* InItem)
{
	DropItem(InItem);
}

void ACookCharacter::DropItem(AActor* InItem)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Drop!"));
	if (InItem)
	{
		InItem -> DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		InItem-> SetActorEnableCollision(true);
		HeldItem = nullptr;
	}
}

void ACookCharacter::Server_DeliveryToTable_Implementation(AOrderTableActor* Table, AFoodActor* Food)
{
	if (Table && Food)
	{
		Table->Server_DeliverOrder(Food);
		HeldItem = nullptr;
	}
}

void ACookCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ACookCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

// Called every frame
void ACookCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACookCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACookCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ACookCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACookCharacter::Look);

		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ACookCharacter::Interact);

		// Discard Ingredients
		EnhancedInputComponent->BindAction(DiscardAction, ETriggerEvent::Started, this, &ACookCharacter::DiscardIngredients);
	}
	
}

void ACookCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACookCharacter, HeldItem);
}

