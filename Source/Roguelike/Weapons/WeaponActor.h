// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class ROGUELIKE_API AWeaponActor : public AActor
{
	GENERATED_BODY()

public:
	AWeaponActor();

	void BeginAttackWindow();
	void EndAttackWindow();

	void IncreaseAttackDamage(float Amount);

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

	bool bIsAttacking = false;

	UPROPERTY()
	TSet<AActor*> HitActors;
};
