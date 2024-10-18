// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Command.h"

/**
 * 
 */
class ATile;
class PUZZLE_API SwapTile : public ICommand 
{
public:
	SwapTile();
	~SwapTile();

	SwapTile(ATile* InBeforeTile, ATile* InClickTile, FVector* CachingStartLoc, FVector* CachingEndLoc);

private:
	ATile* BeforeTile;
	ATile* ClickTile;

	FVector BeforeTileLoc;
	FVector ClickTileLoc;

	FVector* CachingStartLoc;
	FVector* CachingEndLoc;

public:
	virtual void Execute() override;
	virtual void Undo() override;
};
