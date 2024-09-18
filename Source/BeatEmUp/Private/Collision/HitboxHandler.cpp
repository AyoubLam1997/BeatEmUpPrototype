// Fill out your copyright notice in the Description page of Project Settings.


#include "Collision/HitboxHandler.h"

HitboxHandler::HitboxHandler()
{
}

HitboxHandler::~HitboxHandler()
{
}

void HitboxHandler::AddToCollidedArray(AActor* actor)
{
	// Add the actor to the array of collided actors
	m_CollidedObjects.Push(actor);
}

// Empties collided objects array so that they can be handled again
void HitboxHandler::ClearCollidedObjects()
{
	for (UHitbox* hitbox : m_AllHitboxes)
	{
		hitbox->ClearCollidedObjects();
	}

	m_CollidedObjects.Empty();
}

void HitboxHandler::CloseAllHitboxes()
{
	for (int i = 0; i < m_AllHitboxes.Num(); i++)
		m_AllHitboxes[i]->CloseColliderState();
}
