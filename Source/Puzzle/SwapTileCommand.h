// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Command.h"

/**
 * 
 */
class ANewTile;
class PUZZLE_API SwapTileCommand : public ICommand
{
public:
	SwapTileCommand();
	~SwapTileCommand();

	SwapTileCommand(ANewTile* InFirstTile, ANewTile* SecondTile);

private:
	ANewTile* FirstTile;
	ANewTile* SecondTile;

	FName FirstTileType;
	FName SecondTileType;

public:
	//ICommand Interface 함수들 Implement
	virtual void Execute() override;
	virtual void Undo() override;
	
};
