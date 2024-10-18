// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_TileData.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ETileShape : uint8
{
	Circle,
	Triangle,
	Square
};
UCLASS()
class PUZZLE_API UDA_TileData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	ETileShape TileShape;
	
	UPROPERTY(EditAnywhere)
	TArray<FString> Tiles = {TEXT("Circle"), TEXT("Triangle"), TEXT("Square")};
	
	UPROPERTY(EditAnywhere)
	FColor TileColor;

	UPROPERTY(EditAnywhere)
	bool bIsMatch;
};
