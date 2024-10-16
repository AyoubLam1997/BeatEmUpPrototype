// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseFighter.h"
#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>

// Sets default values
ABaseFighter::ABaseFighter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	/*comp = CreateDefaultSubobject<USceneComponent>(TEXT("Comp"));
	RootComponent = comp;*/

	CapsuleMesh = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleMesh"));
	//RootComponent = comp;
	//FighterMesh->SetupAttachment(comp);

	SetRootComponent(CapsuleMesh);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(CapsuleMesh);
	//SkeletalMesh->AddLocalOffset(FVector(0, 0, 0 - (FighterMesh->GetUnscaledBoxExtent().Z / 2.f)));

	//MovementPawn = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	//MovementPawn->UpdatedComponent = CapsuleMesh;

	BufferHandler = new InputBuffer();
}

// Called when the game starts or when spawned
void ABaseFighter::BeginPlay()
{
	Super::BeginPlay();

	//CapsuleMesh = FindComponentByClass<UCapsuleComponent>();
	//SkeletalMesh = FindComponentByClass<USkeletalMeshComponent>();

	State = NewObject <UGroundedState>();
	State->AddToRoot();
	State->Enter(*this);

	HBHandler = new HitboxHandler();
	Hitbox = FindComponentByClass<UHitbox>();
	Hitbox->AssignHitboxHandler(HBHandler);

	MovementPawn = FindComponentByClass<UFloatingPawnMovement>();

	CurrentHealth = MaxHealth;

	LockToTarget = 0;

	//InitializeController();

	//FighterMesh->OnComponentHit.AddDynamic(this, &ABaseFighter::OnHit);
}

void ABaseFighter::InitializeController()
{
	if (APlayerController* playerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(MappingContext, 0);

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Input map context set to controller"));
		}
		else
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No enhanced input"));
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No controller to control"));
}

void ABaseFighter::InitializeAIController()
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.OverrideLevel = GetLevel();
	SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save AI controllers into a map
	AController* NewController = GetWorld()->SpawnActor<AController>(Cont, GetActorLocation(), GetActorRotation(), SpawnInfo);
	if (NewController != NULL)
	{
		// if successful will result in setting this->Controller 
		// as part of possession mechanics
		NewController->Possess(this);
	}

	ControlState = EControlState::AI;

	for (int i = 0; i < MappingContext->GetMappings().Num(); i++)
	{
		InputBufferItem* item = new InputBufferItem();

		EInputType input = InputFromString(MappingContext->GetMappings()[i].Action.GetName());

		if (input != EInputType::None)
		{
			item->AssignDirection(input);

			BufferHandler->m_InputBufferItems.Add(item);
		}
	}
}

// Called every frame
void ABaseFighter::Tick(float DeltaTime)
{
	if (SlowMotionTime > 0.f && SlowMotion == 1)
		SlowMotionTime --;
	else if (SlowMotionTime <= 0.f && SlowMotion == 1)
		SetPhysicsNormal();

	if (ControlState == EControlState::Player)
	{
		ComboManagerHandler.CountTimer(DeltaTime);
	}

	/*if (ControlState == EControlState::AI)
	{
		for (int i = 0; i < BufferHandler->m_InputBufferItems.Num(); i++)
		{
			if (BufferHandler->m_InputBufferItems[i]->m_Buffer.Num() > 0)
			{
				if (BufferHandler->m_InputBufferItems[i]->InputDirection == EInputType::LightPunch)
				{
					if (BufferHandler->m_InputBufferItems[i]->m_InputActionPressed == 0)
					{
						BufferHandler->m_InputBufferItems[i]->SetInputActionPressed(1);
					}
					else if (BufferHandler->m_InputBufferItems[i]->m_InputActionPressed == 1)
					{
						BufferHandler->m_InputBufferItems[i]->SetInputActionPressed(0);
					}
				}
			}
		}
	}*/

	Super::Tick(DeltaTime);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(IsGrounded()));

	/*for (int i = 0; i < BufferHandler->m_InputBufferItems.Num(); i++)
	{
		if (BufferHandler->m_InputBufferItems[i]->InputDirection == EInputType::LightPunch)
		{
			BufferHandler->m_InputBufferItems[i]->m_Buffer[0].SetUsedTrue();
		}
	}*/

	//FQuat q = FQuat(FRotator(0, 0, 180));

	//AddActorWorldRotation(q);

	BufferHandler->BufferUpdate();
	BufferHandler->UpdateMotion(0);

	if (IsValid(State))
	{
		State->Update(*this);

		UBaseState* newState = State->HandleInput(*this);

		if (IsValid(newState))
		{
			State->Exit(*this);

			State->RemoveFromRoot();

			UBaseState* stateToDestroy = State;
			State = nullptr;
			State = newState;

			State->AddToRoot();
			State->Enter(*this);
		}
	}
}

