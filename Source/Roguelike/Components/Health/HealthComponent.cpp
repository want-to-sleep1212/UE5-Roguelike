// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Health/HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.0f || bDead)
	{
		return;
	}

	ChangeHealth(HealAmount);
}

void UHealthComponent::ApplyDamage(float DamageAmount)
{
	if (DamageAmount <= 0.0f || bDead)
	{
		return;
	}

	ChangeHealth(-DamageAmount);
}

void UHealthComponent::ChangeHealth(float Amount)
{
	if (Amount == 0.0f || bDead)
	{
		return;
	}

	const float OldHealth = CurrentHealth;

	CurrentHealth = FMath::Clamp(
		CurrentHealth + Amount,
		0.0f,
		MaxHealth
	);

	const float ChangedAmount = CurrentHealth - OldHealth;

	if (FMath::IsNearlyZero(ChangedAmount))
	{
		return;
	}

	OnHealthChanged.Broadcast(
		CurrentHealth,
		MaxHealth,
		ChangedAmount
	);

	if (CurrentHealth <= 0.0f)
	{
		bDead = true;
		OnDeath.Broadcast();
	}
}

float UHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

bool UHealthComponent::IsDead() const
{
	return bDead;
}

void UHealthComponent::InitializeHealth(float InMaxHealth)
{
	MaxHealth = FMath::Max(0.0f, InMaxHealth);
	CurrentHealth = MaxHealth;
	bDead = false;

	OnHealthChanged.Broadcast(
		CurrentHealth,
		MaxHealth,
		0.0f
	);
}

void UHealthComponent::IncreaseMaxHealth(float Amount)
{
	if (Amount <= 0.0f || bDead)
	{
		return;
	}

	MaxHealth += Amount;

	Heal(Amount);
}
