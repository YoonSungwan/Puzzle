// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Observer.h"
#include "GameWidgetObserver.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLE_API UGameWidgetObserver : public UUserWidget, public IObserver
{
	GENERATED_BODY()

private:
	
	//현재 점수 변수
	int32 CurrentScore;
	
public:
	
	//주체(Subject)로부터 상태변화를 수신할 때 호출
	virtual void OnNotify_Implementation(int32 UpdateScore) override;

	// BlueprintImplementableEvent = 블루프린트에서 실구현 가능
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateScoreUI();
};
