// Fill out your copyright notice in the Description page of Project Settings.


#include "SwapTileCommand.h"
#include "NewTile.h"

SwapTileCommand::SwapTileCommand()
{
}

SwapTileCommand::~SwapTileCommand()
{
}

SwapTileCommand::SwapTileCommand(ANewTile* InFirstTile, ANewTile* SecondTile)
{
	FirstTile = InFirstTile;
	SecondTile = SecondTile;
	
	FirstTileType = InFirstTile->TileType;
	SecondTileType = SecondTile->TileType;
}

void SwapTileCommand::Execute()
{
	//SWAP : FVector 위치(Location), 행렬(Row, Columns) 로 해도 무방
	FName Temp = FirstTile->TileType;
	FirstTile->TileType = SecondTile->TileType;
	SecondTile->TileType = Temp;
}

void SwapTileCommand::Undo()
{
	//커맨드 패턴의 저장하는 로직
	FirstTile->TileType = FirstTileType;
	SecondTile->TileType = SecondTileType;
}

