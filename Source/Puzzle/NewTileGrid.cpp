// Fill out your copyright notice in the Description page of Project Settings.


#include "NewTileGrid.h"
#include "NewTile.h"
#include "Async/ParallelFor.h"
#include "Async/Async.h"

// Sets default values
ANewTileGrid::ANewTileGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GridWidth = 4;
	GridHeigth = 3;

	TileArray.SetNum(GridWidth * GridHeigth);	//전체 Size
}

// Called when the game starts or when spawned
void ANewTileGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANewTileGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANewTileGrid::InitializeGrid()
{
	/*for(int32 x = 0; x < GridWidth; x++)
	{
		for(int y = 0; y < GridHeigth; y++)
		{
			ANewTile* NewTile = GetWorld()->SpawnActor<ANewTile>(ANewTile::StaticClass());
			SetTileAt(x, y, NewTile);
		}
	}*/

	//병렬로 처리를 하는 For
	/*ParallelFor(GridWidth * GridWidth, [this](int32 index)
	{
		int32 X = index % GridWidth;
		int32 Y = index / GridWidth;

		//바둑판 그리기 = 나머지와 몫으로 그리기
		//타일(ATile)을 생성하고 배열에 저장
		ANewTile* NewTile = GetWorld()->SpawnActor<ANewTile>(ANewTile::StaticClass());	//하면 안된다
		SetTileAt(X, Y, NewTile);
	});*/
	
	for(int32 x = 0; x < GridWidth; x++)
	{
		for(int y = 0; y < GridHeigth; y++)
		{
			//비동기 작업
			AsyncTask(ENamedThreads::GameThread, [this, x, y]()
			{
				ANewTile* NewTile = GetWorld()->SpawnActor<ANewTile>(ANewTile::StaticClass());
				SetTileAt(x, y, NewTile);
				UE_LOG(LogTemp, Warning, TEXT("NewTile created"));
			});
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Out"));
	
}

ANewTile* ANewTileGrid::GetTileAt(int32 x, int32 y) const
{
	if(x < 0 || x >= GridWidth || y < 0 || y >= GridHeigth)
	{
		return nullptr;
	}

	return TileArray[y * GridWidth + x];
}

void ANewTileGrid::SetTileAt(int32 x, int32 y, ANewTile* Tile)
{
	if(x >= 0 && x < GridWidth && y >= 0 && y < GridHeigth)
	{
		TileArray[y * GridWidth + x] = Tile;
	}
}

