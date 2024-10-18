// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NewTile.generated.h"

class ANewTile;

UCLASS()
class PUZZLE_API ANewTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANewTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
};
