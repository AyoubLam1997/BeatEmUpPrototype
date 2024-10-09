// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Character/BaseFighter.h"

#include "BeatEmTestArea.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_API ABeatEmTestArea : public AGameModeBase
{
	GENERATED_BODY()

public:

	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf< ABaseFighter> FighterToSpawn;
	UPROPERTY(EditAnywhere)
	TSubclassOf< ABaseFighter> FighterToSpawn2;

	 ABaseFighter* Fighter;
	 ABaseFighter* Fighter2;

	class ACameraActor* Camera;
};
