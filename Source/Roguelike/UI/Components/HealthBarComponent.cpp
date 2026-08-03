// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Components/HealthBarComponent.h"

#include "Components/Health/HealthComponent.h"
#include "Blueprint/UserWidget.h"

#include "UI/Widgets/HealthBarWidget.h"

UHealthBarComponent::UHealthBarComponent()
{
	SetWidgetSpace(EWidgetSpace::Screen);
	SetDrawSize(FVector2D(120.0f, 12.0f));
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UHealthBarComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (!IsValid(Owner))
	{
		return;
	}

	HealthComponent = Owner->FindComponentByClass<UHealthComponent>();

	if (!IsValid(HealthComponent))
	{
		return;
	}

	HealthComponent->OnHealthChanged.AddDynamic(
		this,
		&UHealthBarComponent::HandleHealthChanged
	);

	HandleHealthChanged(
		HealthComponent->GetCurrentHealth(),
		HealthComponent->GetMaxHealth(),
		0.0f
	);
}

void UHealthBarComponent::HandleHealthChanged(
	float CurrentHealth,
	float MaxHealth,
	float ChangedAmount
)
{
	UHealthBarWidget* HealthBarWidget = Cast<UHealthBarWidget>(GetUserWidgetObject());

	if (!IsValid(HealthBarWidget))
	{
		return;
	}

	HealthBarWidget->UpdateHealth(CurrentHealth, MaxHealth);
}
