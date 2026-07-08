// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnded);

UCLASS()
class ROGUELIKE_API AWeaponActor : public AActor
{
	GENERATED_BODY()

public:
	AWeaponActor();

	void StartAttack();
	void EndAttack();

public:
	UPROPERTY(BlueprintAssignable)
	FOnAttackEnded OnAttackEnded;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnWeaponHitBoxOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	void EnableHitBox();
	void DisableHitBox();

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* WeaponHitBox;

	UPROPERTY(EditDefaultsOnly)
	float AttackDamage = 10.f;

	UPROPERTY(EditDefaultsOnly)
	float AttackDuration = 0.5f;

	FTimerHandle AttackTimerHandle;

	bool bIsAttacking = false;

	UPROPERTY()
	TSet<AActor*> HitActors;
};
