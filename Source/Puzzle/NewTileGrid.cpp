// Fill out your copyright notice in the Description page of Project Settings.


#include "NewTileGrid.h"

#include "AsyncTreeDifferences.h"
#include "NewTile.h"
#include "Async/ParallelFor.h"
#include "Async/Async.h"

// Sets default values
ANewTileGrid::ANewTileGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GridWidth = 6;
	GridHeigth = 7;
	TileSpacing = 100.f;

	TileArray.SetNum(GridWidth * GridHeigth);	//전체 Size
}

// Called when the game starts or when spawned
void ANewTileGrid::BeginPlay()
{
	Super::BeginPlay();

	InitializeGrid();
}

// Called every frame
void ANewTileGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANewTileGrid::InitializeGrid()
{
	/*for(int32 x = 0; x < GridWidth; x++)
	{
		for(int y = 0; y < GridHeigth; y++)
		{
			ANewTile* NewTile = GetWorld()->SpawnActor<ANewTile>(ANewTile::StaticClass());
			SetTileAt(x, y, NewTile);
		}
	}*/

	//병렬로 처리를 하는 For
	/*ParallelFor(GridWidth * GridWidth, [this](int32 index)
	{
		int32 X = index % GridWidth;
		int32 Y = index / GridWidth;

		//바둑판 그리기 = 나머지와 몫으로 그리기
		//타일(ATile)을 생성하고 배열에 저장
		ANewTile* NewTile = GetWorld()->SpawnActor<ANewTile>(ANewTile::StaticClass());	//하면 안된다
		SetTileAt(X, Y, NewTile);
	});*/

	// 가능한 TileType 리스트
	TArray<FName> TileTypes =
		{ FName(TEXT("Cone")), FName(TEXT("Cube")),FName(TEXT("Cylinder")),
		FName(TEXT("Sphere")),FName(TEXT("Capsule")),FName(TEXT("Pyramid"))};

	//그냥 for문으로 생성
	/*for(int32 x = 0; x < GridWidth; x++)
	{
		for(int32 y = 0; y < GridHeigth; y++)
		{
			//NewTile 레퍼런스 Class가 설정되지 않은 경우
			if(!TileClass)
			{
				UE_LOG(LogTemp, Error, TEXT("TileClass is not valid"));
				return;
			}
				
			//타일 생성(GameThread *** 실행)
			FActorSpawnParameters SpawnParameters;
			ANewTile* NewTile = GetWorld()->SpawnActor<ANewTile>(TileClass, SpawnParameters);
			
			if(NewTile)
			{
				//위치도 변경
				NewTile->TilePosition = FVector2D(x, y);

				FName RandomTileType = TileTypes[FMath::RandRange(0, TileTypes.Num() - 1)];
				
				//생성된 타일의 타일 설정
				NewTile->TileType = RandomTileType;

				//타일 종류에 따라서 외형 업데이트
				NewTile->UpdateTileAppearance();

				//타일을 타일그리드에 자식으로 부착
				NewTile->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

				//타일을 그리드에 배치
				FVector TileLocation = FVector(x * TileSpacing, y * TileSpacing, 0.f);
				NewTile->SetActorRelativeLocation(TileLocation);

				SetTileAt(x,y,NewTile);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to spawn tile"));
			}
		}
	}*/

	//비동기 for문으로 생성
	for(int x=0; x< GridWidth; ++x)
	{
		for(int y = 0; y < GridHeigth; ++y)
		{
			//백그라운드에서 타일 타입을 결정하는 작업
			AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this, x, y, TileTypes]()
			{
				//타일 타입을 랜덤하게 결정 (AsyncTask로 비동기)
				FName RandomTileType = TileTypes[FMath::RandRange(0, TileTypes.Num() - 1)];
				
				AsyncTask(ENamedThreads::GameThread, [this, x, y, RandomTileType]()
				{
					//NewTile 레퍼런스 Class가 설정되지 않은 경우
					if(!TileClass)
					{
						UE_LOG(LogTemp, Error, TEXT("TileClass is not valid"));
						return;
					}
					
					//타일 생성(GameThread *** 실행)
					FActorSpawnParameters SpawnParameters;
					ANewTile* NewTile = GetWorld()->SpawnActor<ANewTile>(TileClass, SpawnParameters);

					if(NewTile)
					{
						NewTile->TileGrid = this;
						
						NewTile->TilePosition = FVector2D(x, y);
						
						//생성된 타일의 타일 설정
						NewTile->TileType = RandomTileType;

						//타일 종류에 따라서 외형 업데이트
						NewTile->UpdateTileAppearance();

						//타일을 타일그리드에 자식으로 부착
						NewTile->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

						//타일을 그리드에 배치
						FVector TileLocation = FVector(x * TileSpacing, y * TileSpacing, 0.f);
						NewTile->SetActorRelativeLocation(TileLocation);

						SetTileAt(x,y,NewTile);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("Failed to spawn tile"));
					}
				});
			});
		}
	};
}

