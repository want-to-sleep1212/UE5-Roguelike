#include "Components/Health/HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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
