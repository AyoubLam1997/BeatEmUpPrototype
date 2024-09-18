// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/States/BaseStates.h"
#include "Character/BaseFighter.h"

#include "Collision/Hitbox.h"

#include "Animation/AnimSingleNodeInstance.h"

void GroundedState::Enter(ABaseFighter& fighter)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Entering default state"));
	fighter.SkeletalMesh->PlayAnimation(fighter.IdleAnim, 1);
}

BaseState* GroundedState::HandleInput(ABaseFighter& fighter)
{
	if (fighter.MoveDirection.Length() > 0)
		return new WalkState();

	for (int i = 0; i < fighter.ReturnInputBuffer()->m_InputBufferItems.Num(); i++)
	{
		if (fighter.ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer.Num() > 0)
		{
			if (fighter.ReturnInputBuffer()->m_InputBufferItems[i]->InputDirection == EInputType::LightPunch && fighter.ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer[0].HoldTime > 0)
			{
				if (fighter.ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer[0].CanExecute())
				{
					fighter.ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer[0].SetUsedTrue();
					return fighter.LightAttack;
				}
			}
		}
	}

	return nullptr;
}

void GroundedState::Update(ABaseFighter& fighter)
{

}

void GroundedState::Exit(ABaseFighter& fighter)
{
}

void WalkState::Enter(ABaseFighter& fighter)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Entering walk state"));
	fighter.SkeletalMesh->PlayAnimation(fighter.WalkAnim, 1);
}

BaseState* WalkState::HandleInput(ABaseFighter& fighter)
{
	if (fighter.MoveDirection.Length() <= 0)
		return new GroundedState();

	fighter.ReturnAttackState();

	return nullptr;
}

void WalkState::Update(ABaseFighter& fighter)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(fighter.MoveDirection.Length()));
	fighter.Walk();
}

void WalkState::Exit(ABaseFighter& fighter)
{
}

StunState::StunState()
{
	m_StunDuration = 60;
}

StunState::StunState(int stun)
{
	m_StunDuration = stun;
}

void StunState::Enter(ABaseFighter& fighter)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Entering stun state"));

	m_CurrentStunTime = 0;

	fighter.SkeletalMesh->PlayAnimation(fighter.StunnedAnim, 0);
}

BaseState* StunState::HandleInput(ABaseFighter& fighter)
{
	if (m_CurrentStunTime >= m_StunDuration)
		return new GroundedState();

	return nullptr;
}

void StunState::Update(ABaseFighter& fighter)
{
	m_CurrentStunTime += 1;
}

void StunState::Exit(ABaseFighter& fighter)
{

}

KnockbackStunState::KnockbackStunState() : StunState()
{

}

KnockbackStunState::KnockbackStunState(FVector dir, int stun) : StunState(stun)
{
	Direction = dir;
}

void KnockbackStunState::Enter(ABaseFighter& fighter)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Entering stun state"));

	m_CurrentStunTime = 0;

	fighter.SkeletalMesh->PlayAnimation(fighter.StunnedAnim, 0);

	fighter.CapsuleMesh->AddImpulse(Direction * 5);
}

BaseState* KnockbackStunState::HandleInput(ABaseFighter& fighter)
{
	if (m_CurrentStunTime >= m_StunDuration)
		return new GroundedState();

	return nullptr;
}

void KnockbackStunState::Update(ABaseFighter& fighter)
{
	m_CurrentStunTime += 1;
}

void KnockbackStunState::Exit(ABaseFighter& fighter)
{
	fighter.CapsuleMesh->SetPhysicsLinearVelocity(FVector::Zero());
}

void UGroundedAttackState::Enter(ABaseFighter& fighter)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Entering attack state"));

	fighter.SkeletalMesh->PlayAnimation(m_AnimationSequence, 0);

	fighter.Hitbox->SetResponder(m_Responder);

	m_CurrentFrame = 0;

	//fighter.m_SkeletalMesh->SetPosition(.15f);
}

BaseState* UGroundedAttackState::HandleInput(ABaseFighter& fighter)
{
	if (fighter.SkeletalMesh->GetPosition() >= m_AnimationSequence->GetPlayLength())
		return new GroundedState();

	return nullptr;
}

void UGroundedAttackState::Update(ABaseFighter& fighter)
{
	m_CurrentFrame = (int)(fighter.SkeletalMesh->GetPosition() * 60);

	FString frameText = FString::FromInt(m_CurrentFrame);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Frame: ") + frameText);

	if (m_CurrentFrame == m_MinFrame)
		fighter.Hitbox->OpenColliderState();

	if (m_CurrentFrame == m_MaxFrame)
		fighter.Hitbox->CloseColliderState();
}

void UGroundedAttackState::Exit(ABaseFighter& fighter)
{
	fighter.Hitbox->CloseColliderState();
	fighter.Hitbox->ClearCollidedObjects();
	fighter.ReturnHitboxHandler()->ClearCollidedObjects();
}

BaseState* UGroundedComboAttackState::HandleInput(ABaseFighter& fighter)
{
	for (int i = 0; i < fighter.ReturnInputBuffer()->m_InputBufferItems.Num(); i++)
	{
		if (m_CurrentFrame >= m_MinCancelFrame && m_CurrentFrame <= m_MaxCancelFrame)
		{
			if (fighter.ReturnHitboxHandler()->ReturnCollidedActors().Num() > 0)
			{
				if (fighter.ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer[0].CanExecute())
				{
					fighter.ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer[0].SetUsedTrue();
					return m_State.m_State;
				}
			}
		}
	}

	return UGroundedAttackState::HandleInput(fighter);
}

FStateToTransition::FStateToTransition() : m_State(nullptr)
{

}

FStatesToTransitionButton::FStatesToTransitionButton() : FStateToTransition(), m_Input(EInputType::LightPunch)
{

}