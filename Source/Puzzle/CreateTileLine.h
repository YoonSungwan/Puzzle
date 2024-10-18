// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Tile.h"
#include "CreateTileLine.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUZZLE_API UCreateTileLine : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCreateTileLine();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	int32 LineWidth = 6;
	int32 LineHeight = 6;

	float TileSize = 50.f;

public:
	//TArray<TArray<ATile*>> CreateTile(ATile* SingleTile);
	TArray<TArray<ATile*>> CreateTile(TArray<TSubclassOf<ATile>> TileKind);
};
