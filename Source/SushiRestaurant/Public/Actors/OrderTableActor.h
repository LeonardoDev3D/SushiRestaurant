// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FoodActor.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "OrderTableActor.generated.h"

UCLASS()
class SUSHIRESTAURANT_API AOrderTableActor : public AActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* OrderTableCollision;
	
	// Sets default values for this actor's properties
	AOrderTableActor();
	
	UFUNCTION(Server, Reliable)
	void Server_DeliverOrder(AFoodActor* Food);
	void Server_DeliverOrder_Implementation(AFoodActor* Food);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
