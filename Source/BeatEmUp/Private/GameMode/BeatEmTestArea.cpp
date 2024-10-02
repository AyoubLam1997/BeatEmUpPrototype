// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BeatEmTestArea.h"

#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"

void ABeatEmTestArea::BeginPlay()
{
	//UWorld::GetFirstLocalPlayerFromController();

	FVector pos1 = FVector(0, -200, 0);
	FVector pos2 = FVector(0, 200, 0);
	FRotator rot1 = FRotator(0, 0, 0);
	FActorSpawnParameters SpawnInfo = FActorSpawnParameters();

	Fighter = GetWorld()->SpawnActor<ABaseFighter>(FighterToSpawn, pos1, rot1, SpawnInfo);
	//// DEV NOTE: This isn't the way to set the camera to the player. Update in the future
	GetWorld()->GetPlayerControllerIterator()->Get(0)->Possess(Fighter);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(Fighter != nullptr));

	Fighter->InitializeController();
	
	Fighter2 = GetWorld()->SpawnActor<ABaseFighter>(FighterToSpawn2, pos2, rot1, SpawnInfo);
	//////GetWorld()->GetPlayerControllerIterator()->Get(1)->Possess(m_P2Fighter);
	////m_P2Fighter->InitializeController();

	FVector cameraPos = FVector(-750, 0, 150);

	FTransform transform(FRotator{ -20.f, 0.f, 0.f }, cameraPos, FVector{ 1.f, 1.f, 1.f });
	UWorld* poWorld = GetWorld();
	Camera = poWorld->SpawnActor<ACameraActor>();
	Camera->SetActorTransform(transform);

	//// PERSONAL NOTE: I HATE HOW UNREAL HANDLES CAMERAS. THIS DUMB & STUPID & DUMB
	APlayerController* playerController = Cast<APlayerController>(Fighter->Controller);

	playerController->SetViewTarget(Camera);

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	Super::BeginPlay();
}
