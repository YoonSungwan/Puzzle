// Fill out your copyright notice in the Description page of Project Settings.


#include "GI_Puzzle.h"

UGI_Puzzle::UGI_Puzzle()
{
	PlayerScore = 0;
	RemainingMoves = 30;
}

void UGI_Puzzle::AddScore(int32 point)
{
	PlayerScore += point;
}

void UGI_Puzzle::SubtractScore(int32 point)
{
	PlayerScore -= point;
}

void UGI_Puzzle::DecreaseMoves()
{
	if(RemainingMoves > 0)
	{
		RemainingMoves--;
	}
}

void UGI_Puzzle::ResetGameState()
{
	PlayerScore = 0;
	RemainingMoves = 30;
}
