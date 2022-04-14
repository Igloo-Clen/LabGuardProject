// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindTarget.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "EnemyAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

UBTTask_FindTarget::UBTTask_FindTarget()
{
	NodeName = TEXT("FindTarget");
}

EBTNodeResult::Type UBTTask_FindTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto PlayerPawn = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerPawn != nullptr)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("TargetPos")), PlayerPawn->GetActorLocation());
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
