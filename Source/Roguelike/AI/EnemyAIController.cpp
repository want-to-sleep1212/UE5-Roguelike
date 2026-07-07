// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemyAIController::AEnemyAIController()
{
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTree == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BehaviorTree is nullptr"));
		return;
	}

	RunBehaviorTree(BehaviorTree);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (PlayerPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerPawn is nullptr"));
		return;
	}

	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();

	if (BlackboardComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BlackboardComponent is nullptr"));
		return;
	}

	BlackboardComponent->SetValueAsObject(TEXT("TargetActor"), PlayerPawn);
}
