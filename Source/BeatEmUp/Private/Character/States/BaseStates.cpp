// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/States/BaseStates.h"
#include "Character/BaseFighter.h"

#include "Collision/Hitbox.h"

#include "Animation/AnimSingleNodeInstance.h"

void UGroundedState::Enter(ABaseFighter& fighter)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Entering default state"));
	fighter.SkeletalMesh->PlayAnimation(fighter.IdleAnim, 1);
}

UBaseState* UGroundedState::HandleInput(ABaseFighter& fighter)
{
	if (fighter.MoveDirection.Length() > 0)
		return NewObject<UWalkState>();

	for (int i = 0; i < fighter.ReturnInputBuffer()->m_InputBufferItems.Num(); i++)
	{
		if (fighter.ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer.Num() > 0)
		{
			if (fighter.ReturnInputBuffer()->m_InputBufferItems[i]->InputDirection == EInputType::LightPunch && fighter.ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer[0].HoldTime > 0)
			{
				if (fighter.ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer[0].CanExecute())
				{
					fighter.ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer[0].SetUsedTrue();

					return DuplicateObject(fighter.LightAttack.GetDefaultObject(), nullptr);
				}
			}
		}
	}

	return nullptr;
}

void UGroundedState::Update(ABaseFighter& fighter)
{

}

void UGroundedState::Exit(ABaseFighter& fighter)
{
}

void ULayingState::Enter(ABaseFighter& fighter)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Entering standing up state"));
	fighter.SkeletalMesh->PlayAnimation(fighter.LayingAnim, 0);

	m_LayingTimer = 0;
}

UBaseState* ULayingState::HandleInput(ABaseFighter& fighter)
{
	if (m_LayingTimer >= 60)
		return NewObject<UStandingUpState>();

	return nullptr;
}

void ULayingState::Update(ABaseFighter& fighter)
{
	m_LayingTimer += 1;
}

void ULayingState::Exit(ABaseFighter& fighter)
{
}

void UStandingUpState::Enter(ABaseFighter& fighter)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Entering standing up state"));
	fighter.SkeletalMesh->PlayAnimation(fighter.StandingUpAnim, 0);
}

UBaseState* UStandingUpState::HandleInput(ABaseFighter& fighter)
{
	if (fighter.SkeletalMesh->GetPosition() >= fighter.StandingUpAnim->GetPlayLength())
		return NewObject<UGroundedState>();

	return nullptr;
}

void UStandingUpState::Update(ABaseFighter& fighter)
{

}

void UStandingUpState::Exit(ABaseFighter& fighter)
{
}

void UWalkState::Enter(ABaseFighter& fighter)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Entering walk state"));
	fighter.SkeletalMesh->PlayAnimation(fighter.WalkAnim, 1);
}

UBaseState* UWalkState::HandleInput(ABaseFighter& fighter)
{
	if (fighter.MoveDirection.Length() <= 0)
		return NewObject <UGroundedState>();

	fighter.ReturnAttackState();

	return nullptr;
}

void UWalkState::Update(ABaseFighter& fighter)
{
	fighter.Walk();
}

void UWalkState::Exit(ABaseFighter& fighter)
{
}

UStunState::UStunState()
{
	m_StunDuration = 60;
}

UStunState::UStunState(int stun)
{
	m_StunDuration = stun;
}

void UStunState::Enter(ABaseFighter& fighter)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Entering stun state"));

	m_CurrentStunTime = 0;

	fighter.SkeletalMesh->PlayAnimation(fighter.StunnedAnim, 0);
}

UBaseState* UStunState::HandleInput(ABaseFighter& fighter)
{
	if (m_CurrentStunTime >= m_StunDuration)
		return NewObject <UGroundedState>();

	return nullptr;
}

void UStunState::Update(ABaseFighter& fighter)
{
	m_CurrentStunTime += 1;
}

void UStunState::Exit(ABaseFighter& fighter)
{

}

UKnockbackStunState::UKnockbackStunState() : UStunState()
{

}

//KnockbackStunState::KnockbackStunState(FVector dir, int stun) : StunState(stun)
//{
//	Direction = dir;
//}

void UKnockbackStunState::Init(FVector dir, int duration)
{
	Direction = dir;

	m_StunDuration = duration;
}

void UKnockbackStunState::Enter(ABaseFighter& fighter)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Entering stun state"));

	m_CurrentStunTime = 0;

	fighter.SkeletalMesh->PlayAnimation(fighter.StunnedAnim, 0);

	fighter.CapsuleMesh->AddImpulse(Direction * 5);
}

UBaseState* UKnockbackStunState::HandleInput(ABaseFighter& fighter)
{
	if (m_CurrentStunTime >= m_StunDuration)
		return NewObject <UGroundedState>();

	return nullptr;
}

