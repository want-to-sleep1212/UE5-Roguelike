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

UCLASS()
class ROGUELIKE_API APlayerCharacter
	: public ACharacter
	, public ICombatStateInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	// getter
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

	virtual bool CanStartAttack() const override;

private:
	// 입력
	void MoveInput(const FInputActionValue& Value);
	void MoveInputCompleted(const FInputActionValue& Value);
	void DashInput();
	void AttackInput();
	void LookInput(const FInputActionValue& Value);

	// 대쉬
	void EndDash();

	// 무적
	void EndInvincible();

	UFUNCTION()
	void Die();

private:
	// 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	// 입력
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* IMC_Character;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Dash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Attack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Look;

	FVector2D MoveInputValue;

	// 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	EMovementState MovementState = EMovementState::Idle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	EControlState ControlState = EControlState::Normal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	ELifeState LifeState = ELifeState::Alive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status", meta = (AllowPrivateAccess = "true"))
	TArray<FStatusEffect> ActiveEffects;

	// 이동
	FVector LastMoveDirection = FVector::ForwardVector;

	// 대쉬
	UPROPERTY(EditAnywhere, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float DashPower = 1800.0f;

	UPROPERTY(EditAnywhere, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float DashDuration = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float DashInvincibleTime = 0.2f;

	FTimerHandle DashTimerHandle;

	// 무적
	FTimerHandle InvincibleTimerHandle;

	// 체력
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;

	// 전투
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UCombatComponent* CombatComponent;
};
