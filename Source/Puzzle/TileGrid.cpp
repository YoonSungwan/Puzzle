// Fill out your copyright notice in the Description page of Project Settings.


#include "TileGrid.h"
#include "SwapTile.h"
#include "TileCommandInvoker.h"
#include "GameStateSubject.h"

// Sets default values
ATileGrid::ATileGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	StartSwapFloat.BindUFunction(this, FName("StartLerpLocEvent"));
	EndSwapEvent.BindUFunction(this, FName("EndLerpLocEvent"));
	
	static ConstructorHelpers::FObjectFinder<UCurveFloat> GetCurve(TEXT("Script/Engine.CurveFloat'/Game/MovetileCurve.MovetileCurve'"));
	if(GetCurve.Succeeded())
	{
		MovingCurve = GetCurve.Object;
	}
	
}

// Called when the game starts or when spawned
void ATileGrid::BeginPlay()
{
	Super::BeginPlay();
	
	AllTileArray = CreateTile();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if(PlayerController)
	{
		PlayerController->SetShowMouseCursor(true);
	
		PlayerController->bEnableClickEvents = true;
		EnableInput(PlayerController);
	}

	if (MovingCurve)
	{
		Timeline->AddInterpFloat(MovingCurve, StartSwapFloat);
		Timeline->SetTimelineFinishedFunc(EndSwapEvent);

		Timeline->SetLooping(false);
		Timeline->SetTimelineLength(1.f);
	}
	
	CommandInvoker = GetWorld()->SpawnActor<ATileCommandInvoker>(ATileCommandInvoker::StaticClass()); 

	CheckEqualTile();
}

// Called every frame
void ATileGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<TArray<ATile*>> ATileGrid::CreateTile()
{
	int32 TileKindNum = TileKind.Num();

	if(TileKindNum < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Tile Setting"));
		return TArray<TArray<ATile*>>();
	}
	
	FVector TileLocation = FVector(0,0, 25);

	//새로 만들기
	for(int i=0; i<LineWidth * LineHeight; i++)
	{
		if(i !=0 && i % LineWidth == 0)
		{
			TileLocation.Z += TileHeight;
			TileLocation.Y = 0;
		}
		
		int32 RandomTile = FMath::RandRange(0, TileKindNum-1);
		ATile* SingleTile = GetWorld()->SpawnActor<ATile>(TileKind[RandomTile], TileLocation, FRotator(0, 0, 0));

		//Y 위치 조정
		SingleTile->TileShape = RandomTile;

		//타일 Swap Delegate
		SingleTile->tileDelegate.AddDynamic(this, &ATileGrid::ChangeTile);

		//Array 추가
		Tiles.Emplace(SingleTile);
		
		TileLocation.Y -= TileHeight;
	}

	return TArray<TArray<ATile*>>();
}

void ATileGrid::ChangeTile(ATile* ClickTile)
{
	if(ClickTile)
	{
		if(BeforeIndex >= 0)
		{
			CachingTile = ClickTile;
			
			ATile* beforeTile = Tiles[BeforeIndex];
			CachingBeforeTile = beforeTile;

			//CallSwapCommand();
			if(SwapTile* SwapTileCommand = new SwapTile(beforeTile, ClickTile, &StartLocation, &EndLocation))
			{
				CommandInvoker->ExecuteCommand(SwapTileCommand);
				bIsSwap = true;
				Timeline->PlayFromStart();

				//다른 클래스에서 썼을 때 예제
				/*UGameStateSubject* ObserverGameState = Cast<UGameStateSubject>(UGameStateSubject::StaticClass());
				if(ObserverGameState)
				{
					ObserverGameState->IncreaseScore(10);
				}*/
			}
		}
		else
		{
			for(int i=0; i<Tiles.Num(); i++)
			{
				if(ClickTile->GetUniqueID() == Tiles[i]->GetUniqueID())
				{
					BeforeIndex = i;
				}
			}
		}
	}
}

