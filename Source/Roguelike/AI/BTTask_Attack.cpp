// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"

#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/Pawn.h"
#include "Characters/Enemy/EnemyCharacter.h"
#include "Components/Combat/CombatComponent.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory
)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AIController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController is null"));
		return EBTNodeResult::Failed;
	}

	APawn* OwnerPawn = AIController->GetPawn();

	if (OwnerPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController Pawn is null"));
		return EBTNodeResult::Failed;
	}

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (Enemy == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyCharacter cast failed"));
		return EBTNodeResult::Failed;
	}

	if (!Enemy->CanStartAttack())
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy can't StartAttack"));
		return EBTNodeResult::Failed;
	}

	UCombatComponent* CombatComponent = Enemy->GetCombatComponent();

	if (!CombatComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("CombatComponent is null"));
		return EBTNodeResult::Failed;
	}

	AIController->StopMovement();

	Enemy->Attack();

	return EBTNodeResult::Succeeded;
}