void UKnockbackStunState::Update(ABaseFighter& fighter)
{
	m_CurrentStunTime += 1;
}

void UKnockbackStunState::Exit(ABaseFighter& fighter)
{
	fighter.CapsuleMesh->SetPhysicsLinearVelocity(FVector::Zero());
}

UAirStunState::UAirStunState() : UKnockbackStunState()
{

}

//AirStunState::AirStunState(FVector dir, int stun) : KnockbackStunState(dir, stun)
//{
//
//}

void UAirStunState::Enter(ABaseFighter& fighter)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Entering air stun state"));

	fighter.SkeletalMesh->PlayAnimation(fighter.FallBlendAnim, 1);
	FVector BlendParams(0, fighter.GetVelocity().Z, 0);
	fighter.SkeletalMesh->GetSingleNodeInstance()->SetBlendSpacePosition(BlendParams);

	fighter.CapsuleMesh->AddImpulse(Direction * 5);

	m_CurrentStunTime = 0;
}

UBaseState* UAirStunState::HandleInput(ABaseFighter& fighter)
{
	if (fighter.GetVelocity().Z <= 0 && fighter.IsGrounded() && m_CurrentStunTime > 1)
	{
		return NewObject<ULayingState>();
	}

	return nullptr;
}

void UAirStunState::Update(ABaseFighter& fighter)
{
	m_CurrentStunTime += 1;
	FVector BlendParams(0, fighter.GetVelocity().Z, 0);
	fighter.SkeletalMesh->GetSingleNodeInstance()->SetBlendSpacePosition(BlendParams);
}

void UAirStunState::Exit(ABaseFighter& fighter)
{
	fighter.CapsuleMesh->SetPhysicsLinearVelocity(FVector::Zero());
}

void UGroundedAttackState::Enter(ABaseFighter& fighter)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Entering attack state"));

	fighter.SkeletalMesh->PlayAnimation(m_AnimationSequence, 0);

	fighter.Hitbox->SetResponder(m_Responder);

	m_CurrentFrame = 0;

	StateEnter(&fighter);

	//fighter.m_SkeletalMesh->SetPosition(.15f);
}

UBaseState* UGroundedAttackState::HandleInput(ABaseFighter& fighter)
{
	if (fighter.SkeletalMesh->GetPosition() >= m_AnimationSequence->GetPlayLength())
		return NewObject <UGroundedState>();

	StateHandleInput(&fighter);

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

	StateUpdate(&fighter);
}

void UGroundedAttackState::Exit(ABaseFighter& fighter)
{
	fighter.Hitbox->CloseColliderState();
	fighter.Hitbox->ClearCollidedObjects();
	fighter.ReturnHitboxHandler()->ClearCollidedObjects();

	StateExit(&fighter);
}

void UGroundedAttackState::StateEnter_Implementation(ABaseFighter* fighter)
{

}

UBaseState* UGroundedAttackState::StateHandleInput_Implementation(ABaseFighter* fighter)
{
	return nullptr;
}

void UGroundedAttackState::StateUpdate_Implementation(ABaseFighter* fighter)
{

}

void UGroundedAttackState::StateExit_Implementation(ABaseFighter* fighter)
{

}

UBaseState* UGroundedComboAttackState::HandleInput(ABaseFighter& fighter)
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

					return DuplicateObject(m_State.m_State.GetDefaultObject(), nullptr);
				}
			}
		}
	}

	StateHandleInput(&fighter);

	return UGroundedAttackState::HandleInput(fighter);
}

FStateToTransition::FStateToTransition() : m_State(nullptr)
{

}

FStatesToTransitionButton::FStatesToTransitionButton() : FStateToTransition(), m_Input(EInputType::LightPunch)
{

}

void UCustomState::Enter(ABaseFighter& fighter)
{
	StateEnter(&fighter);
}

UBaseState* UCustomState::HandleInput(ABaseFighter& fighter)
{
	StateHandleInput(&fighter);

	return nullptr;
}

void UCustomState::Update(ABaseFighter& fighter)
{
	StateUpdate(&fighter);
}

void UCustomState::Exit(ABaseFighter& fighter)
{
	StateExit(&fighter);
}

void UCustomState::StateEnter_Implementation(ABaseFighter* fighter)
{

}

UCustomState* UCustomState::StateHandleInput_Implementation(ABaseFighter* fighter)
{
	return nullptr;
}

void UCustomState::StateUpdate_Implementation(ABaseFighter* fighter)
{

}

void UCustomState::StateExit_Implementation(ABaseFighter* fighter)
{

}
