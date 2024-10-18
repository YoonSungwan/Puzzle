// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>("Capsule Component");
	SetRootComponent(CapsuleComp);

	StaticComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh Component");
	StaticComp->SetupAttachment(CapsuleComp);
	
	OnClicked.AddDynamic(this, &ATile::ClickEvent);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::ClickEvent(AActor* Target, FKey ButtonPressed)
{
	ATile* clickActor = Cast<ATile>(Target);
	if(clickActor)
	{
		clickActor->IsClicked = true;
		tileDelegate.Broadcast(clickActor);
	}
}
