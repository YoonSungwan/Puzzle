// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CreateTileLine.h"
#include "TileGrid.generated.h"

class ATile;
class ATileCommandInvoker;
UCLASS()
class PUZZLE_API ATileGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TileGrid Kind")
	TArray<TSubclassOf<ATile>> TileKind;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TileGrid Size")
	int32 LineWidth = 3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TileGrid Size")
	int32 LineHeight = 4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TileGrid Size")
	float TileHeight = 50.f;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TileGrid")
	//class UCreateTileLine* CreateTileLineComponent;

	int32 RowClicked = -1;
	int32 ColClicked = -1;

	UPROPERTY()
	TArray<int32> BeforeClickedIndex = {-1, -1};

	TArray<TArray<ATile*>> AllTileArray;
	FTimerHandle CheckDelay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileGrid")
	TArray<ATile*> Tiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileGrid")
	int32 BeforeIndex = -1;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TArray<TArray<ATile*>> CreateTile();

	UFUNCTION()
	void ChangeTile(ATile* ClickTile);
	void FillTile();
	
	bool CheckEqualTile();
	
	void DestroyTileLine(int32 StartIndex, int32 EndIndex);
	void DestroyTileLine2(int32 EndIndex, int32 RowCount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	class UTimelineComponent* Timeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	class UCurveFloat* MovingCurve;
	
	FOnTimelineFloat StartSwapFloat;
	FOnTimelineEvent EndSwapEvent;

	UFUNCTION()
	void StartLerpLocEvent(float value);
	UFUNCTION()
	void EndLerpLocEvent();

	UPROPERTY()
	ATile* CachingBeforeTile;
	UPROPERTY()
	ATile* CachingTile;

	UPROPERTY()
	FVector StartLocation;
	UPROPERTY()
	FVector EndLocation;
	UPROPERTY()
	bool bIsSwap = false;
	
	UPROPERTY()
	ATileCommandInvoker* CommandInvoker;

	void CallSwapCommand();

	void ReLocateTile(int32 Row, int32 Col);
};
