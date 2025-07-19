// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/CookGameState.h"

#include "Actors/OrderTableActor.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ACookGameState::ACookGameState()
{
	bReplicates = true;
}

void ACookGameState::OnRep_Orders()
{
	// Used for UI
}

void ACookGameState::InitializeTablesAndOrders()
{
	Tables.Empty();
	TableOrders.Empty();

	// Busca todas as mesas no mapa
	TArray<AActor*> FoundTables;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOrderTableActor::StaticClass(), FoundTables);

	for (AActor* Actor : FoundTables)
	{
		if (AOrderTableActor* Table = Cast<AOrderTableActor>(Actor))
		{
			Tables.Add(Table);
			TableOrders.Add(FOrder()); // Create Void Order
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Number of tables is %d"),Tables.Num()));
}

void ACookGameState::SetOrderForTable(int32 TableIndex, const FOrder& NewOrder)
{
	if (TableOrders.IsValidIndex(TableIndex))
	{
		TableOrders[TableIndex] = NewOrder;
		OnRep_Orders();
	}
}

void ACookGameState::ClearOrderForTable(int32 TableIndex)
{
	if (TableOrders.IsValidIndex(TableIndex))
	{
		TableOrders[TableIndex] = FOrder(); // Reset
		OnRep_Orders();
	}
}

void ACookGameState::BeginPlay()
{
	Super::BeginPlay();

	InitializeTablesAndOrders();
	
}

void ACookGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACookGameState, TableOrders);
}

