// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Projectile.h"
#include "EnemyCharacter.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectPool.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_API UObjectPool : public UActorComponent
{
	GENERATED_BODY()

public:	
	UObjectPool();

	// Projectile Get Pooled Objects
	AProjectile* GetPooledObject();

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, Category = ObjectPooler)
		TSubclassOf<class AProjectile> PooledObjectSubclass;

	UPROPERTY(EditAnywhere, Category = ObjectPooler)
		int32 PoolSize = 12;

	// Projectile Pool
	TArray<AProjectile*> Pool;
	
};
