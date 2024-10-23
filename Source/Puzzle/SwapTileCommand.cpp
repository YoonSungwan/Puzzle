// Fill out your copyright notice in the Description page of Project Settings.


#include "SwapTileCommand.h"
#include "NewTile.h"
#include "NewTileGrid.h"

void USwapTileCommand::Initialize(ANewTile* InFirstTile, ANewTile* InSecondTile)
{
	FirstTile = InFirstTile;
	SecondTile = InSecondTile;

	//원래 위치 저장
	FirstTileOriginalLocation = InFirstTile->GetActorLocation();
	SecondTileOriginalLocation = InSecondTile->GetActorLocation();
}

void USwapTileCommand::Execute()
{
	//Valid Check
	if(!FirstTile || !SecondTile)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Tiles for swap"));
		return;
	}

	//타일 그리드 가져오기
	ANewTileGrid* TileGrid = FirstTile->TileGrid;

	//그리드 Valid
	if(!TileGrid)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid TileGrid"));
		return;
	}

	//타일 Swap
	TileGrid->SwapTiles(FirstTile, SecondTile);

	//타일 Swap 후 매칭 확인
	TArray<ANewTile*> MatcingTiles = TileGrid->CheckForMatches();
	UE_LOG(LogTemp, Warning, TEXT("Execute??? %d"), MatcingTiles.Num());
	if(MatcingTiles.Num() > 0)
	{
		//매칭된 타일 삭제
		TileGrid->RemoveMatchingTiles(MatcingTiles);
	}
	else
	{
		//매칭이 없을 경우, 타일 원래대로
		//Undo();
	}
	
	/*
	//두 타일의 위치를 서로 교환
	FirstTile->SetActorLocation(SecondTileOriginalLocation);
	SecondTile->SetActorLocation(FirstTileOriginalLocation);

	//매칭 확인 등의 로직 필요
	
	//그리드 좌표도 교체
	FVector2D TempPosition = FirstTile->TilePosition;
	FirstTile->UpdateTilePosition(SecondTile->TilePosition);
	SecondTile->UpdateTilePosition(TempPosition);
	*/
}

void USwapTileCommand::Undo()
{
	//타일의 위치를 원래대로 되돌림
	FirstTile->SetActorLocation(FirstTileOriginalLocation);
	SecondTile->SetActorLocation(SecondTileOriginalLocation);

	//그리드 좌표 원래대로
	FVector2D TempPosition = FirstTile->TilePosition;
	FirstTile->UpdateTilePosition(SecondTile->TilePosition);
	SecondTile->UpdateTilePosition(TempPosition);

	//그리드 배열 업데이트
	ANewTileGrid* TileGrid = FirstTile->TileGrid;
	if(TileGrid)
	{
		int x1, x2, y1, y2;
		if(TileGrid->GetTileGridPosition(FirstTile, x1, y1) && TileGrid->GetTileGridPosition(SecondTile, x2, y2))
		{
			//타일 배열 업데이트
			TileGrid->SetTileAt(x1, y1, SecondTile);
			TileGrid->SetTileAt(x2, y2, FirstTile);

			/*//타일의 그리드 좌표 업데이트
			FVector2D Temp = SecondTile->TilePosition;
			FirstTile->TilePosition = SecondTile->TilePosition;
			SecondTile->TilePosition = Temp;

			//타일의 위치를 스왑
			FVector TempLocation = FirstTile->GetActorLocation();
			FirstTile->SetActorLocation(SecondTile->GetActorLocation());
			SecondTile->SetActorLocation(TempLocation);*/
		}
	}
}

