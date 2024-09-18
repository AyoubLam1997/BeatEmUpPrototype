// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "InputMappingContext.h"

#include "InputBuffer.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EInputType
{
	None = 0,
	Top,
	TopRight,
	Right,
	DownRight,
	Down,
	DownLeft,
	Left,
	TopLeft,
	LightPunch,
	MediumPunch,
	HeavyPunch,
	LightKick,
	MediumKick,
	HeavyKick
};

FString InputToString(EInputType type);

EInputType InputFromString(FString string);

class BEATEMUP_API MotionInput
{
public:

	MotionInput();
	MotionInput(TArray<EInputType> motions);
	~MotionInput() {};

	/*UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EInputType> InputDirection;*/

	TArray<EInputType> MotionInputs;

	int BufferTime;

	int IndexCheck;

	bool MotionUsed;

	void BufferCheck();
	void IncreaseIndex();
	void ResetInput();

	bool MotionComplete();
	bool InputCheck(EInputType input);
};

class BEATEMUP_API InputStateItem
{
public:

	bool CanExecute();
	bool CanMotionExecute();

	void HoldUp();
	void ReleasedUp();
	void SetHoldUsed(int time, bool used, bool motion);
	void SetUsedTrue();
	void SetMotionTrue();

	int HoldTime = 0;

	bool IsUsed = 0;
	bool MotionUsed = 0;
};

class BEATEMUP_API InputBufferItem
{
public:

	InputBufferItem();
	InputBufferItem(EInputType direction);
	~InputBufferItem() {};

	void AssignDirection(EInputType direction);
	void SetHoldUsed(int index, int time, bool used, bool motion);

	void InputCheck();

	void SetUsedTrue();
	void SetUsedFalse();

	void SetInputActionPressed(bool pressed);

	EInputType InputDirection;

	UInputAction* m_Action;

	bool m_Used = 0;

	bool m_InputActionPressed = 0;

	TArray<InputStateItem> m_Buffer;

protected:

};

class BEATEMUP_API InputBuffer
{
public:

	static const int m_BufferWindow = 12;

	InputBuffer();
	InputBuffer(TArray<MotionInput*>& inputs);
	~InputBuffer();

	TArray<InputBufferItem*> m_InputBufferItems;
	//TArray<InputAction*> m_InputActions;
	TArray<MotionInput*> m_MotionInputs;

	void Initialize();
	void BufferUpdate();

	void add(TArray<MotionInput*>& inputs);

	void UpdateMotion(bool right);
};
