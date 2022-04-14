// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectPool.h"
#include "UObject/ConstructorHelpers.h"
#include "EnemyCharacter.h"
#include "GameFramework/Actor.h"
#include "Engine.h"

UObjectPool::UObjectPool()
{
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<AProjectile> TEMP(TEXT("Blueprint'/Game/Blueprints/BP_Projectile.BP_Projectile_C'"));
	if (TEMP.Succeeded())
	{
		PooledObjectSubclass = TEMP.Class;
	}
}


AProjectile* UObjectPool::GetPooledObject()
{
	for (AProjectile* Poolable : Pool)
	{
		if (!Poolable->IsActive())
		{
			return Poolable;
		}
	}
	return nullptr;
}

void UObjectPool::BeginPlay()
{
	Super::BeginPlay();

	UWorld*  const World = GetWorld();

	if (PooledObjectSubclass != NULL)
	{
		if (World)
		{
			for (int i = 0; i < PoolSize; i++)
			{
				AProjectile* Poolable = GetWorld()->SpawnActor<AProjectile>(PooledObjectSubclass, FVector().ZeroVector, FRotator().ZeroRotator);
				Poolable->SetActive(false);
				Pool.Add(Poolable);
			}
		}
	}

	
}
