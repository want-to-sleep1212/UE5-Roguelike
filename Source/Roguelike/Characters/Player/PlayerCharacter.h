// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Core/Types/CharacterStates.h"
#include "Core/Types/StatusEffectTypes.h"

#include "InputActionValue.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Core/Interfaces/CombatStateInterface.h"

#include "PlayerCharacter.generated.h"

class UCombatComponent;
class AWeaponActor;
class UHealthComponent;
class UAnimMontage;
class UDataTable;

class UInputMappingContext;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnGoldChanged,
	int32, CurrentGold
);

UCLASS()
class ROGUELIKE_API APlayerCharacter
	: public ACharacter
	, public ICombatStateInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// damage
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	// combat
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void BeginAttackWindow();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EndAttackWindow();

	virtual bool CanStartAttack() const override;

	UFUNCTION()
	void OnAttackMontageEnded(
		UAnimMontage* Montage,
		bool bInterrupted
	);

	// gold
	void AddGold(int32 Amount);
	bool SpendGold(int32 Amount);

	// getter
	UHealthComponent* GetHealthComponent() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetCurrentHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	EMovementState GetMovementState() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	EControlState GetControlState() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	ELifeState GetLifeState() const;

	int32 GetGold() const;

	UCombatComponent* GetCombatComponent() const;

public:
	UPROPERTY(BlueprintAssignable, Category = "Gold")
	FOnGoldChanged OnGoldChanged;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> AttackMontage;

private:
	void MoveInput(const FInputActionValue& Value);
	void MoveInputCompleted(const FInputActionValue& Value);
	void DashInput();
	void AttackInput();
	void LookInput(const FInputActionValue& Value);

	void EndDash();

	void EndInvincible();

	UFUNCTION()
	void Die();

	void ChangeGold(int32 Amount);

private:
	// camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	// input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> IMC_Character;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IA_Dash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IA_Attack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> IA_Look;

	FVector2D MoveInputValue;

	// state
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	EMovementState MovementState = EMovementState::Idle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	EControlState ControlState = EControlState::Normal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	ELifeState LifeState = ELifeState::Alive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StatusEffect", meta = (AllowPrivateAccess = "true"))
	TArray<FStatusEffect> ActiveEffects;

	// movement
	FVector LastMoveDirection = FVector::ForwardVector;

	UPROPERTY(EditAnywhere, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float DashPower = 1800.0f;

	UPROPERTY(EditAnywhere, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float DashDuration = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float DashInvincibleTime = 0.2f;

	FTimerHandle DashTimerHandle;
	FTimerHandle InvincibleTimerHandle;

	// components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCombatComponent> CombatComponent;

	// data
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	TObjectPtr<UDataTable> CharacterStatTable;

	// gold
	UPROPERTY(VisibleAnywhere, Category = "Gold")
	int32 Gold = 50;
};
