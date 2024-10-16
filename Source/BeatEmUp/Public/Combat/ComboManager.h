// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UBaseState;
enum class EAttackType;

/**
 * 
 */
class BEATEMUP_API ComboManager
{
public:
	ComboManager();
	~ComboManager();

	TArray<TEnumAsByte<UBaseState>> PreviousStates;
	TArray<TEnumAsByte<EAttackType>> PreviousTypes;

	void CountTimer(float time);

	void AddToCombo(EAttackType type);
	void AddToComboUnscaled(EAttackType type);

	void ClearCombo();

	const float AddToComboAndReturnDamage(float damage, EAttackType type);

	const float ReturnScaledDamage(float damage);

	const int ReturnCombo() const;

	//void SetComboMultiplier(float multiplier);

private:

	int CurrentCombo;

	float ComboTimer;
	float CurrentComboMultiplier;
};