void ATileGrid::FillTile()
{
	for(int i=0; i<LineWidth; i++)
	{
		for(int j=0; j<LineHeight; j++)
		{
			if(!IsValid(AllTileArray[i][j]))
			{
				UE_LOG(LogTemp, Warning, TEXT("%d, %d"), i, j);
				/*FVector newTileLocate = AllTileArray[i][j]->GetActorLocation();
				
				int32 RandomTile = FMath::RandRange(0, TileKind.Num()-1);

				ATile* SingleTile = GetWorld()->SpawnActor<ATile>(TileKind[RandomTile], newTileLocate, FRotator(0, 0, 0));
				AllTileArray[i][j] = SingleTile;
				
				SingleTile->IndexLoc[0] = i;
				SingleTile->IndexLoc[1] = j;
				SingleTile->TileShape = RandomTile;

				//Debug Code
				SingleTile->StaticComp->SetWorldScale3D(FVector(0.1));*/
				
				/*for(int k = j; k < AllTileArray[i].Num(); k++)
				{
					if(IsValid(AllTileArray[i][k]))
					{
						AllTileArray[i][k];
					}
				}*/
			}
		}
	}

	/*if(GetWorldTimerManager().TimerExists(CheckDelay))
	{
		GetWorldTimerManager().ClearTimer(CheckDelay);
	}
	else
	{
		GetWorldTimerManager().SetTimer(CheckDelay, ATileGrid::CheckEqualTile(), false, 5.f);
	}*/
}

bool ATileGrid::CheckEqualTile()
{
	bool IsDestoryed = false;
	
	int32 ColType = -1;
	int32 CheckColCount = 1;

	ColType = Tiles[0]->TileShape;
	for(int i=0; i < Tiles.Num(); i++)
	{
		if(i % LineWidth == 0)
		{
			ColType = Tiles[i]->TileShape;
			CheckColCount = 1;
			continue;
		}
		
		//횟수 체크
		if(Tiles[i]->TileShape == ColType)
		{
			CheckColCount++;
		}
		else
		{
			if(CheckColCount >= 3)
			{
				DestroyTileLine(i-CheckColCount, i);
				IsDestoryed = true;
			}
			ColType = Tiles[i]->TileShape;
			CheckColCount = 1;
		}

		if(i == Tiles.Num())
		{
			if(CheckColCount >= 3)
			{
				DestroyTileLine(i-CheckColCount, i);
				IsDestoryed = true;
			}
		}
	}

	int32 RowType = -1;
	int32 CheckRowCount = 1;
	
	for(int i=0; i < LineWidth; i++)
	{
		RowType = Tiles[i]->TileShape;
		for(int j=1; j<(Tiles.Num()/LineWidth); j++)
		{
			if(RowType == Tiles[(i+(LineWidth*j))]->TileShape)
			{
				CheckRowCount++;
				//UE_LOG(LogTemp, Warning, TEXT("Index is %d, Cnt is %d"), (i+(LineWidth*j)), CheckRowCount);
			}
			else
			{
				if(CheckRowCount >= 3)
				{
					UE_LOG(LogTemp, Warning, TEXT("Reset Count is %d, %d"), i,j);
					DestroyTileLine2((i+(LineWidth*j)) - CheckRowCount, CheckRowCount);
					IsDestoryed = true;
				}
				RowType = Tiles[(i+(LineWidth*j))]->TileShape;
				CheckRowCount = 1;
			}

			if(j == (Tiles.Num()/LineWidth)-1)
			{
				if(CheckRowCount >= 3)
				{
					UE_LOG(LogTemp, Warning, TEXT("Reset Countttt is %d, %d"), i,j);
					DestroyTileLine2((i+(LineWidth*j)) - CheckRowCount, CheckRowCount);
					IsDestoryed = true;
					//UE_LOG(LogTemp, Warning, TEXT("Reset Count end is %d"), (i+(LineWidth*j)));
				}
				CheckRowCount = 1;
			}
		}
		
	}

	return IsDestoryed;
}

