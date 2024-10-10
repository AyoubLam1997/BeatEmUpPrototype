// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/States/BaseStates.h"
#include "Character/BaseFighter.h"

#include "Collision/Hitbox.h"

#include "Animation/AnimSingleNodeInstance.h"

void UGroundedState::Enter(ABaseFighter& fighter)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Entering default state"));
	fighter.ReturnSkeletalMesh()->PlayAnimation(fighter.IdleAnim, 1);
}

UBaseState* UGroundedState::HandleInput(ABaseFighter& fighter)
{
	if (fighter.ReturnMoveInput().Length() > 0)
		return NewObject<UWalkState>();

	if (fighter.InputCheck(EInputType::LightPunch))
		return DuplicateObject(fighter.LightAttack.GetDefaultObject(), nullptr);
	if (fighter.InputCheck(EInputType::MediumPunch))
		return DuplicateObject(fighter.MediumAttack.GetDefaultObject(), nullptr);
	if (fighter.InputCheck(EInputType::HeavyPunch))
		return NewObject<UJumpState>();

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
	fighter.ReturnSkeletalMesh()->PlayAnimation(fighter.LayingAnim, 0);

	LayingTimer = 0;
}

UBaseState* ULayingState::HandleInput(ABaseFighter& fighter)
{
	if (LayingTimer >= 60)
		return NewObject<UStandingUpState>();

	return nullptr;
}

void ULayingState::Update(ABaseFighter& fighter)
{
	LayingTimer += 1;
}

void ULayingState::Exit(ABaseFighter& fighter)
{
}

void UStandingUpState::Enter(ABaseFighter& fighter)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Entering standing up state"));
	fighter.ReturnSkeletalMesh()->PlayAnimation(fighter.StandingUpAnim, 0);
}

UBaseState* UStandingUpState::HandleInput(ABaseFighter& fighter)
{
	if (fighter.ReturnSkeletalMesh()->GetPosition() >= fighter.StandingUpAnim->GetPlayLength())
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
	fighter.ReturnSkeletalMesh()->PlayAnimation(fighter.WalkAnim, 1);
}

UBaseState* UWalkState::HandleInput(ABaseFighter& fighter)
{
	if (fighter.ReturnMoveInput().Length() <= 0)
		return NewObject <UGroundedState>();

	return fighter.ReturnAttackState();
}

void UWalkState::Update(ABaseFighter& fighter)
{
	fighter.Walk();
}

void UWalkState::Exit(ABaseFighter& fighter)
{
}

void UAirState::Enter(ABaseFighter& fighter)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Entering air state"));
	fighter.ReturnSkeletalMesh()->PlayAnimation(fighter.JumpAnim, 0);
}

UBaseState* UAirState::HandleInput(ABaseFighter& fighter)
{
	if (fighter.InputCheck(EInputType::LightPunch))
		return DuplicateObject(fighter.AirLightAttack.GetDefaultObject(), nullptr);

	if (fighter.GetVelocity().Z < 0 && fighter.IsGrounded())
		return NewObject<UGroundedState>();

	return nullptr;
}

void UAirState::Update(ABaseFighter& fighter)
{
	fighter.Walk();
}

void UAirState::Exit(ABaseFighter& fighter)
{
}

void UJumpState::Enter(ABaseFighter& fighter)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Entering jump state"));
	fighter.ReturnSkeletalMesh()->PlayAnimation(fighter.JumpAnim, 0);

	fighter.ReturnCapsuleMesh()->AddImpulse(FVector(0, 0, 25000));
}

UBaseState* UJumpState::HandleInput(ABaseFighter& fighter)
{
	return UAirState::HandleInput(fighter);
}

void UJumpState::Update(ABaseFighter& fighter)
{
	fighter.Walk();
}

void UJumpState::Exit(ABaseFighter& fighter)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("exiting jump state"));
}

UStunState::UStunState()
{
	StunDuration = 60;
}

UStunState::UStunState(int stun)
{
	StunDuration = stun;
}

void UStunState::Enter(ABaseFighter& fighter)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Entering stun state"));

	CurrentStunTime = 0;

	if (!fighter.IsGrounded())
		fighter.ReturnSkeletalMesh()->PlayAnimation(fighter.FallBlendAnim, 0);
	else
		fighter.ReturnSkeletalMesh()->PlayAnimation(fighter.StunnedAnim, 0);
}

