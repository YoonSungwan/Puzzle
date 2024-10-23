// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Command.h"
#include "SwapTileCommand.generated.h"

/**
 * 
 */
class ANewTile;

UCLASS()
class PUZZLE_API USwapTileCommand : public UObject, public ICommand
{
	GENERATED_BODY()

private:
	ANewTile* FirstTile;
	ANewTile* SecondTile;

	//교환 전 타일의 위치
	FVector FirstTileOriginalLocation;
	FVector SecondTileOriginalLocation;

public:
	//생서자 내용 대체
	void Initialize(ANewTile* InFirstTile, ANewTile* InSecondTile);
	
	//ICommand Interface 함수들 Implement
	virtual void Execute() override;
	virtual void Undo() override;
	
};
