// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/Interface.h"

#include "Input/InputBuffer.h"

#include "BaseStates.generated.h"

class ABaseFighter;
class UHitboxResponder;
enum class EInputType;

/**
 *
 */

 //UINTERFACE(MinimalAPI, Blueprintable)
 //class UBaseState : public UInterface
 //{
 //	GENERATED_BODY()
 //};

UCLASS(Blueprintable, BlueprintType)
class BEATEMUP_API UBaseState : public UObject
{
	GENERATED_BODY()
public:
	virtual void Enter(ABaseFighter& fighter) {};
	virtual UBaseState* HandleInput(ABaseFighter& fighter) { return nullptr; };
	virtual void Update(ABaseFighter& fighter) {};
	virtual void Exit(ABaseFighter& fighter) {};
};

UCLASS(Blueprintable, BlueprintType)
class BEATEMUP_API UGroundedState : public UBaseState
{
	GENERATED_BODY()
public:
	virtual void Enter(ABaseFighter& fighter) override;
	virtual UBaseState* HandleInput(ABaseFighter& fighter) override;
	virtual void Update(ABaseFighter& fighter) override;
	virtual void Exit(ABaseFighter& fighter) override;
};

UCLASS(Blueprintable, BlueprintType)
class BEATEMUP_API ULayingState : public UGroundedState
{
	GENERATED_BODY()
public:

	//LayingState();
	//LayingState(float duration);

	virtual void Enter(ABaseFighter& fighter) override;
	virtual UBaseState* HandleInput(ABaseFighter& fighter) override;
	virtual void Update(ABaseFighter& fighter) override;
	virtual void Exit(ABaseFighter& fighter) override;

private:

	float m_LayingTimer;
};

UCLASS(Blueprintable, BlueprintType)
class BEATEMUP_API UStandingUpState : public UGroundedState
{
	GENERATED_BODY()
public:

	virtual void Enter(ABaseFighter& fighter) override;
	virtual UBaseState* HandleInput(ABaseFighter& fighter) override;
	virtual void Update(ABaseFighter& fighter) override;
	virtual void Exit(ABaseFighter& fighter) override;
};

UCLASS(Blueprintable, BlueprintType)
class BEATEMUP_API UWalkState : public UGroundedState
{
	GENERATED_BODY()
public:
	virtual void Enter(ABaseFighter& fighter) override;
	virtual UBaseState* HandleInput(ABaseFighter& fighter) override;
	virtual void Update(ABaseFighter& fighter) override;
	virtual void Exit(ABaseFighter& fighter) override;
};
//
//UCLASS(Blueprintable, BlueprintType)
//class BEATEMUP_API UDash : public UPrimaryDataAsset, public GroundedState
//{
//	GENERATED_BODY()
//
//public:
//
//	//Dash();
//	//Dash(FVector impulse);
//
//	virtual void Enter(ABaseFighter& fighter) override {};
//	virtual BaseState* HandleInput(ABaseFighter& fighter) override { return nullptr; };
//	virtual void Update(ABaseFighter& fighter) override {};
//	virtual void Exit(ABaseFighter& fighter) override {};
//
//	UPROPERTY(EditAnywhere)
//	UAnimSequence* m_AnimationSequence;
//
//	UPROPERTY(EditAnywhere)
//	FVector m_DashImpulse;
//
//	UPROPERTY(EditAnywhere)
//	int m_DashStartUp;
//	UPROPERTY(EditAnywhere)
//	int m_DashStartRecovery;
//
//	int m_CurrentFrame;
//};
//
////UCLASS(Blueprintable, BlueprintType)
////class BEATEMUP_API UBackwardDash : public UPrimaryDataAsset, public Dash
////{
////	GENERATED_BODY()
////
////public:
////
////	virtual void Enter(ABaseFighter& fighter) override;
////	virtual BaseState* HandleInput(ABaseFighter& fighter) override;
////	virtual void Update(ABaseFighter& fighter) override;
////	virtual void Exit(ABaseFighter& fighter) override;
////};
//
//class BEATEMUP_API AirborneState : virtual public BaseState
//{
//public:
//
//	float m_CurrentFallVelocity;
//	float m_MaxFallVelocity = 1500.f;
//
//	AirborneState();
//	AirborneState(FVector impulse);
//	AirborneState(float velocity);
//
//	virtual void Enter(ABaseFighter& fighter) override;
//	virtual BaseState* HandleInput(ABaseFighter& fighter) override;
//	virtual void Update(ABaseFighter& fighter) override;
//	virtual void Exit(ABaseFighter& fighter) override;
//
//private:
//
//	FVector m_ImpulseDirection;
//};
//
//class BEATEMUP_API MoveAirborneState : virtual public AirborneState
//{
//public:
//
//	MoveAirborneState();
//	MoveAirborneState(FVector impulse);
//	MoveAirborneState(float velocity);
//
//	virtual void Enter(ABaseFighter& fighter) override;
//	virtual BaseState* HandleInput(ABaseFighter& fighter) override;
//	virtual void Update(ABaseFighter& fighter) override;
//	virtual void Exit(ABaseFighter& fighter) override;
//
//private:
//
//	FVector m_ImpulseDirection;
//};