// Called to bind functionality to input
void ABaseFighter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInput->BindAction(MovementInput, ETriggerEvent::Triggered, this, &ABaseFighter::SetMoveDirection);
	EnhancedInput->BindAction(MovementInput, ETriggerEvent::Completed, this, &ABaseFighter::ResetMoveDirection);

	for (int i = 0; i < MappingContext->GetMappings().Num(); i++)
	{
		InputBufferItem* item = new InputBufferItem();

		EInputType input = InputFromString(MappingContext->GetMappings()[i].Action.GetName());

		if (input != EInputType::None)
		{
			item->AssignDirection(input);

			BufferHandler->m_InputBufferItems.Add(item);

			EnhancedInput->BindAction(MappingContext->GetMappings()[i].Action, ETriggerEvent::Triggered, this, &ABaseFighter::ButtonPressed, BufferHandler->m_InputBufferItems.Num() - 1);
			EnhancedInput->BindAction(MappingContext->GetMappings()[i].Action, ETriggerEvent::Completed, this, &ABaseFighter::ButtonPressed, BufferHandler->m_InputBufferItems.Num() - 1);
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::FromInt(BufferHandler->m_InputBufferItems.Num()));

	EnhancedInput->BindAction(LockInput, ETriggerEvent::Started, this, &ABaseFighter::SetLockMode);
}

void ABaseFighter::ButtonPressed(const FInputActionValue& value, const int index)
{
	const bool pressed = value.Get<bool>();

	BufferHandler->m_InputBufferItems[index]->SetInputActionPressed(pressed);
}

void ABaseFighter::SetLockMode(const FInputActionValue& value)
{
	LockToTarget = !LockToTarget;
}

void ABaseFighter::SetMoveDirection(const FInputActionValue& value)
{
	MoveDirection = value.Get<FVector2D>();
}

void ABaseFighter::ResetMoveDirection(const FInputActionValue& value)
{
	MoveDirection = FVector2D(0, 0);
}

void ABaseFighter::SetMoveDirection(FVector dir)
{
	if (ControlState == EControlState::AI)
		MoveDirection = FVector2D(dir.X, dir.Y);
}

void ABaseFighter::Walk()
{
	if (ControlState == EControlState::AI)
	{
		CapsuleMesh->SetPhysicsLinearVelocity(FVector(MoveDirection.X * 100, MoveDirection.Y * 100, CapsuleMesh->GetPhysicsLinearVelocity().Z));

		if (MoveDirection.Length() != 0)
		{
			FRotator rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetActorLocation() + CapsuleMesh->GetPhysicsLinearVelocity());

			FRotator dir = (GetActorLocation() - (GetActorLocation() + FVector(-MoveDirection.X, MoveDirection.Y, 0))).Rotation();

			SkeletalMesh->SetWorldRotation(FRotator(0, rot.Yaw - 90, 0));
		}

		return;
	}

	if(LockToTarget)
	{
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetActorLocation() + LocToRotateTowards);

		FRotator dir = (GetActorLocation() - ( FVector(-LocToRotateTowards.X, LocToRotateTowards.Y, LocToRotateTowards.Z))).Rotation();

		SkeletalMesh->SetWorldRotation(FRotator(0, rot.Yaw - 90, 0));
	}

	FRotator rot = GetActorRotation();

	FRotator yaw(0.f, rot.Yaw, 0.f);

	FVector forwardDir = FRotationMatrix(yaw).GetUnitAxis(EAxis::X);
	AddMovementInput(FVector(.75f, 0, 0), MoveDirection.Y);

	FVector rightDir = FRotationMatrix(yaw).GetUnitAxis(EAxis::Y);
	AddMovementInput(FVector(0, .75f, 0), MoveDirection.X);

	if (MoveDirection.Length() != 0 && LockToTarget == 0)
	{
		FRotator dir = (GetActorLocation() - (GetActorLocation() + FVector(-MoveDirection.X, MoveDirection.Y, 0))).Rotation();

		SkeletalMesh->SetWorldRotation(dir + FVector(0, 0, 0).Rotation());
	}

	/*FVector forwardDir = FRotationMatrix(yaw).GetUnitAxis(EAxis::X);
	AddMovementInput(forwardDir, MoveDirection.Y);

	FVector rightDir = FRotationMatrix(yaw).GetUnitAxis(EAxis::Y);
	AddMovementInput(rightDir, MoveDirection.X);*/
}

