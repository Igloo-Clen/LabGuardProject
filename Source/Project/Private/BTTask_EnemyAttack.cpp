// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EnemyAttack.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"

UBTTask_EnemyAttack::UBTTask_EnemyAttack()
{
	// TickTask 실행
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_EnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto CurrentPawn = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (CurrentPawn == nullptr)
		return EBTNodeResult::Failed;

	CurrentPawn->Attack();
	bIsAttacking = true;

	CurrentPawn->AttackEnd.AddLambda([this]()
		{
			bIsAttacking = false;
		});

	return Result;
}

void UBTTask_EnemyAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (bIsAttacking == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
