// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Collision/Hitbox.h"

/**
 *
 */
class BEATEMUP_API HitboxHandler
{
public:
	HitboxHandler();
	~HitboxHandler();

	UFUNCTION(BlueprintCallable)
	void AddToCollidedArray(AActor* actor);
	// Clears the array of actors that has collided with the hitbox
	UFUNCTION(BlueprintCallable)
	void ClearCollidedObjects();
	UFUNCTION(BlueprintCallable)
	void CloseAllHitboxes();

	TArray<AActor*> ReturnCollidedActors() { return m_CollidedObjects; }

private:
	TArray<AActor*> m_CollidedObjects;

	TArray<UHitbox*> m_AllHitboxes;
};
