// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Command.h"
#include "TileCommandInvoker.generated.h"

UCLASS()
class PUZZLE_API ATileCommandInvoker : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<ICommand*> CommandHistory;
	
public:	
	// Sets default values for this actor's properties
	ATileCommandInvoker();

public:
	//커맨드 실행 후, CommandHistory에 저장(TArray가 아니라도 Index 개념이 있는 Container면 OK)
	void ExecuteCommand(ICommand* Command);

	//마지막 명령을 취소(Undo)
	void UndoLastCommand();
};
