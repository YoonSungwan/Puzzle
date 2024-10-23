// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NewTile.h"
#include "NewTileGrid.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "TileGamePlayerController.generated.h"

/**
 * 
 */
class UInputMappingContext;
class UInputAction;
UCLASS()
class PUZZLE_API ATileGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATileGamePlayerController();

protected:
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

	//첫 번째와 두 번째로 선택된 타일을 약한 참조로 저장(GC 대응)
	TWeakObjectPtr<ANewTile> FirstSelectedTile;
	TWeakObjectPtr<ANewTile> SecondSelectedTile;

	//TileGrid
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	ANewTileGrid* TileGrid;

	//Input 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SelectTileActioin;

	//타일을 선택하는 함수
	void SelectTile(const FInputActionValue& Value);

	//두 개의 타일을 선택하고 처리하는 함수
	void ProcessSelectTiles();
};
