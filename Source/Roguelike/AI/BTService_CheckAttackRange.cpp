// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"

UBTService_CheckAttackRange::UBTService_CheckAttackRange()
{
	NodeName = TEXT("Check Attack Range");

	Interval = 0.2f;
	RandomDeviation = 0.05f;
}

void UBTService_CheckAttackRange::TickNode(
	UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory,
	float DeltaSeconds
)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();

	if (AIController == nullptr)
	{
		return;
	}

	APawn* OwnerPawn = AIController->GetPawn();

	if (OwnerPawn == nullptr)
	{
		return;
	}

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	if (BlackboardComponent == nullptr)
	{
		return;
	}

	AActor* TargetActor = Cast<AActor>(
		BlackboardComponent->GetValueAsObject(TEXT("TargetActor"))
	);

	if (TargetActor == nullptr)
	{
		BlackboardComponent->SetValueAsBool(TEXT("IsInAttackRange"), false);
		return;
	}

	const float Distance = FVector::Dist(
		OwnerPawn->GetActorLocation(),
		TargetActor->GetActorLocation()
	);

	const bool bIsInAttackRange = Distance <= AttackRange;

	BlackboardComponent->SetValueAsBool(
		TEXT("IsInAttackRange"),
		bIsInAttackRange
	);

	//UE_LOG(
	//	LogTemp,
	//	Warning,
	//	TEXT("InAttackRange : %s"),
	//	bIsInAttackRange ? TEXT("True") : TEXT("False")
	//);
}
