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

	BaseState* State;

	HitboxHandler* HBHandler;

	bool FacingRight = 0;

	int FreezeTime = 0;

public:
	// Sets default values for this pawn's properties
	ABaseFighter();

	UFUNCTION(BlueprintCallable)
	void InitializeController();

	void Walk();

	void ChangeState(BaseState* state);

	BaseState* ReturnAttackState();

	UFUNCTION(BlueprintCallable)
	void ChangeToGroundedState();
	UFUNCTION(BlueprintCallable)
	void ChangeToStunState();

	UFUNCTION(BlueprintCallable)
	void ChangeToStunStateKnock(FVector dir);
	UFUNCTION(BlueprintCallable)
	void ChangeToStunStateAir(FVector dir);

	const bool IsGrounded();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputMappingContext* MappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* MovementInput;

	UPROPERTY(EditAnywhere)
	UGroundedAttackState* LightAttack;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCustomState> CustomState;

	FVector2D MoveDirection;

	FRotator CurrentRotation;

	UFloatingPawnMovement* MovementPawn;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);*/

private:

	void ButtonPressed(const FInputActionValue& value, const int index);
	void SetMoveDirection(const FInputActionValue& value);
	void ResetMoveDirection(const FInputActionValue& value);

};
