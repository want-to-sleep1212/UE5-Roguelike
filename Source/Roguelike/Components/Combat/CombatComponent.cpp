// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/CombatComponent.h"

#include "GameFramework/Character.h"
#include "Weapons/WeaponActor.h"

#include "Core/Types/CharacterStates.h"

#include "Core/Interfaces/CombatStateInterface.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("CombatComponent Owner is null"));

		return;
	}

	if (!WeaponClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponClass is null"));

		return;
	}

	CurrentWeapon = GetWorld()->SpawnActor<AWeaponActor>(WeaponClass);

	if (!CurrentWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn WeaponActor"));

		return;
	}

	CurrentWeapon->SetOwner(OwnerCharacter);
	CurrentWeapon->SetInstigator(OwnerCharacter);

	CurrentWeapon->AttachToComponent(
		OwnerCharacter->GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale,
		TEXT("RightHand")
	);

	CurrentWeapon->SetActorHiddenInGame(!bWeaponVisible);

	CurrentWeapon->OnAttackEnded.AddDynamic(
		this,
		&UCombatComponent::EndAttack
	);



	CombatStateOwner = Cast<ICombatStateInterface>(GetOwner());

	if (!CombatStateOwner)
	{
		UE_LOG(LogTemp, Warning, TEXT("CombatComponent Owner does not implement CombatStateInterface"));
		return;
	}
}


//// Called every frame
//void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

void UCombatComponent::StartAttack()
{
	if (!CombatStateOwner || !CurrentWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("CombatStateOwner or CurrentWeapon is null"));
		return;
	}

	if (!CombatStateOwner->CanStartAttack())
	{
		UE_LOG(LogTemp, Warning, TEXT("CombatStateOwner can't start attack"));
		return;
	}

	if (CombatState != ECombatState::None)
	{
		//UE_LOG(LogTemp, Warning, TEXT("CombatState is not none"));
		return;
	}
	
	CombatState = ECombatState::Attack;

	CurrentWeapon->StartAttack();
}

void UCombatComponent::EndAttack()
{
	if (!OwnerCharacter)
	{
		return;
	}

	CombatState = ECombatState::None;
}

void UCombatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

void UCombatComponent::SetWeaponVisible(bool bVisible)
{
	bWeaponVisible = bVisible;

	if (!CurrentWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't set WeaponVisiblity. Current weapon is null"));
		return;
	}

	CurrentWeapon->SetActorHiddenInGame(!bWeaponVisible);
}
