// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DA_TileData.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate, ATile*, singleTile);
UCLASS()
class PUZZLE_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	FDelegate tileDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Tile")
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditAnywhere, Category = "Tile")
	class UStaticMeshComponent* StaticComp;

	UDA_TileData* TileDataAsset;

	UPROPERTY(VisibleAnywhere, Category = "TileData")
	TArray<UDA_TileData*> AllTileData;

	UPROPERTY(EditAnywhere, Category = "TileType")
	int32 TileShape = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileData")
	TArray<int32> IndexLoc = {-1, -1};

	UPROPERTY(VisibleAnywhere)
	bool IsClicked = false;

	UPROPERTY(VisibleAnywhere)
	bool IsMatch = false;

	UFUNCTION()
	void ClickEvent(AActor* Target, FKey ButtonPressed);

	//void NotifyActorOnClicked(FKey ButtonPressed) override;
};