UCLASS(Blueprintable, BlueprintType)
class BEATEMUP_API UBaseStunState : public UBaseState
{
	GENERATED_BODY()
public:

	virtual void Enter(ABaseFighter& fighter) override {};
	virtual UBaseState* HandleInput(ABaseFighter& fighter) override { return nullptr; };
	virtual void Update(ABaseFighter& fighter) override {};
	virtual void Exit(ABaseFighter& fighter) override {};

private:
};

UCLASS(Blueprintable, BlueprintType)
class BEATEMUP_API UStunState : public UBaseStunState
{
	GENERATED_BODY()
public:

	int m_CurrentStunTime;
	int m_StunDuration;

	UStunState();
	UStunState(int duration);

	virtual void Enter(ABaseFighter& fighter) override;
	virtual UBaseState* HandleInput(ABaseFighter& fighter) override;
	virtual void Update(ABaseFighter& fighter) override;
	virtual void Exit(ABaseFighter& fighter) override;

private:
};

UCLASS(Blueprintable, BlueprintType)
class BEATEMUP_API UKnockbackStunState : public UStunState
{
	GENERATED_BODY()
public:

	FVector Direction;

	UKnockbackStunState();
	//KnockbackStunState(FVector dir, int duration);

	UFUNCTION(BlueprintCallable)
	void Init(FVector dir, int duration);

	virtual void Enter(ABaseFighter& fighter) override;
	virtual UBaseState* HandleInput(ABaseFighter& fighter) override;
	virtual void Update(ABaseFighter& fighter) override;
	virtual void Exit(ABaseFighter& fighter) override;

private:
};

UCLASS(Blueprintable, BlueprintType)
class BEATEMUP_API UAirStunState : public UKnockbackStunState
{
	GENERATED_BODY()
public:

	UAirStunState();
	//AirStunState(FVector dir, int duration);

	virtual void Enter(ABaseFighter& fighter) override;
	virtual UBaseState* HandleInput(ABaseFighter& fighter) override;
	virtual void Update(ABaseFighter& fighter) override;
	virtual void Exit(ABaseFighter& fighter) override;
};
//
////class BEATEMUP_API GrabStartupState : public BaseState
////{
////public:
////
////	GrabStartupState();
////
////	virtual void Enter(ABaseFighter& fighter) override;
////	virtual BaseState* HandleInput(ABaseFighter& fighter) override;
////	virtual void Update(ABaseFighter& fighter) override;
////	virtual void Exit(ABaseFighter& fighter) override;
////
////private:
////
////	int m_CurrentFrame;
////
////	int m_MinFrame = 5;
////	int m_MaxFrame = 8;
////};
//
//// DEV NOTE: Probably make this a uclass that designers can edit
////class BEATEMUP_API GrabbedState : public BaseState
////{
////public:
////
////	GrabbedState();
////
////	virtual void Enter(ABaseFighter& fighter) override;
////	virtual BaseState* HandleInput(ABaseFighter& fighter) override;
////	virtual void Update(ABaseFighter& fighter) override;
////	virtual void Exit(ABaseFighter& fighter) override;
////
////private:
////};
//
////UCLASS(Blueprintable, BlueprintType)
////class BEATEMUP_API UGrabState : public UPrimaryDataAsset, public BaseState
////{
////	GENERATED_BODY()
////
////public:
////
////	UPROPERTY(BlueprintReadWrite, EditAnywhere)
////	float m_Damage;
////
////	UPROPERTY(EditAnywhere)
////	int m_DamageFrame;
////
////	virtual void Enter(ABaseFighter& fighter) override;
////	virtual BaseState* HandleInput(ABaseFighter& fighter) override;
////	virtual void Update(ABaseFighter& fighter) override;
////	virtual void Exit(ABaseFighter& fighter) override;
////
////	/*UFUNCTION(BlueprintCallable)
////	void SetEnemy(ABaseFighter* fighter);*/
////
////private:
////
////	ABaseFighter* m_EnemyFighter;
////};
//

