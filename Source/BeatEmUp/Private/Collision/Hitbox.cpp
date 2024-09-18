// Fill out your copyright notice in the Description page of Project Settings.


#include "Collision/Hitbox.h"
#include "Collision/HitboxHandler.h"

#include "DrawDebugHelpers.h"
#include "Materials/MaterialRenderProxy.h"

UHitbox::UHitbox()
{
	PrimaryComponentTick.bCanEverTick = true;

	m_ColliderState = ColliderState::Closed;
}

UHitbox::~UHitbox()
{
	//delete m_Responder;
}

void UHitbox::BeginPlay()
{
	Super::BeginPlay();
}

void UHitbox::AssignHitboxHandler(HitboxHandler* handler)
{
	m_HitboxHandler = handler;
}

void UHitbox::UpdateHitbox()
{
	// Returns method when the collider state is closed
	if (m_ColliderState == ColliderState::Closed || m_HitboxHandler == nullptr)
		return;

	TArray<AActor*> actors;

	// Get all the colliding actors in this frame
	GetOverlappingActors(actors);

	for (AActor* actor : actors)
	{
		if (GetOwner() == actor)
		{
			actors.Remove(actor);
			continue;
		}

		if (!m_CollidedObjects.Contains(actor))
		{
			m_CollidedObjects.Push(actor);

			// Checks if the colliding object is already in the array to prevent handling the actor multiple times
			if (!m_HitboxHandler->ReturnCollidedActors().Contains(actor))
			{
				// Add the actor to the array of collided actors
				m_HitboxHandler->AddToCollidedArray(actor);

				// Set collider state to closed to prevent early collision handling
				// Call the responder & handle the collision there
				// DEV NOTE: Handles like an event, collision should only be called when a new actor is
				// colliding with the hitbox & should only be called for 1 frame.

				// DEV NOTE: Optionally call this even when there is no hitbox handler?
				m_Responder->CollisionWith(GetOwner(), actor);
			}
		}
	}

	// Sets the collision state to Colliding if the number of colliding actors is greater than 0
	// Otherwise, set the colliding state to Open
	m_ColliderState = actors.Num() > 0 ? ColliderState::Colliding : ColliderState::Open;
}

// Function to draw the debug box. Color changes based on the collision state
void UHitbox::DebugDrawHitbox()
{
	FVector boxExtentsScaled = FVector(BoxExtent * GetComponentScale());

	switch (m_ColliderState)
	{
	case ColliderState::Open:
		DrawDebugSolidBox(GetWorld(), GetComponentLocation(), boxExtentsScaled, GetComponentQuat(), m_OpenStateColor);
		break;
	case ColliderState::Colliding:
		DrawDebugSolidBox(GetWorld(), GetComponentLocation(), boxExtentsScaled, GetComponentQuat(), m_CollidingStateColor);
		break;
	case ColliderState::Closed:
		DrawDebugSolidBox(GetWorld(), GetComponentLocation(), boxExtentsScaled, GetComponentQuat(), m_ClosedStateColor);
		break;
	}
}

void UHitbox::SetResponder(UHitboxResponder* responder)
{
	m_Responder = responder;
}

void UHitbox::ClearCollidedObjects()
{
	m_CollidedObjects.Empty();
}

// Set the collider state to Open
void UHitbox::OpenColliderState()
{
	m_ColliderState = ColliderState::Open;
}

// Set the collider state to Closed
void UHitbox::CloseColliderState()
{
	m_ColliderState = ColliderState::Closed;
}

void UHitbox::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateHitbox();
	DebugDrawHitbox();
}

void UHitbox::OnRegister()
{
	Super::OnRegister();
}
