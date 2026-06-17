// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUELIKE_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void StartAttack();
	void EndAttack();
	void HandleHit(AActor* HitActor);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BaseDamage = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float AttackRange = 100.f;

private:
	bool bIsAttacking = false;

	TSet<AActor*> HitActors;

};
