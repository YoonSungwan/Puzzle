// Fill out your copyright notice in the Description page of Project Settings.


#include "NewTile.h"

#include "HeadMountedDisplayTypes.h"
#include "RHIValidationTransientResourceAllocator.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ANewTile::ANewTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Static mesh component 생성
	TileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = TileMeshComponent;

	TileType = FName("Default");
	bIsSelected = false;

	//그리드 상의 초기화 위치
	TilePosition = FVector2D::ZeroVector;
}

// Called when the game starts or when spawned
void ANewTile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANewTile::SetSelected(bool bSelected)
{
	this->bIsSelected = bSelected;
	Updateppearance();
}

void ANewTile::Updateppearance()
{
	//선택 되었을 때, 시각적 피드백 -> Material Emissive
	if(bIsSelected)
	{
		if(UStaticMeshComponent* StaticMeshComponent = TileMeshComponent)
		{
			//선택된 타일을 강조
			//TileMeshComponent->SetRenderCustomDepth(true);

			//Emissive 제어를 통한 하이라이트
			TileMeshComponent->SetScalarParameterValueOnMaterials(TEXT("EmissiveStrength"), 10.f);
		}
	}
	else
	{
		if(UStaticMeshComponent* StaticMeshComponent = TileMeshComponent)
		{
			//선택된 타일을 강조 꺼짐
			//TileMeshComponent->SetRenderCustomDepth(false);
			
			TileMeshComponent->SetScalarParameterValueOnMaterials(TEXT("EmissiveStrength"), 0.f);
		}
	}
}

// Called every frame
void ANewTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ANewTile::IsMatching(ANewTile* otherTile) const
{
	return TileType == otherTile->TileType;
}

void ANewTile::ProcessDataInParallel()
{
	TArray<int32> DataArray;
	DataArray.Init(0, 100);

	ParallelFor(DataArray.Num(), [&](int32 index)
	{
		DataArray[index] = index*2;

		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Tile %d - %d"), index, DataArray[index]));
	});

	UE_LOG(LogTemp, Warning, TEXT("ParallelForFinish"));
}

void ANewTile::UpdateTileAppearance()
{
	//TileMeshes Map에 TileType에 해당하는 키값이 존재하는지
	if(TileMeshes.Contains(TileType))
	{
		TileMeshComponent->SetStaticMesh(TileMeshes[TileType]);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UpdateTileAppearance TileMesh does not exist"));
	}
}

bool ANewTile::IsAdjacentTo(ANewTile* otherTile) const
{
	if(!otherTile)
	{
		return false;
	}

	//두 타일의 그리드 좌표 차이를 계산하여 인접 여부 확인
	int32 DeltaX = FMath::Abs(TilePosition.X - otherTile->TilePosition.X);
	int32 DeltaY = FMath::Abs(TilePosition.Y - otherTile->TilePosition.Y);

	//두 타일이 가로 또는 세로 1칸 범위 내일 경우
	//대각선 이동도 막아야하므로 둘 중 하나만 이동(둘 중 하나만 1)했을 때만 true
	return (DeltaX + DeltaY) == 1;

	
	/*
	//인접 여부를 확인 (가로, 세로 한칸 범위 내인지)
	//절대값 -> 음수를 양수화시킨다는 개념 X -> 방향을 없앤다
	bool _IsHolizontal = (FMath::Abs(TilePosition.X - otherTile->TilePosition.X) == 1 && TilePosition.Y == otherTile->TilePosition.Y);
	bool _IsVertical = (FMath::Abs(TilePosition.Y - otherTile->TilePosition.Y) == 1 && TilePosition.X == otherTile->TilePosition.X);


	UE_LOG(LogTemp, Warning, TEXT("Adjacent Check: %d, %d"), _IsHolizontal, _IsVertical);
	
	return (_IsHolizontal || _IsVertical);
	*/
}

//Getter, Setter처럼 안전하게
void ANewTile::UpdateTilePosition(const FVector2D& NewPosition)
{
	TilePosition = NewPosition;
}


