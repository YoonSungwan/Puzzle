// Fill out your copyright notice in the Description page of Project Settings.


#include "SwapTile.h"

#include "Tile.h"

SwapTile::SwapTile()
{
}

SwapTile::~SwapTile()
{
}

SwapTile::SwapTile(ATile* InBeforeTile, ATile* InClickTile, FVector* InCachingStartLoc, FVector* InCachingEndLoc)
{
	BeforeTile = InBeforeTile;
	ClickTile = InClickTile;

	BeforeTileLoc = InBeforeTile->GetActorLocation();
	ClickTileLoc = InClickTile->GetActorLocation();

	CachingStartLoc = InCachingStartLoc;
	CachingEndLoc = InCachingEndLoc;
}

void SwapTile::Execute()
{
	
	*CachingStartLoc = ClickTile->GetActorLocation();
	*CachingEndLoc = BeforeTile->GetActorLocation();
}

void SwapTile::Undo()
{
	*CachingStartLoc = BeforeTileLoc;
	*CachingEndLoc = ClickTileLoc;
}
