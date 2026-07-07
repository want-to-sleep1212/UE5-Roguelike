// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Core/Types/CharacterStates.h"

#include "EnemyCharacter.generated.h"

class AEnemyCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnEnemyDead,
	AEnemyCharacter*,
	Enemy
);

UCLASS()
class ROGUELIKE_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser
	) override;

	void Attack();

public:
	UPROPERTY(BlueprintAssignable)
	FOnEnemyDead OnEnemyDead;

protected:
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Die();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float MaxHealth = 30.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	float CurrentHealth;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	ELifeState LifeState = ELifeState::Alive;

	UPROPERTY(EditDefaultsOnly, Category = "Drop", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> DropItemClass;

	UPROPERTY(EditDefaultsOnly, Category = "Drop", meta = (ClampMin = "0.0", ClampMax = "1.0", AllowPrivateAccess = "true"))
	float DropChance = 0.3f;

private:
	void TryDropItem();
};
