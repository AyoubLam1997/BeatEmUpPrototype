// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseFighter.h"
#include "AIFighter.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_API AAIFighter : public ABaseFighter
{
	GENERATED_BODY()
	
public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