void ABaseFighter::RotateToInputDirection()
{
	if (MoveDirection.Length() != 0 && LockToTarget == 0)
	{
		FRotator dir = (GetActorLocation() - (GetActorLocation() + FVector(-MoveDirection.X, MoveDirection.Y, 0))).Rotation();

		SkeletalMesh->SetWorldRotation(dir + FVector(0, 0, 0).Rotation());
	}
}

const float ABaseFighter::AddToCombo(float damage, TEnumAsByte<EAttackType> type)
{
	return ComboManagerHandler.AddToComboAndReturnDamage(damage, type);
}

const int ABaseFighter::ReturnCurrentCombo()
{
	return ComboManagerHandler.ReturnCombo();
}

void ABaseFighter::ChangeState(UBaseState* newState)
{
	State->Exit(*this);

	State->StateChange = nullptr;

	UBaseState* stateToDestroy = State;
	State = nullptr;
	State = newState;

	State->Enter(*this);
}

void ABaseFighter::ChangeStateReference(TSubclassOf <UBaseState> newState)
{
	UBaseState* obj = newState.GetDefaultObject();
	
	State->StateChange = obj;
}

UBaseState* ABaseFighter::ReturnAttackState()
{
	if (InputCheck(EInputType::LightPunch))
		return DuplicateObject(LightAttack.GetDefaultObject(), nullptr);
	if (InputCheck(EInputType::MediumPunch))
		return DuplicateObject(MediumAttack.GetDefaultObject(), nullptr);
	if (InputCheck(EInputType::HeavyPunch))
		return NewObject<UJumpState>();
	for (int i = 0; i < ReturnInputBuffer()->m_InputBufferItems.Num(); i++)
	{
		if (ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer.Num() > 0)
		{
			if (ReturnInputBuffer()->m_InputBufferItems[i]->InputDirection == EInputType::Block && ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer[0].HoldTime > 0)
			{
				return NewObject<UBlockState>();
			}
		}
	}

	/*if (InputCheck(EInputType::HeavyPunch))
		return NewObject<UJumpState>();*/

	return nullptr;
}

bool ABaseFighter::InputCheck(EInputType input)
{
	for (int i = 0; i < ReturnInputBuffer()->m_InputBufferItems.Num(); i++)
	{
		if (ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer.Num() > 0)
		{
			if (ReturnInputBuffer()->m_InputBufferItems[i]->InputDirection == input)
			{
				//DEV NOTE: 0 is the unnused state for the buffer. Start with 1 for input buffer check
				for (int j = 1; j < ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer.Num(); j++)
				{
					if (ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer[j].CanExecute())
					{
						ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer[j].SetUsedTrue();

						// For debugging purposes
						int value = BufferHandler->m_InputBufferItems[i]->m_Buffer[j].HoldTime;
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Entering attack state in buffer ") + InputToString(input) + ": Index " + FString::FromInt(j));

						return 1;
					}
				}
			}
			else
				continue;
		}
	}

	return 0;
}

const bool ABaseFighter::IsGrounded()
{
	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = GetActorLocation() + FVector(0, 0, -1.f) * 50.f;

	// You can use FCollisionQueryParams to further configure the query
	// Here we add ourselves to the ignored list so we won't block the trace
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FHitResult Hit;

	ECollisionChannel TraceChannelProperty = ECollisionChannel::ECC_Visibility;

	// To run the query, you need a pointer to the current level, which you can get from an Actor with GetWorld()
	// UWorld()->LineTraceSingleByChannel runs a line trace and returns the first actor hit over the provided collision channel.
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannelProperty, QueryParams);

	if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
		return 1;

	return 0;
}

