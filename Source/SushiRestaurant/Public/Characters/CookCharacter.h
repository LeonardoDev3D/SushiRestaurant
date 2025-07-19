// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/IngredientActor.h"
#include "GameFramework/Character.h"
#include "CookCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

UCLASS()
class SUSHIRESTAURANT_API ACookCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

protected:

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* MouseLookAction;

	/** Interact like cook Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* InteractAction;

	/** Interact like cook Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* DiscardAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interation")
	TEnumAsByte<ECollisionChannel> InteractionTraceChannel = ECC_Visibility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interation")
	FName HeldSocketName;
public:
	
public:
	// Sets default values for this character's properties
	ACookCharacter();

	void GrabItem(AActor* InItem);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Interact();

	void DiscardIngredients();

	/** Current held object */
	UPROPERTY(Replicated)
	AActor* HeldItem;
	
	UFUNCTION(Server, Reliable)
	void Server_TraceInteract(FVector InStart, FVector InEnd, FVector Extent, FRotator Rotation);
	void Server_TraceInteract_Implementation(FVector InStart, FVector InEnd, FVector Extent, FRotator Rotation);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_TraceInteract(FVector InStart, FVector InEnd, FVector Extent, FRotator Rotation);
	void Multicast_TraceInteract_Implementation(FVector InStart, FVector InEnd, FVector Extent, FRotator Rotation);
	
	UFUNCTION(Server, Reliable)
	void Server_DropItem(AActor* InItem);
	void Server_DropItem_Implementation(AActor* InItem);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DropItem(AActor* InItem);
	void Multicast_DropItem_Implementation(AActor* InItem);
	
	void DropItem(AActor* InItem);
public:
	
	UFUNCTION(BlueprintPure)
	AActor * GetHeldItem()const{return HeldItem;}
	
	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	
public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	
};