ANewTile* ANewTileGrid::GetTileAt(int32 x, int32 y) const
{
	if(x < 0 || x >= GridWidth || y < 0 || y >= GridHeigth)
	{
		return nullptr;
	}

	return TileArray[y * GridWidth + x];
}

void ANewTileGrid::SetTileAt(int32 x, int32 y, ANewTile* Tile)
{
	if(x >= 0 && x < GridWidth && y >= 0 && y < GridHeigth)
	{
		TileArray[y * GridWidth + x] = Tile;
	}
}

TArray<ANewTile*> ANewTileGrid::CheckForMatches()
{
	//매칭된 타일들 생성
	TArray<ANewTile*> AllMatchedTiles;

	//Grid를 for문으로 바툭판 확인
	for(int32 x=0; x<GridWidth; ++x)
	{
		for(int32 y=0; y<GridHeigth; ++y)
		{
			ANewTile* CurrentTile = GetTileAt(x, y);
			if(!CurrentTile) continue;

			//매칭된 타일 찾기
			TArray<ANewTile*> HorizontalMathches = CheckHorizontalMatches(x, y);

			//매칭 기준 만족 숫자
			if(HorizontalMathches.Num() >= 3)
			{
				AllMatchedTiles.Append(HorizontalMathches);
			}
		}	
	}

	//Grid를 for문으로 바툭판 확인
	for(int32 x=0; x<GridWidth; ++x)
	{
		for(int32 y=0; y<GridHeigth; ++y)
		{
			ANewTile* CurrentTile = GetTileAt(x, y);
			if(!CurrentTile) continue;

			//매칭된 타일 찾기
			TArray<ANewTile*> VerticalMathches = CheckVerticalMatches(x, y);

			//매칭 기준 만족 숫자
			if(VerticalMathches.Num() >= 3)
			{
				AllMatchedTiles.Append(VerticalMathches);
			}
		}	
	}
	
	return AllMatchedTiles;
}

void ANewTileGrid::SwapTiles(ANewTile* FirstTile, ANewTile* SecondTile)
{
	int x1, y1, x2, y2;
	UE_LOG(LogTemp, Warning, TEXT("Swapppp"));
	if(GetTileGridPosition(FirstTile, x1, y1) && GetTileGridPosition(SecondTile, x2, y2))
	{
		//타일 배열 업데이트
		SetTileAt(x1, y1, SecondTile);
		SetTileAt(x2, y2, FirstTile);

		//타일의 그리드 좌표 업데이트
		FVector2D TempPosition = SecondTile->TilePosition;
		FirstTile->TilePosition = SecondTile->TilePosition;
		SecondTile->TilePosition = TempPosition;

		//타일의 위치를 스왑
		FVector TempLocation = FirstTile->GetActorLocation();
		FirstTile->SetActorLocation(SecondTile->GetActorLocation());
		SecondTile->SetActorLocation(TempLocation);
	}
}

void ANewTileGrid::RemoveMatchingTiles(const TArray<ANewTile*>& MatchingTiles)
{
	for(ANewTile* Tile : MatchingTiles)
	{
		//삭제전 
		if(Tile)
		{
			int32 X, Y;
			if(GetTileGridPosition(Tile, X, Y))
			{
				SetTileAt(X, Y, nullptr);
				
				//타일을 삭제하기 전에 애니메이션을 적용 가능
				//ex)Tile->PlayDeleteAnimation();
				
				Tile->Destroy();
			}
		}
	}
	DropDownTiles();

	//TODO: 점수 확인

	//이후 매칭 루프 처리
	ProcessMatchingLoop();
}

void ANewTileGrid::DropDownTiles()
{
	for(int32 x=0; x<GridWidth; ++x)
	{
		for(int32 y=GridHeigth - 1; y >= 0; --y)	//아래에서 위로 확인
		{
			if(!GetTileAt(x, y))	//빈 칸일 경우
			{
				for(int32 AboveY = y-1; AboveY >= 0; --AboveY)
				{
					ANewTile* AboveTile = GetTileAt(x, AboveY);
					if(AboveTile)
					{
						//타일 배열 업데이트
						SetTileAt(x, y, AboveTile);
						SetTileAt(x, AboveY, nullptr);

						//타일의 그리드 좌표갱신
						AboveTile->TilePosition = FVector2D(x,y);

						//타일을 이동(상대적 위치로) = 초기화
						FVector RelativeLocation = FVector(x*TileSpacing, y*TileSpacing, 0.f);
						AboveTile->SetActorRelativeLocation(RelativeLocation);

						break;
					}
				}
			}
		}
	}

	RefillGrid();
}

