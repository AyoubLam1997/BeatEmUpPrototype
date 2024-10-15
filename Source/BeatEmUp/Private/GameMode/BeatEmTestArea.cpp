// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BeatEmTestArea.h"

#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include <Kismet/GameplayStatics.h>

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
	Fighter2->InitializeAIController();

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

void ABeatEmTestArea::Tick(float DeltaTime)
{
	if(UGameplayStatics::GetGlobalTimeDilation(GetWorld()) < 1.f)
	{
		Fighter->CustomTimeDilation = 1.f;
		Fighter2->CustomTimeDilation = 1.f;
	}

	Super::Tick(DeltaTime);

	double dist = FVector::Dist(Fighter->GetActorLocation(), Fighter2->GetActorLocation());

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(dist));

	if (!Fighter2->IsDead())
	{
		if (dist >= 100)
		{
			FVector dir = Fighter->GetActorLocation() - Fighter2->GetActorLocation();

			dir.Normalize();

			GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::SanitizeFloat(Fighter->ReturnMoveInput().X));
			GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::SanitizeFloat(Fighter->ReturnMoveInput().Y));
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(Fighter->ReturnCapsuleMesh()->GetPhysicsLinearVelocity().Y));
			GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::SanitizeFloat(dir.X));
			GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::SanitizeFloat(dir.Y));

			Fighter2->SetMoveDirection(dir);
		}
		else
		{
			Fighter2->SetMoveDirection(FVector::Zero());

			for (int i = 0; i < Fighter2->ReturnInputBuffer()->m_InputBufferItems.Num(); i++)
			{
				if (Fighter2->ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer.Num() > 0)
				{
					if (Fighter2->ReturnInputBuffer()->m_InputBufferItems[i]->InputDirection == EInputType::LightPunch)
					{
						if (Fighter2->ReturnInputBuffer()->m_InputBufferItems[i]->m_InputActionPressed == 0)
						{
							Fighter2->ReturnInputBuffer()->m_InputBufferItems[i]->SetInputActionPressed(1);
						}
						else if (Fighter2->ReturnInputBuffer()->m_InputBufferItems[i]->m_InputActionPressed == 1)
						{
							Fighter2->ReturnInputBuffer()->m_InputBufferItems[i]->SetInputActionPressed(0);
						}
					}
				}
			}
		}
	}
}
