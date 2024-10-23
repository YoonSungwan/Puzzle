// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NewTileGrid.generated.h"

class ANewTile;

UCLASS()
class PUZZLE_API ANewTileGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANewTileGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//그리드 width, height

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Grid")
	int32 GridWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Grid")
	int32 GridHeigth;

	//타일 간 배치 간격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Grid")
	float TileSpacing;

	UPROPERTY()
	TArray<ANewTile*> TileArray;

	//타일을 생성한 ANewTile 레퍼런스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Grid")
	TSubclassOf<ANewTile> TileClass;

	//그리드 초기화 함수
	void InitializeGrid();

	//특정 위치의 타일을 얻는 함수
	ANewTile* GetTileAt(int32 x, int32 y) const;

	//특정 위치의 타일을 설정하는 함수
	void SetTileAt(int32 x, int32 y, ANewTile* Tile);

	//매칭을 확인 여부 Array
	TArray<ANewTile*> CheckForMatches();

	//Swap 함수
	void SwapTiles(ANewTile* FirstTile, ANewTile* SecondTile);

	void RemoveMatchingTiles(const TArray<ANewTile*>& MatchingTiles);
	void DropDownTiles();

	bool GetTileGridPosition(ANewTile* Tile, int32& OutX, int32& OutY) const;

	void RefillGrid();

	void ProcessMatchingLoop();

private:
	
	TArray<ANewTile*> CheckHorizontalMatches(int32 StartX, int32 StartY);
	TArray<ANewTile*> CheckVerticalMatches(int32 StartX, int32 StartY);

	FName GenerateRandomTileType();
};
