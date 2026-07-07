// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckAttackRange.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ROGUELIKE_API UBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_CheckAttackRange();

protected:
	virtual void TickNode(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory,
		float DeltaSeconds
	) override;

private:
	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRange = 150.0f;
};