UBaseState* ABaseFighter::CancelToState(EInputType input, TSubclassOf<UBaseState> newState)
{
	if(InputCheck(input))
		return DuplicateObject(newState.GetDefaultObject(), nullptr);

	return nullptr;
}

void ABaseFighter::SetPhysicsSlowMotion()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Setting global slow motion"));
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), .1f);

	SlowMotionTime = 15.f;
	SlowMotion = 1;
}

void ABaseFighter::SetLocalSlowMotion()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Setting local slow motion"));
	CustomTimeDilation = .05f;

	SlowMotionTime = 30.f;
	SlowMotion = 1;
}

void ABaseFighter::SetPhysicsNormal()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	CustomTimeDilation = 1.f;

	SlowMotion = 0;
	SlowMotionTime = 0.f;
}

void ABaseFighter::ChangeToGroundedState()
{
	ChangeState(NewObject<UGroundedState>());
}

void ABaseFighter::ChangeToStunState()
{
	if(UBlockState* block = Cast<UBlockState>(State))
	{
		if(block->BlockTime <= 3)
			ChangeState(NewObject<UParryState>());
		else
			ChangeState(NewObject<UBlockingState>());

		return;
	}

	ChangeState(NewObject<UStunState>());
}

void ABaseFighter::ChangeToStunStateKnock(FVector dir)
{
	if (UBlockState* block = Cast<UBlockState>(State))
	{
		if (block->BlockTime <= 3)
			ChangeState(NewObject<UParryState>());
		else
			ChangeState(NewObject<UBlockingState>());

		return;
	}

	UKnockbackStunState* state = NewObject<UKnockbackStunState>();

	state->Init(dir, 60);

	ChangeState(state);
}

void ABaseFighter::ChangeToStunStateAir(FVector dir)
{
	if (UBlockState* block = Cast<UBlockState>(State))
	{
		if (block->BlockTime <= 3)
			ChangeState(NewObject<UParryState>());
		else
			ChangeState(NewObject<UBlockingState>());

		return;
	}

	UAirStunState* state = NewObject<UAirStunState>();

	state->Init(dir, 60);

	ChangeState(state);
}

HitboxHandler* ABaseFighter::ReturnHitboxHandler()
{
	return HBHandler;
}

InputBuffer* ABaseFighter::ReturnInputBuffer()
{
	return BufferHandler;
}

const bool ABaseFighter::HasHitEnemy()
{
	if (HBHandler->ReturnCollidedActors().Num() > 0)
	{

		return 1;
	}

	return 0;
}

const FVector2D ABaseFighter::ReturnMoveInput() const
{
	return MoveDirection;
}

UCapsuleComponent* ABaseFighter::ReturnCapsuleMesh() const
{
	return CapsuleMesh;
}

USkeletalMeshComponent* ABaseFighter::ReturnSkeletalMesh() const
{
	return SkeletalMesh;
}

UHitbox* ABaseFighter::ReturnHitbox() const
{
	return Hitbox;
}

const bool ABaseFighter::IsBlocking()
{
	if (UBlockState* block = Cast<UBlockState>(State))
		return 1;
	if (UParryState* parry = Cast<UParryState>(State))
		return 1;

	return 0;
}

void ABaseFighter::RemoveHealth(float value)
{
	if (value <= 0)
		return;

	CurrentHealth -= value;
}

const bool ABaseFighter::IsDead() const
{
	return CurrentHealth <= 0 ? 1 : 0;
}

void ABaseFighter::SetLookAtRotation(FVector look)
{
	LocToRotateTowards = look;
}

UBaseState* ABaseFighter::GetCurrentState()
{
	return State;
}

UAttackState* ABaseFighter::GetAttackState()
{
	if (UAttackState* attack = Cast<UAttackState>(State))
		return attack;

	return nullptr;
}

void ABaseFighter::InitializeAIControllerBlueprint_Implementation()
{

}
