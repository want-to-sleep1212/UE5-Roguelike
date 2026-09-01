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

	CombatStateOwner = Cast<ICombatStateInterface>(GetOwner());

	if (!CombatStateOwner)
	{
		UE_LOG(LogTemp, Warning, TEXT("CombatComponent Owner does not implement CombatStateInterface"));
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
}


//// Called every frame
//void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

bool UCombatComponent::StartAttack()
{
	if (!CombatStateOwner || !CurrentWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("CombatStateOwner or CurrentWeapon is null"));

		return false;
	}

	if (!CombatStateOwner->CanStartAttack())
	{
		return false;
	}

	if (CombatState != ECombatState::None)
	{
		return false;
	}
	
	CombatState = ECombatState::Attack;

	return true;
}

void UCombatComponent::EndAttack()
{
	if (CurrentWeapon)
	{
		// 몽타주가 AttackEnd Notify 전에 중단된 경우에 공격을 중단하기 위해 호출
		CurrentWeapon->EndAttackWindow();
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

void UCombatComponent::BeginAttackWindow()
{
	if (!CurrentWeapon || CombatState != ECombatState::Attack)
	{
		return;
	}

	CurrentWeapon->BeginAttackWindow();
}

void UCombatComponent::EndAttackWindow()
{
	if (!CurrentWeapon)
	{
		return;
	}

	CurrentWeapon->EndAttackWindow();
}

void UCombatComponent::IncreaseAttackDamage(float Amount)
{
	if (Amount <= 0.0f || !CurrentWeapon)
	{
		return;
	}

	CurrentWeapon->IncreaseAttackDamage(Amount);
}
