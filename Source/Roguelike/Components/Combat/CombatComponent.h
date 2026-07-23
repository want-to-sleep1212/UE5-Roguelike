// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/Types/CharacterStates.h"
#include "CombatComponent.generated.h"

class ACharacter;
class AWeaponActor;
class ICombatStateInterface;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUELIKE_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

//public:	
//	// Called every frame
//	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void StartAttack();

	void SetWeaponVisible(bool bVisible);

private:
	UFUNCTION()
	void EndAttack();

	void InitializeWeapon();
	void EquipWeapon(AWeaponActor* NewWeapon);

private:
	UPROPERTY()
	ACharacter* OwnerCharacter;

	// 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState = ECombatState::None;

	// 공격
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float AttackDamage = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float AttackDuration = 0.3f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float AttackCooldown = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float AttackPowerBonus = 0.0f;

	bool bIsAttacking = false;

	FTimerHandle AttackTimerHandle;

	// 무기
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeaponActor> WeaponClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	AWeaponActor* CurrentWeapon;

	bool bWeaponVisible = true;

	// 인터페이스
	ICombatStateInterface* CombatStateOwner;
};