void ATileGrid::DestroyTileLine(int32 StartIndex, int32 EndIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("DestroyTileLine %d, %d"), StartIndex, EndIndex);
	for(int i = StartIndex; i< EndIndex; i++)
	{
		if(Tiles[i])
		{
			//Tiles[i]->IsMatch = true;
			Tiles[i]->StaticComp->SetVisibility(false);
		}
	}
}

void ATileGrid::DestroyTileLine2(int32 EndIndex, int32 RowCount)
{
	int32 StartIndex = (EndIndex-(RowCount-1)*LineWidth);
	UE_LOG(LogTemp, Warning, TEXT("In Destroy2 %d, %d"), EndIndex, RowCount);
	for(int i = 0; i < RowCount; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("DestroyTileLine %d, %d, %d"), StartIndex, i, StartIndex + i*LineWidth);
		/*if(Tiles[StartIndex + i*LineWidth])
		{
			//Tiles[StartIndex + i*LineWidth]->IsMatch = true;
			Tiles[StartIndex + i*LineWidth]->StaticComp->SetVisibility(false);
		}*/
	}
}

void ATileGrid::StartLerpLocEvent(float value)
{
	FVector ClickNewLoc = FMath::Lerp(CachingTile->GetActorLocation(), EndLocation, value);
	CachingTile->SetActorLocation(ClickNewLoc);
	
	//if(bIsSwap)
	//{
		FVector BeforeNewLoc = FMath::Lerp(CachingBeforeTile->GetActorLocation(), StartLocation, value);
		CachingBeforeTile->SetActorLocation(BeforeNewLoc);
	//}
}

void ATileGrid::EndLerpLocEvent()
{
	if(bIsSwap)
	{
		CommandInvoker->UndoLastCommand();

		bIsSwap = false;
		BeforeIndex = -1;
		
		Timeline->PlayFromStart();
		//delete SwapTileCommand;
	}
}

void ATileGrid::CallSwapCommand()
{
}

void ATileGrid::ReLocateTile(int32 Row, int32 Col)
{
	TArray<ATile*> TempTileArray;

	for(int i=0; i<LineWidth; i++)
	{
		if(Row < i)
		{
			FVector BaiscLoc = AllTileArray[i][Col]->GetActorLocation();
			FVector NewLoc = FVector(BaiscLoc.X, BaiscLoc.Y, BaiscLoc.Z-50);
			UE_LOG(LogTemp, Warning, TEXT("[%d][%d] before Loc is %s"), i, Col,*BaiscLoc.ToString());
			AllTileArray[i][Col]->SetActorLocation(NewLoc);
			AllTileArray[i][Col]->SetActorScale3D(FVector(0.3f, 0.3f, 0.3f));
			AllTileArray[i-1][Col] = AllTileArray[i][Col];
			
			UE_LOG(LogTemp, Warning, TEXT("[%d][%d] New Loc is %s"), i-1, Col,*AllTileArray[i-1][Col]->GetActorLocation().ToString());
		}
	
	}
	
	/*for(int i=0; i<AllTileArray[Row].Num(); i++)
	{
		if(Col < i)
		{
			if(AllTileArray[Row][i])
			{
				FVector BaiscLoc = AllTileArray[Row][i]->GetActorLocation();
				FVector NewLoc = FVector(BaiscLoc.X, BaiscLoc.Y, BaiscLoc.Z-50);
				
				UE_LOG(LogTemp, Warning, TEXT("Row is %d, i is %d, Col is %d"), Row, i, Col);
				//UE_LOG(LogTemp, Warning, TEXT("Basic Loc is %s"), *BaiscLoc.ToString());
				//UE_LOG(LogTemp, Warning, TEXT("New Loc is %s"), *NewLoc.ToString());
				//AllTileArray[Row][i]->SetActorLocation(NewLoc);
				//AllTileArray[Row][i-1] = AllTileArray[Row][i];

				//TempTileArray.Emplace(AllTileArray[Row][i]);
			}	
		}
		/*else
		{
			TempTileArray.Emplace(AllTileArray[Row][i]);
		}#1#
	}*/
	/*AllTileArray[Row].Empty();
	AllTileArray[Row] = TempTileArray;*/
}