UBaseState* UStunState::HandleInput(ABaseFighter& fighter)
{
	if (CurrentStunTime >= StunDuration)
	{
		if (fighter.IsGrounded())
		{
			return NewObject<UGroundedState>();
		}

		UAirStunState* state = NewObject<UAirStunState>();

		state->Init(fighter.ReturnCapsuleMesh()->GetPhysicsLinearVelocity() / 5, 60);

		return state;
	}

	return nullptr;
}

void UStunState::Update(ABaseFighter& fighter)
{
	CurrentStunTime += 1;
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

	StunDuration = duration;
}

void UKnockbackStunState::Enter(ABaseFighter& fighter)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Entering stun state"));

	CurrentStunTime = 0;

	if(!fighter.IsGrounded())
		fighter.ReturnSkeletalMesh()->PlayAnimation(fighter.FallBlendAnim, 0);
	else
		fighter.ReturnSkeletalMesh()->PlayAnimation(fighter.StunnedAnim, 0);

	if (!fighter.IsGrounded())
	{
		fighter.ReturnCapsuleMesh()->AddImpulse(Direction * 2);
	}
	else
		fighter.ReturnCapsuleMesh()->AddImpulse(Direction * 5);
}

UBaseState* UKnockbackStunState::HandleInput(ABaseFighter& fighter)
{
	if (CurrentStunTime >= StunDuration)
	{
		if (fighter.IsGrounded())
		{
			return NewObject<UGroundedState>();
		}

		UAirStunState* state = NewObject<UAirStunState>();

		state->Init(fighter.ReturnCapsuleMesh()->GetPhysicsLinearVelocity() / 5, 60);

		return state;
	}

	return nullptr;
}

void UKnockbackStunState::Update(ABaseFighter& fighter)
{
	CurrentStunTime += 1;

	if (fighter.ReturnCapsuleMesh()->GetPhysicsLinearVelocity().Z != 0)
	{
		float x = fighter.ReturnCapsuleMesh()->GetPhysicsLinearVelocity().X;
		float y = fighter.ReturnCapsuleMesh()->GetPhysicsLinearVelocity().Y;

		if(!fighter.IsGrounded())
			fighter.ReturnCapsuleMesh()->SetPhysicsLinearVelocity(FVector(x, y, 0));
	}
}

void UKnockbackStunState::Exit(ABaseFighter& fighter)
{
	fighter.ReturnCapsuleMesh()->SetPhysicsLinearVelocity(FVector::Zero());
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

	fighter.ReturnSkeletalMesh()->PlayAnimation(fighter.FallBlendAnim, 1);
	FVector BlendParams(0, fighter.GetVelocity().Z, 0);
	fighter.ReturnSkeletalMesh()->GetSingleNodeInstance()->SetBlendSpacePosition(BlendParams);

	fighter.ReturnCapsuleMesh()->AddImpulse(Direction * 5);

	CurrentStunTime = 0;
}

UBaseState* UAirStunState::HandleInput(ABaseFighter& fighter)
{
	if (fighter.GetVelocity().Z <= 0 && fighter.IsGrounded() && CurrentStunTime > 1)
		return NewObject<ULayingState>();

	return nullptr;
}

void UAirStunState::Update(ABaseFighter& fighter)
{
	CurrentStunTime += 1;
	FVector BlendParams(0, fighter.GetVelocity().Z, 0);
	fighter.ReturnSkeletalMesh()->GetSingleNodeInstance()->SetBlendSpacePosition(BlendParams);
}

void UAirStunState::Exit(ABaseFighter& fighter)
{
	fighter.ReturnCapsuleMesh()->SetPhysicsLinearVelocity(FVector::Zero());
}

void UAttackState::Enter(ABaseFighter& fighter)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Entering attack state"));

	fighter.ReturnSkeletalMesh()->PlayAnimation(AnimationSequence, 0);

	fighter.ReturnHitbox()->SetResponder(Responder);

	CurrentFrame = 0;

	StateEnter(&fighter);

	//fighter.m_SkeletalMesh->SetPosition(.15f);
}

UBaseState* UAttackState::HandleInput(ABaseFighter& fighter)
{
	if (fighter.ReturnSkeletalMesh()->GetPosition() >= AnimationSequence->GetPlayLength())
	{
		if(!fighter.IsGrounded())
			return NewObject <UAirState>();
		return NewObject <UGroundedState>();
	}

	if (StateChange != nullptr)
		return StateChange;

	return StateHandleInput(&fighter);
}

