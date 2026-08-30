// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Core/Types/CharacterStates.h"
#include "Core/Interfaces/CombatStateInterface.h"

#include "EnemyCharacter.generated.h"

class AEnemyCharacter;
class UCombatComponent;
class UHealthComponent;
class UHealthBarComponent;
class UAnimMontage;
class UDataTable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnEnemyDead,
	AEnemyCharacter*,
	Enemy
);

UCLASS()
class ROGUELIKE_API AEnemyCharacter
	: public ACharacter
	, public ICombatStateInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

public:
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser
	) override;

	void Attack();

	virtual bool CanStartAttack() const override;

	UCombatComponent* GetCombatComponent() const;

	void Activate();
	void Deactivate();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void BeginAttackWindow();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EndAttackWindow();

public:
	UPROPERTY(BlueprintAssignable)
	FOnEnemyDead OnEnemyDead;

protected:
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Die();

	void OnAttackMontageEnded(
		UAnimMontage* Montage,
		bool bInterrupted
	);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	TObjectPtr<UDataTable> CharacterStatTable;

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	FName StatRowName;

private:
	void TryDropItem();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	ELifeState LifeState = ELifeState::Alive;

	UPROPERTY(EditDefaultsOnly, Category = "Drop", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> DropItemClass;

	UPROPERTY(EditDefaultsOnly, Category = "Drop", meta = (ClampMin = "0.0", ClampMax = "1.0", AllowPrivateAccess = "true"))
	float DropChance = 0.3f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCombatComponent> CombatComponent;

	// 블루프린트 참조 문제로 이름 수정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHealthComponent> CharacterHealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHealthBarComponent> HealthBarComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> AttackMontage;
};