TArray<ANewTile*> ANewTileGrid::CheckHorizontalMatches(int32 StartX, int32 StartY)
{
	TArray<ANewTile*> HorizontalMatches;
	ANewTile* StartTile = GetTileAt(StartX, StartY);

	if(!StartTile)
	{
		return HorizontalMatches;
	}

	HorizontalMatches.Add(StartTile);
	UE_LOG(LogTemp, Warning, TEXT("StartY is %d"), StartY);
	//오른쪽으로 2칸까지 같은 타일인지 확인
	for(int32 x = (StartX + 1); x < GridWidth; ++x)
	{
		ANewTile* NewTile = GetTileAt(x, StartY);
		
		if(NewTile && NewTile->TileType == StartTile->TileType)
		{
			HorizontalMatches.Add(NewTile);
		}
		else
		{
			break;
		}
	}
	
	return HorizontalMatches;
}

TArray<ANewTile*> ANewTileGrid::CheckVerticalMatches(int32 StartX, int32 StartY)
{
	TArray<ANewTile*> VerticalMatches;
	ANewTile* StartTile = GetTileAt(StartX, StartY);

	if(!StartTile)
	{
		return VerticalMatches;
	}

	VerticalMatches.Add(StartTile);

	//오른쪽으로 2칸까지 같은 타일인지 확인
	for(int32 y = StartX + 1; y < GridHeigth; ++y)
	{
		ANewTile* NewTile = GetTileAt(StartX, y);
		if(NewTile && NewTile->TileType == StartTile->TileType)
		{
			VerticalMatches.Add(NewTile);
		}
		else
		{
			break;
		}
	}
	
	return VerticalMatches;
}

bool ANewTileGrid::GetTileGridPosition(ANewTile* Tile, int32& OutX, int32& OutY) const
{
	for(int i=0; i<TileArray.Num(); ++i)
	{
		if(TileArray[i] == Tile)
		{
			OutX = i % GridWidth;		//X 좌표 계산
			OutY  = i / GridHeigth;		//Y 좌표 계산
			return true;
		}
	}
	return true;
}

void ANewTileGrid::RefillGrid()
{
	for(int x=0; x<GridWidth; ++x)
	{
		for(int y=0; y<GridHeigth; ++y)
		{
			//해당 x,y 좌표 위치 그리드에 타일이 없다면 새로운 타일 생성
			if(!GetTileAt(x, y))
			{
				ANewTile* NewTile = GetWorld()->SpawnActor<ANewTile>(TileClass);
				if(NewTile)
				{
					NewTile->TileGrid = this;
						
					NewTile->TilePosition = FVector2D(x, y);
						
					//생성된 타일의 타일 설정, 이전에는 비동기였으므로
					NewTile->TileType = GenerateRandomTileType();

					//타일 종류에 따라서 외형 업데이트
					NewTile->UpdateTileAppearance();

					//타일을 타일그리드에 자식으로 부착
					NewTile->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

					//타일을 그리드에 배치
					FVector TileLocation = FVector(x * TileSpacing, y * TileSpacing, 0.f);
					NewTile->SetActorRelativeLocation(TileLocation);

					SetTileAt(x,y,NewTile);
				}
			}
		}
	}

	//또 다른 매칭이 있는지 확인
	ProcessMatchingLoop();
}

void ANewTileGrid::ProcessMatchingLoop()
{
	//매칭이 있는지 확인
	TArray<ANewTile*> MatchingTiles = CheckForMatches();

	if(MatchingTiles.Num() > 0)
	{
		//매칭된 타일이 있을 경우 삭제
		RemoveMatchingTiles(MatchingTiles);

		//타일을 빈 공간으로 이동
		DropDownTiles();

		//빈 공간에 새로운 타일 채우기
		RefillGrid();

		//모든 작업이 끝난 후 다시 매칭 확인을 위한 재귀 호출
		ProcessMatchingLoop();
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No more matching Tiles"));
	}
}

FName ANewTileGrid::GenerateRandomTileType()
{
	TArray<FName> TileTypes =
		{ FName(TEXT("Cone")), FName(TEXT("Cube")),FName(TEXT("Cylinder")),
		FName(TEXT("Sphere")),FName(TEXT("Capsule")),FName(TEXT("Pyramid"))};
	
	return TileTypes[FMath::RandRange(0, TileTypes.Num() - 1)];
}