void UAttackState::Update(ABaseFighter& fighter)
{
	CurrentFrame = (int)(fighter.ReturnSkeletalMesh()->GetPosition() * 60);

	FString frameText = FString::FromInt(CurrentFrame);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Frame: ") + frameText);

	if (CurrentFrame == MinFrame)
		fighter.ReturnHitbox()->OpenColliderState();

	if (CurrentFrame == MaxFrame)
		fighter.ReturnHitbox()->CloseColliderState();

	StateUpdate(&fighter);
}

void UAttackState::Exit(ABaseFighter& fighter)
{
	StateExit(&fighter);

	fighter.ReturnHitbox()->CloseColliderState();
	fighter.ReturnHitbox()->ClearCollidedObjects();
	fighter.ReturnHitboxHandler()->ClearCollidedObjects();

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Exiting attack state"));
}

void UAirAttackState::Enter(ABaseFighter& fighter)
{
	fighter.ReturnCapsuleMesh()->SetPhysicsLinearVelocity(FVector(0, 0, 0));

	UAttackState::Enter(fighter);

	//fighter.m_SkeletalMesh->SetPosition(.15f);
}

UBaseState* UAirAttackState::HandleInput(ABaseFighter& fighter)
{
	if (fighter.ReturnSkeletalMesh()->GetPosition() >= AnimationSequence->GetPlayLength())
	{
		if (fighter.GetVelocity().Z < 0 && fighter.IsGrounded())
			return NewObject <UGroundedState>();
		return NewObject <UAirState>();
	}

	if (StateChange != nullptr)
		return StateChange;

	return StateHandleInput(&fighter);
}

void UAirAttackState::Update(ABaseFighter& fighter)
{
	if (!AffectedByGravity)
		fighter.ReturnCapsuleMesh()->SetPhysicsLinearVelocity(FVector(0, 0, 0));

	UAttackState::Update(fighter);
}

void UAirAttackState::Exit(ABaseFighter& fighter)
{
	UAttackState::Exit(fighter);
}

//void UAirAttackState::Enter(ABaseFighter& fighter)
//{
//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Entering attack state"));
//
//	fighter.SkeletalMesh->PlayAnimation(m_AnimationSequence, 0);
//
//	fighter.ReturnHitbox()->SetResponder(m_Responder);
//
//	fighter.ReturnCapsuleMesh()->SetPhysicsLinearVelocity(FVector(0, 0, 0));
//
//	m_CurrentFrame = 0;
//
//	StateEnter(&fighter);
//
//	//fighter.m_SkeletalMesh->SetPosition(.15f);
//}
//
//UBaseState* UAirAttackState::HandleInput(ABaseFighter& fighter)
//{
//	if (fighter.SkeletalMesh->GetPosition() >= m_AnimationSequence->GetPlayLength())
//		return NewObject <UAirState>();
//
//	StateHandleInput(&fighter);
//
//	if (StateChange != nullptr)
//		return StateChange;
//
//	return UAirState::HandleInput(fighter);
//}
//
//void UAirAttackState::Update(ABaseFighter& fighter)
//{
//	m_CurrentFrame = (int)(fighter.SkeletalMesh->GetPosition() * 60);
//
//	FString frameText = FString::FromInt(m_CurrentFrame);
//
//	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Frame: ") + frameText);
//
//	if (m_CurrentFrame == m_MinFrame)
//		fighter.ReturnHitbox()->OpenColliderState();
//
//	if (m_CurrentFrame == m_MaxFrame)
//		fighter.ReturnHitbox()->CloseColliderState();
//
//	StateUpdate(&fighter);
//}
//
//void UAirAttackState::Exit(ABaseFighter& fighter)
//{
//	StateExit(&fighter);
//
//	fighter.ReturnHitbox()->CloseColliderState();
//	fighter.ReturnHitbox()->ClearCollidedObjects();
//	fighter.ReturnHitboxHandler()->ClearCollidedObjects();
//
//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Exiting attack state"));
//}

void UAttackState::StateEnter_Implementation(ABaseFighter* fighter)
{

}

UBaseState* UAttackState::StateHandleInput_Implementation(ABaseFighter* fighter)
{
	return nullptr;
}

void UAttackState::StateUpdate_Implementation(ABaseFighter* fighter)
{

}

