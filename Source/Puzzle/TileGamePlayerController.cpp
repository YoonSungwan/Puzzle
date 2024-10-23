// Fill out your copyright notice in the Description page of Project Settings.


#include "TileGamePlayerController.h"
#include "NewTile.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SwapTileCommand.h"
#include "TileCommandInvoker.h"

ATileGamePlayerController::ATileGamePlayerController()
{
	//약한 참조로 타일을 관리
	FirstSelectedTile = nullptr;
	SecondSelectedTile = nullptr;
}

void ATileGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	//마우스 커서 보이게 하기
	bShowMouseCursor = true;

	if(APlayerController* PlayerController = Cast<APlayerController>(this))
	{
		//UEnhancedInputLocalPlayerSubSystem* SubSystem = UEnhancedInputLocalPlayerSubSystem::Get();
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if(Subsystem)
		{
			//우선순위 0으로 설정
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}
}

void ATileGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if(UEnhancedInputComponent* EnhancedInputCompo = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputCompo->BindAction(SelectTileActioin, ETriggerEvent::Started, this, &ATileGamePlayerController::SelectTile);
	}
}

void ATileGamePlayerController::SelectTile(const FInputActionValue& Value)
{
	//마우스 클릭 위치를 가져옴
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if(Hit.bBlockingHit)
	{
		ANewTile* ClickedTile = Cast<ANewTile>(Hit.GetActor());
		if(ClickedTile)
		{
			if(!FirstSelectedTile.IsValid())
			{
				FirstSelectedTile = ClickedTile;
				FirstSelectedTile->SetSelected(true);
				UE_LOG(LogTemp, Warning, TEXT("First Selected Tile Clicked %s"), *FirstSelectedTile->GetName());
			}
			else if(!SecondSelectedTile.IsValid() && ClickedTile != FirstSelectedTile)
			{
				//인접한지 체크
				if(FirstSelectedTile->IsAdjacentTo(ClickedTile))
				{
					SecondSelectedTile = ClickedTile;
					FirstSelectedTile->SetSelected(false);
					UE_LOG(LogTemp, Warning, TEXT("Second Selected Tile Clicked %s"), *SecondSelectedTile->GetName());

					//타일 선택 완료
					ProcessSelectTiles();
				}
				else
				{
					
				}
			}
		}
	}
}

void ATileGamePlayerController::ProcessSelectTiles()
{
	//두 개의 타일이 선택되었을 때
	if(!FirstSelectedTile.IsValid() || !SecondSelectedTile.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid tiles Selected"));
		return;
	}

	//타일이 인접한지 확인
	if(!FirstSelectedTile->IsAdjacentTo(SecondSelectedTile.Get()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Tiles are not adjacentTo"));
		return;
	}
	
	//타일 처리 로직(자리 교환 및 매칭 확인)

	//인터페이스를 통해서 교환명령 생성
	USwapTileCommand* SwapCommand = NewObject<USwapTileCommand>();
	SwapCommand->Initialize(FirstSelectedTile.Get(), SecondSelectedTile.Get());

	//커맨드 실행
	ATileCommandInvoker* CommnadInvoker = GetWorld()->SpawnActor<ATileCommandInvoker>();
	CommnadInvoker->ExecuteCommand(SwapCommand);

	//위에서 타일 교환 후, 삭제 됐을 수 있으므로 다시 체크
	
	//선택 된 타일을 해제
	if(FirstSelectedTile.IsValid()) FirstSelectedTile->SetSelected(false);
	if(SecondSelectedTile.IsValid()) SecondSelectedTile->SetSelected(false);
	
	//선택 초기화
	if(FirstSelectedTile.IsValid()) FirstSelectedTile = nullptr;
	if(SecondSelectedTile.IsValid()) SecondSelectedTile = nullptr;
}
