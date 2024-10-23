// Fill out your copyright notice in the Description page of Project Settings.


#include "Match3GameMode.h"

#include "GameStateSubject.h"
#include "GI_Puzzle.h"
#include "NewTileGrid.h"
#include "Kismet/GameplayStatics.h"

void AMatch3GameMode::BeginPlay()
{
	Super::BeginPlay();

	//UGI_Puzzle* MyGameInstance = Cast<UGI_Puzzle>(GetGameInstance());
	/*UGI_Puzzle* MyGameInstance = Cast<UGI_Puzzle>(UGameplayStatics::GetGameInstance(GetWorld()));

	if(MyGameInstance)	//사실 매우매우 필요없음
	{
		MyGameInstance->ResetGameState();
	}

	//타일 그리드 초기화 <- 만약 타일그리드가 월드에 존재 시, 생성은 불필요
	ANewTileGrid* TileGrid = GetWorld()->SpawnActor<ANewTileGrid>(ANewTileGrid::StaticClass());
	if(TileGrid)
	{
		TileGrid->InitializeGrid();
	}*/

	//Observer 주체(subject) 생성
	/*UGameStateSubject* ObserverGameState = NewObject<UGameStateSubject>();
	
	if(widget)
	{
		UGameWidgetObserver* ScoreWidget = CreateWidget<UGameWidgetObserver>(GetWorld(), widget);

		if(ScoreWidget)
		{
			ScoreWidget->AddToViewport();

			//위젯을 Observer로 등록
			ObserverGameState->RegisterObserver(ScoreWidget);

			//다른 클래스에서 썼을 때 예제
			/*
			 * UGameStateSubject* ObserverGameState = Cast<UGameStateSubject>(...);
			 * ObserverGameState->IncreaseScore()
			 #1#
		}
	}*/
	
}
