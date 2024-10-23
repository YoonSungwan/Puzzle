// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NewTile.generated.h"

class ANewTileGrid;

UCLASS()
class PUZZLE_API ANewTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANewTile();

	//타일이 선택되었는지 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	bool bIsSelected;

	//타일을 선택 또는 해제하는 함수
	void SetSelected(bool bSelected);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//타일이 선택되면 시각적으로 상태가 업데이트
	void Updateppearance();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//타일 간의 구분
	//단순 구분 용도로는 색상 - Vector(Gradient) 보다는 Enum이나 FName이 적합
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Properties")
	//FColor TileColor;
	FName TileType;

	//타일 매칭 확인 함수
	bool IsMatching(ANewTile* otherTile) const;

	void ProcessDataInParallel();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Properties")
	UStaticMeshComponent* TileMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Properties")
	TMap<FName, UStaticMesh*> TileMeshes;
	
	//타일의 외형을 TileType에 따라 설정하는 함수
	void UpdateTileAppearance();

	//타일의 2D(그리드) 좌표에서의 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Properties")
	FVector2D TilePosition;

	//다른 타입과 인접 여부를 확인하는 함수
	bool IsAdjacentTo(ANewTile* otherTile) const;

	//타일의 위치를 변경할 때, 그리드 상의 위치 업데이트 함수
	void UpdateTilePosition(const FVector2D& NewPosition);

	ANewTileGrid* TileGrid;
};