UCLASS(Blueprintable, BlueprintType)
class BEATEMUP_API UCustomState : public UBaseState
{
	GENERATED_BODY()
public:

	virtual void Enter(ABaseFighter& fighter) override;
	virtual UBaseState* HandleInput(ABaseFighter& fighter) override;
	virtual void Update(ABaseFighter& fighter) override;
	virtual void Exit(ABaseFighter& fighter) override;

	UFUNCTION(BlueprintNativeEvent)
	void StateEnter(ABaseFighter* fighter);

	UFUNCTION(BlueprintNativeEvent)
	UCustomState* StateHandleInput(ABaseFighter* fighter);

	UFUNCTION(BlueprintNativeEvent)
	void StateUpdate(ABaseFighter* fighter);

	UFUNCTION(BlueprintNativeEvent)
	void StateExit(ABaseFighter* fighter);
};

UCLASS(Blueprintable, BlueprintType)
class BEATEMUP_API UGroundedAttackState : public UGroundedState
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float m_Damage;

	UPROPERTY(EditAnywhere)
	int m_MinFrame;
	UPROPERTY(EditAnywhere)
	int m_MaxFrame;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int m_FreezeTime;

	UPROPERTY(EditAnywhere)
	UHitboxResponder* m_Responder;

	UPROPERTY(EditAnywhere)
	UAnimSequence* m_AnimationSequence;

	virtual void Enter(ABaseFighter& fighter) override;
	virtual UBaseState* HandleInput(ABaseFighter& fighter) override;
	virtual void Update(ABaseFighter& fighter) override;
	virtual void Exit(ABaseFighter& fighter) override;

	UFUNCTION(BlueprintNativeEvent)
	void StateEnter(ABaseFighter* fighter);

	UFUNCTION(BlueprintNativeEvent)
	UBaseState* StateHandleInput(ABaseFighter* fighter);

	UFUNCTION(BlueprintNativeEvent)
	void StateUpdate(ABaseFighter* fighter);

	UFUNCTION(BlueprintNativeEvent)
	void StateExit(ABaseFighter* fighter);

protected:

	UPROPERTY(BlueprintReadOnly)
	int m_CurrentFrame;
};

//TODO: ADJUST THIS TO TRANSITION TO ANY DESIRED STATE (THAT MAKES USE OF THE DATA TABLES)
USTRUCT(Blueprintable, BlueprintType)
struct FStateToTransition
{
	GENERATED_BODY()

public:

	FStateToTransition();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UBaseState> m_State;
};

USTRUCT(Blueprintable, BlueprintType)
struct FStatesToTransitionButton : public FStateToTransition
{
	GENERATED_BODY()

public:

	FStatesToTransitionButton();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EInputType m_Input;
};

//USTRUCT(Blueprintable, BlueprintType)
//struct FStatesToTransitionMotionInput : public FStateToTransition
//{
//	GENERATED_BODY()
//
//public:
//
//	TArray<EInputType> m_Inputs;
//};

UCLASS(Blueprintable, BlueprintType)
class BEATEMUP_API UGroundedComboAttackState : public UGroundedAttackState
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FStatesToTransitionButton m_State;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int m_MinCancelFrame;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int m_MaxCancelFrame;

	virtual UBaseState* HandleInput(ABaseFighter& fighter) override;
};

//class BEATEMUP_API SpecialMoveState : public BaseState
//{
//public:
//
//	SpecialMoveState();
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	TArray<EInputType> m_RequiredInput;
//
//	MotionInput* m_MotionInput;
//
//	MotionInput* ReturnMotionInput();
//};
//
//UCLASS(Blueprintable, BlueprintType)
//class BEATEMUP_API USpecialMoveState : public UPrimaryDataAsset, public SpecialMoveState
//{
//	GENERATED_BODY()
//
//public:
//};
//
//UCLASS(Blueprintable, BlueprintType)
//class BEATEMUP_API USpecialMoceTransitionState : public USpecialMoveState
//{
//	GENERATED_BODY()
//
//public:
//
//	UPROPERTY(EditAnywhere)
//	FStatesToTransitionButton m_State;
//
//};
