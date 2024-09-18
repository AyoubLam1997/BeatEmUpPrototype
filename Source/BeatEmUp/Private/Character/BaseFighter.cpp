// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseFighter.h"
#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>

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

	RootComponent = CapsuleMesh;

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

	CapsuleMesh = FindComponentByClass<UCapsuleComponent>();
	SkeletalMesh = FindComponentByClass<USkeletalMeshComponent>();

	State = new GroundedState();

	HBHandler = new HitboxHandler();
	Hitbox = FindComponentByClass<UHitbox>();
	Hitbox->AssignHitboxHandler(HBHandler);

	//InitializeController();

	//FighterMesh->OnComponentHit.AddDynamic(this, &ABaseFighter::OnHit);
}

void ABaseFighter::InitializeController()
{
	if (APlayerController* playerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
			SubSystem->AddMappingContext(MappingContext, 0);
		else
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No enhanced input"));
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No controller to control"));
}

// Called every frame
void ABaseFighter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*for (int i = 0; i < BufferHandler->m_InputBufferItems.Num(); i++)
	{
		if (BufferHandler->m_InputBufferItems[i]->InputDirection == EInputType::LightPunch)
		{
			BufferHandler->m_InputBufferItems[i]->m_Buffer[0].SetUsedTrue();
		}
	}*/

	BufferHandler->BufferUpdate();
	BufferHandler->UpdateMotion(0);

	if (State != nullptr)
	{
		State->Update(*this);

		BaseState* state = State->HandleInput(*this);

		if (state != nullptr)
		{
			State->Exit(*this);

			State = nullptr;

			State = state;

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
}

void ABaseFighter::ButtonPressed(const FInputActionValue& value, const int index)
{
	const bool pressed = value.Get<bool>();

	BufferHandler->m_InputBufferItems[index]->SetInputActionPressed(pressed);
}

void ABaseFighter::SetMoveDirection(const FInputActionValue& value)
{
	MoveDirection = value.Get<FVector2D>();
}

void ABaseFighter::ResetMoveDirection(const FInputActionValue& value)
{
	MoveDirection = FVector2D(0, 0);
}

void ABaseFighter::Walk()
{
	FRotator rot = Controller->GetControlRotation();

	FRotator yaw(0.f, rot.Yaw, 0.f);

	FVector forwardDir = FRotationMatrix(yaw).GetUnitAxis(EAxis::X);
	AddMovementInput(forwardDir, MoveDirection.Y);

	FVector rightDir = FRotationMatrix(yaw).GetUnitAxis(EAxis::Y);
	AddMovementInput(rightDir, MoveDirection.X);
}

void ABaseFighter::ChangeState(BaseState* state)
{
	State->Exit(*this);

	State = nullptr;

	State = state;

	State->Enter(*this);
}

BaseState* ABaseFighter::ReturnAttackState()
{
	for (int i = 0; i < ReturnInputBuffer()->m_InputBufferItems.Num(); i++)
	{
		if (ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer.Num() > 0)
		{
			if (ReturnInputBuffer()->m_InputBufferItems[i]->InputDirection == EInputType::LightPunch && ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer[0].HoldTime > 0)
			{
				if (ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer[0].CanExecute())
				{
					ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer[0].SetUsedTrue();
					return LightAttack;
				}
			}
		}
	}

	return nullptr;
}

void ABaseFighter::ChangeToStunState()
{
	ChangeState(new StunState());
}

void ABaseFighter::ChangeToStunStateKnock(FVector dir)
{
	ChangeState(new KnockbackStunState(dir, 60));
}

HitboxHandler* ABaseFighter::ReturnHitboxHandler()
{
	return HBHandler;
}

InputBuffer* ABaseFighter::ReturnInputBuffer()
{
	return BufferHandler;
}
