// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnemyCharacter.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SwordEnemyObjectPool.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_API USwordEnemyObjectPool : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USwordEnemyObjectPool();

	AEnemyCharacter* GetSwordEnemyPooledObject();
	TArray<AEnemyCharacter*> GetSwordEnemyPool() { return SwordEnemyPool; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, Category = ObjectPooler)
		TSubclassOf<class AEnemyCharacter> PooledSwordEnemySubclass;

	UPROPERTY(EditAnywhere, Category = ObjectPooler)
		int32 SwordEnemyPoolSize = 25;
		
	TArray<AEnemyCharacter*> SwordEnemyPool;
};
