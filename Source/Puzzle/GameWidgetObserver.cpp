// Fill out your copyright notice in the Description page of Project Settings.


#include "GameWidgetObserver.h"

void UGameWidgetObserver::OnNotify_Implementation(int32 UpdateScore)
{
	//IObserver::OnNotify_Implementation(UpdateScore);

	//주체(Subject)로부터 새로운 점수를 받아서 Current Score를 Update
	CurrentScore = UpdateScore;

	UpdateScoreUI();
}