void UAttackState::StateExit_Implementation(ABaseFighter* fighter)
{

}
//
//void UAirAttackState::StateEnter_Implementation(ABaseFighter* fighter)
//{
//
//}
//
//UBaseState* UAirAttackState::StateHandleInput_Implementation(ABaseFighter* fighter)
//{
//	return nullptr;
//}
//
//void UAirAttackState::StateUpdate_Implementation(ABaseFighter* fighter)
//{
//
//}
//
//void UAirAttackState::StateExit_Implementation(ABaseFighter* fighter)
//{
//
//}

UBaseState* UGroundedComboAttackState::HandleInput(ABaseFighter& fighter)
{
	if (CurrentFrame >= MinCancelFrame && CurrentFrame <= MaxCancelFrame)
	{
		if (fighter.HasHitEnemy())
		{
			if (fighter.InputCheck(State.Input))
				return DuplicateObject(State.State.GetDefaultObject(), nullptr);
		}
	}

	StateHandleInput(&fighter);

	return UAttackState::HandleInput(fighter);
}

UBaseState* UAirComboAttackState::HandleInput(ABaseFighter& fighter)
{
	if (CurrentFrame >= MinCancelFrame && CurrentFrame <= MaxCancelFrame)
	{
		if (fighter.HasHitEnemy())
		{
			if (fighter.InputCheck(State.Input))
				return DuplicateObject(State.State.GetDefaultObject(), nullptr);
		}
	}

	StateHandleInput(&fighter);

	return UAirAttackState::HandleInput(fighter);
}

//
//UBaseState* UAirComboAttackState::HandleInput(ABaseFighter& fighter)
//{
//	for (int i = 0; i < fighter.ReturnInputBuffer()->m_InputBufferItems.Num(); i++)
//	{
//		if (m_CurrentFrame >= m_MinCancelFrame && m_CurrentFrame <= m_MaxCancelFrame)
//		{
//			if (fighter.ReturnHitboxHandler()->ReturnCollidedActors().Num() > 0)
//			{
//				if (fighter.ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer[0].CanExecute())
//				{
//					fighter.ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer[0].SetUsedTrue();
//
//					return DuplicateObject(m_State.m_State.GetDefaultObject(), nullptr);
//				}
//			}
//		}
//	}
//
//	StateHandleInput(&fighter);
//
//	return UAirAttackState::HandleInput(fighter);
//}

FStateToTransition::FStateToTransition() : State(nullptr)
{

}

FStatesToTransitionButton::FStatesToTransitionButton() : FStateToTransition(), Input(EInputType::LightPunch)
{

}

void UCustomState::Enter(ABaseFighter& fighter)
{
	StateEnter(&fighter);
}

UBaseState* UCustomState::HandleInput(ABaseFighter& fighter)
{
	return StateHandleInput(&fighter);
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

UBaseState* UCustomState::StateHandleInput_Implementation(ABaseFighter* fighter)
{
	return nullptr;
}

void UCustomState::StateUpdate_Implementation(ABaseFighter* fighter)
{

}

void UCustomState::StateExit_Implementation(ABaseFighter* fighter)
{

}

void UBlockState::Enter(ABaseFighter& fighter)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Entering block state"));
	fighter.ReturnSkeletalMesh()->PlayAnimation(fighter.BlockAnim, 1);

	BlockTime = 0;
}

UBaseState* UBlockState::HandleInput(ABaseFighter& fighter)
{
	for (int i = 0; i < fighter.ReturnInputBuffer()->m_InputBufferItems.Num(); i++)
	{
		if (fighter.ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer.Num() > 0)
		{
			if (fighter.ReturnInputBuffer()->m_InputBufferItems[i]->InputDirection == EInputType::LightPunch && fighter.ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer[0].HoldTime <= 0)
			{
				if(BlockTime <= 3)
					return NewObject<UParryState>();

				return NewObject<UGroundedState>();
			}
		}
	}

	return nullptr;
}

void UBlockState::Update(ABaseFighter& fighter)
{
	BlockTime += 1;
}

void UBlockState::Exit(ABaseFighter& fighter)
{
}

void UParryState::Enter(ABaseFighter& fighter)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Entering parry state"));
	fighter.ReturnSkeletalMesh()->PlayAnimation(fighter.ParryAnim, 1);

	CurrentParryTimer = 0;
}

UBaseState* UParryState::HandleInput(ABaseFighter& fighter)
{
	if (CurrentParryTimer >= MaxParryTime)
		return NewObject<UGroundedState>();

	return nullptr;
}

void UParryState::Update(ABaseFighter& fighter)
{
	CurrentParryTimer += 1;
}

void UParryState::Exit(ABaseFighter& fighter)
{
}
