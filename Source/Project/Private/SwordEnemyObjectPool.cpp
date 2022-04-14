// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordEnemyObjectPool.h"
#include "EnemyCharacter.h"
#include "GameFramework/Actor.h"
#include "Engine.h"

// Sets default values for this component's properties
USwordEnemyObjectPool::USwordEnemyObjectPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<AEnemyCharacter> SwordEnemy(TEXT("Blueprint'/Game/Blueprints/BP_SwordEnemyCharacter.BP_SwordEnemyCharacter_C'"));
	if (SwordEnemy.Succeeded())
	{
		PooledSwordEnemySubclass = SwordEnemy.Class;
	}
}


AEnemyCharacter* USwordEnemyObjectPool::GetSwordEnemyPooledObject()
{
	for (AEnemyCharacter* SwordEnemyPoolable : SwordEnemyPool)
	{
		if (!SwordEnemyPoolable->IsActive())
		{
			return SwordEnemyPoolable;
		}
	}
	return nullptr;
}

// Called when the game starts
void USwordEnemyObjectPool::BeginPlay()
{
	Super::BeginPlay();

	if (PooledSwordEnemySubclass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			for (int i = 0; i < SwordEnemyPoolSize; i++)
			{
				AEnemyCharacter* SwordEnemyPoolable = GetWorld()->SpawnActor<AEnemyCharacter>(PooledSwordEnemySubclass, FVector(300.f, 300.f, 300.f), FRotator().ZeroRotator);
				SwordEnemyPoolable->SetActive(false);
				SwordEnemyPool.Add(SwordEnemyPoolable);
			}
		}
	}
}
