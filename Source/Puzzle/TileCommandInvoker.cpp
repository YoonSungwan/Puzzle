// Fill out your copyright notice in the Description page of Project Settings.


#include "TileCommandInvoker.h"

// Sets default values
ATileCommandInvoker::ATileCommandInvoker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

void ATileCommandInvoker::ExecuteCommand(ICommand* Command)
{
	//캡슐화 <- Casting 등의 다른 작업 X
	Command->Execute();
	CommandHistory.Push(Command);
}

void ATileCommandInvoker::UndoLastCommand()
{
	//되돌아갈 History가 있어야 한다
	if (!CommandHistory.IsEmpty() && CommandHistory.Num() > 0)
	{
		ICommand* LastCommand = CommandHistory.Last();
		LastCommand->Undo();

		//게임마다 다름
		//CommandHistory.RemoveAll();
		//CommandHistory.RemoveAt();
		CommandHistory.Pop();
	}
}
