// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GI_Puzzle.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLE_API UGI_Puzzle : public UGameInstance
{
	GENERATED_BODY()
	//Singleton Class
	
private: 
	UGI_Puzzle();

public:
	//게임 전체에 필요한 변수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game Data")
	int32 PlayerScore;	//플레이어 점수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game Data")
	int32 RemainingMoves;	//남은 매칭(이동) 횟수

	//점수 관리할 함수

	UFUNCTION(BlueprintCallable, Category="Game Function")
	void AddScore(int32 point);		//점수 증가

	UFUNCTION(BlueprintCallable, Category="Game Function")
	void SubtractScore(int32 point);		//점수 감소

	UFUNCTION(BlueprintCallable, Category="Game Function")
	void DecreaseMoves();		//남은 매칭(이동) 횟수 감소


	//게임 상태 초기화(레벨 시작 혹은 Retry 시, 호출)
	UFUNCTION(BlueprintCallable, Category="Game Function")
	void ResetGameState();
	
};
