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

UENUM(Blueprintable)
enum class EControlState
{
	Player = 0,
	AI
};

UCLASS()
class BEATEMUP_API ABaseFighter : public APawn
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	float WalkSpeed;

	UPROPERTY(BlueprintReadOnly)
	float CurrentHealth;

	InputBuffer* BufferHandler;

	UPROPERTY()
	UBaseState* State;

	HitboxHandler* HBHandler;

public:
	// Sets default values for this pawn's properties
	ABaseFighter();

	UFUNCTION(BlueprintCallable)
	void InitializeController();
	UFUNCTION(BlueprintCallable)
	void InitializeAIController();

	UFUNCTION(BlueprintNativeEvent)
	void InitializeAIControllerBlueprint();

	UFUNCTION(BlueprintCallable)
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
	void SetLocalSlowMotion();

	UFUNCTION(BlueprintCallable)
	const bool IsGrounded();

	UFUNCTION(BlueprintCallable)
	const bool IsBlocking();

	UFUNCTION(BlueprintCallable)
	const bool HasHitEnemy();

	/*UFUNCTION(BlueprintCallable)
	BaseState* GetCurrentState() { return nullptr; };*/

	InputBuffer* ReturnInputBuffer();

	HitboxHandler* ReturnHitboxHandler();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, Category = "Default animations")
	TSubclassOf<AController> Cont;

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
	UPROPERTY(EditAnywhere, Category = "Default animations")
	UAnimSequence* BlockAnim;
	UPROPERTY(EditAnywhere, Category = "Default animations")
	UAnimSequence* ParryAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputMappingContext* MappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* MovementInput;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttackState> LightAttack;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttackState> MediumAttack;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAirAttackState> AirLightAttack;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBaseState> Grab;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBaseState> Dash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int FighterCombo;

	int FighterComboTimer = 0;

	/*UPROPERTY(EditAnywhere)
	TSubclassOf<UBaseState> CustomState;*/

	UFloatingPawnMovement* MovementPawn;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);*/

	UFUNCTION(BlueprintCallable)
	void AddToCombo();

	UFUNCTION(BlueprintCallable)
	void SetPhysicsSlowMotion();

	UFUNCTION(BlueprintCallable)
	void SetPhysicsNormal();

	UFUNCTION(BlueprintCallable)
	bool InputCheck(EInputType input);

	UFUNCTION(BlueprintCallable)
	UBaseState* CancelToState(EInputType input, TSubclassOf<UBaseState> newState);

	UFUNCTION(BlueprintCallable)
	const FVector2D ReturnMoveInput() const;

	UFUNCTION(BlueprintCallable)
	UCapsuleComponent* ReturnCapsuleMesh() const;
	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* ReturnSkeletalMesh() const;

	UHitbox* ReturnHitbox() const;

	void SetMoveDirection(FVector dir);

	TEnumAsByte <EControlState> ControlState = EControlState::Player;

protected:

	UPROPERTY(BlueprintReadOnly)
	FVector2D MoveDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHitbox* Hitbox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMesh;

private:

	float SlowMotionTime;
	bool SlowMotion;

	void ButtonPressed(const FInputActionValue& value, const int index);
	void SetMoveDirection(const FInputActionValue& value);
	void ResetMoveDirection(const FInputActionValue& value);
};
