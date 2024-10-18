// Fill out your copyright notice in the Description page of Project Settings.


#include "CreateTileLine.h"

// Sets default values for this component's properties
UCreateTileLine::UCreateTileLine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCreateTileLine::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCreateTileLine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<TArray<ATile*>> UCreateTileLine::CreateTile(TArray<TSubclassOf<ATile>> TileKind)
{
	int32 TileKindNum = TileKind.Num();

	if(TileKindNum < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Tile Setting"));
		return TArray<TArray<ATile*>>();
	}

	TArray<TArray<ATile*>> AllTileArray;
	AllTileArray.SetNum(LineWidth);
	FVector TileLocation = FVector(0,0, 25);

	for(int i=0; i<LineWidth; i++)
	{
		AllTileArray[i].SetNum(LineHeight);
		
		for(int j=0; j<LineHeight; j++)
		{
			int32 RandomTile = FMath::RandRange(0, TileKindNum-1);

			ATile* SingleTile = GetWorld()->SpawnActor<ATile>(TileKind[RandomTile], TileLocation, FRotator(0, 0, 0));
			AllTileArray[i][j] = SingleTile;
			
			TileLocation.Y += TileSize;
			
			SingleTile->IndexLoc[0] = i;
			SingleTile->IndexLoc[1] = j;
			SingleTile->TileShape = RandomTile;
		}
		
		TileLocation.Y = 0;
		TileLocation.Z += TileSize;
	}

	return AllTileArray;
}


/*TArray<TArray<ATile*>> UCreateTileLine::CreateTile(ATile* SingleTile)
{
	TArray<TArray<ATile*>> TileArray;
	FVector TileLocation = FVector::ZeroVector;
	
	for(int i=0; i<LineWidth; i++)
	{
		for(int j=0; j<LineHeight; j++)
		{
			//ATile SpawnTile = GetWorld()->SpawnActor<ATile>(ATile::StaticClass(), TileLocation, FRotator::ZeroRotator);
			SingleTile->SetActorLocation(TileLocation);
			TileLocation.Y += TileSize;

			TileArray[i][j] = SingleTile;
		}

		TileLocation.Y = 0;
		TileLocation.Z += TileSize;
	}

	return TileArray;
}*/
