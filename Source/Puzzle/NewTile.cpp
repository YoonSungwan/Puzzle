// Fill out your copyright notice in the Description page of Project Settings.


#include "NewTile.h"

// Sets default values
ANewTile::ANewTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANewTile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANewTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ANewTile::IsMatching(ANewTile* otherTile) const
{
	return TileType == otherTile->TileType;
}

void ANewTile::ProcessDataInParallel()
{
	TArray<int32> DataArray;
	DataArray.Init(0, 100);

	ParallelFor(DataArray.Num(), [&](int32 index)
	{
		DataArray[index] = index*2;

		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Tile %d - %d"), index, DataArray[index]));
	});

	UE_LOG(LogTemp, Warning, TEXT("ParallelForFinish"));
}

