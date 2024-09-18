
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/BoxComponent.h"

#include "Hitbox.generated.h"

class HitboxHandler;

// Colliding states of a hitbox
enum class ColliderState
{
	// Collider can check for collision
	Open = 0,
	// Collider cannot check for collision
	Closed,
	// Collider was open & detected collision
	Colliding
};

/*
 /////// SUMMARY
	An interface class. This class handles what happens when collision has been detected
	DEV NOTE: After receiving feedback that this implementation is convoluted, the responder wil probably not be used in
	the future in favour of simple events
 */
UCLASS(Blueprintable, BlueprintType)
class UHitboxResponder : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	// Blueprint-only function. Handles collision handling 
	UFUNCTION(BlueprintImplementableEvent)
	void CollisionWith(AActor* owner, AActor* enemy);
};

// 
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class BEATEMUP_API UHitbox : public UBoxComponent
{
	GENERATED_BODY()

public:
	UHitbox();
	~UHitbox();

	void AssignHitboxHandler(HitboxHandler* handler);

	UFUNCTION(BlueprintCallable)
	void SetResponder(UHitboxResponder* responder);

	UFUNCTION(BlueprintCallable)
	void ClearCollidedObjects();
	// Sets the collider state to Open
	UFUNCTION(BlueprintCallable)
	void OpenColliderState();
	// Sets the collider state to Closed. Also calls ClearCollidedObjects() to empty the array
	UFUNCTION(BlueprintCallable)
	void CloseColliderState();

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> ReturnCollidedObjects() { return m_CollidedObjects; };

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnRegister() override;

	/*DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitEvent, AActor*, actor);
	UPROPERTY(BlueprintAssignable)
	FOnHitEvent OnHitEvent;*/

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FColor m_OpenStateColor = FColor::Green;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FColor m_CollidingStateColor = FColor::Orange;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FColor m_ClosedStateColor = FColor::Red;

	// Method to draw hitbox in the editor
	void DebugDrawHitbox();

	// Update method of the hitbox to check & handle collision
	void UpdateHitbox();

	virtual void BeginPlay() override;

private:

	HitboxHandler* m_HitboxHandler = nullptr;

	UHitboxResponder* m_Responder = nullptr;

	TArray<AActor*> m_CollidedObjects;

	ColliderState m_ColliderState;
};
