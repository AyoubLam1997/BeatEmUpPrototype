// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/ComboManager.h"
#include "Character/States/BaseStates.h"

ComboManager::ComboManager()
{
	CurrentCombo = 0;
	ComboTimer = 0;

	CurrentComboMultiplier = 1.f;
}

ComboManager::~ComboManager()
{
}

void ComboManager::CountTimer(float delta)
{
	if (ComboTimer <= 0)
		return;

	ComboTimer -= delta;

	if (ComboTimer <= 0)
	{
		ClearCombo();
	}
}

void ComboManager::AddToCombo(EAttackType type)
{
	if (PreviousTypes.Num() > 0)
	{
		if (PreviousTypes.Num() > 0)
		{
			if (PreviousTypes[PreviousTypes.Num() - 1] == type)
			{
				CurrentComboMultiplier -= .2f;
			}
			else
				CurrentComboMultiplier -= .05f;
		}
		else
			CurrentComboMultiplier -= .05f;
	}

	if (CurrentComboMultiplier <= .1f)
		CurrentComboMultiplier = .1f;

	CurrentCombo += 1;
	ComboTimer = 10.f;

	PreviousTypes.Add(type);
}

void ComboManager::AddToComboUnscaled(EAttackType type)
{
	PreviousTypes.Add(type);
}

const float ComboManager::AddToComboAndReturnDamage(float damage, EAttackType type)
{
	AddToCombo(type);

	return ReturnScaledDamage(damage);
}

const int ComboManager::ReturnCombo() const
{
	return CurrentCombo;
}

void ComboManager::ClearCombo()
{
	PreviousTypes.Empty();
	CurrentCombo = 0;
	ComboTimer = 0;
	CurrentComboMultiplier = 1.f;
}

const float ComboManager::ReturnScaledDamage(float damage)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(damage * CurrentComboMultiplier));
	return damage * CurrentComboMultiplier;
}
