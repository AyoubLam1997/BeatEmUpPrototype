// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Animation/BlendSpace1D.h"

#include "InputMappingContext.h"

#include "Character/States/BaseStates.h"

#include "Input/InputBuffer.h"

#include "Collision/Hitbox.h"
#include "Collision/HitboxHandler.h"

#include <GameFramework/FloatingPawnMovement.h>

#include "BaseFighter.generated.h"

UCLASS()
class BEATEMUP_API ABaseFighter : public APawn
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	float WalkSpeed;

	UPROPERTY(BlueprintReadOnly)
	float CurrentHealth;

	EInputType ForwardInput;
	EInputType BackwardInput;

	InputBuffer* BufferHandler;

	UPROPERTY()
	UBaseState* State;

	HitboxHandler* HBHandler;

	bool FacingRight = 0;

	int FreezeTime = 0;

public:
	// Sets default values for this pawn's properties
	ABaseFighter();

	UFUNCTION(BlueprintCallable)
	void InitializeController();

	void Walk();

	UFUNCTION(BlueprintCallable)
	void RotateToInputDirection();

	UFUNCTION(BlueprintCallable)
	void ChangeState(UBaseState* newState);

	UFUNCTION(BlueprintCallable)
	void ChangeStateReference(TSubclassOf<UBaseState> newState);

	UBaseState* ReturnAttackState();

	UFUNCTION(BlueprintCallable)
	void ChangeToGroundedState();
	UFUNCTION(BlueprintCallable)
	void ChangeToStunState();

	UFUNCTION(BlueprintCallable)
	void ChangeToStunStateKnock(FVector dir);
	UFUNCTION(BlueprintCallable)
	void ChangeToStunStateAir(FVector dir);

	UFUNCTION(BlueprintCallable)
	const bool IsGrounded();

	UFUNCTION(BlueprintCallable)
	bool HasHitEnemy();

	/*UFUNCTION(BlueprintCallable)
	BaseState* GetCurrentState() { return nullptr; };*/

	InputBuffer* ReturnInputBuffer();

	HitboxHandler* ReturnHitboxHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHitbox* Hitbox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, Category = "Default animations")
	UBlendSpace* FallBlendAnim;
	UPROPERTY(EditAnywhere, Category = "Default animations")
	UAnimSequence* IdleAnim;
	UPROPERTY(EditAnywhere, Category = "Default animations")
	UAnimSequence* WalkAnim;
	UPROPERTY(EditAnywhere, Category = "Default animations")
	UAnimSequence* StunnedAnim;
	UPROPERTY(EditAnywhere, Category = "Default animations")
	UAnimSequence* LayingAnim;
	UPROPERTY(EditAnywhere, Category = "Default animations")
	UAnimSequence* StandingUpAnim;
	UPROPERTY(EditAnywhere, Category = "Default animations")
	UAnimSequence* JumpAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputMappingContext* MappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* MovementInput;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGroundedAttackState> LightAttack;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGroundedAttackState> MediumAttack;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAirAttackState> AirLightAttack;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBaseState> Grab;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBaseState> Dash;

	/*UPROPERTY(EditAnywhere)
	TSubclassOf<UBaseState> CustomState;*/

	UPROPERTY(BlueprintReadOnly)
	FVector2D MoveDirection;

	FRotator CurrentRotation;

	UFloatingPawnMovement* MovementPawn;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);*/

	UFUNCTION(BlueprintCallable)
	void SetPhysicsSlowMotion();

	UFUNCTION(BlueprintCallable)
	void SetPhysicsNormal();

	UFUNCTION(BlueprintCallable)
	bool InputCheck(EInputType input);
	UFUNCTION(BlueprintCallable)
	UBaseState* CancelToState(EInputType input, TSubclassOf<UBaseState> newState);

	float SlowMotionTime;
	bool SlowMotion;

private:

	void ButtonPressed(const FInputActionValue& value, const int index);
	void SetMoveDirection(const FInputActionValue& value);
	void ResetMoveDirection(const FInputActionValue& value);

};
