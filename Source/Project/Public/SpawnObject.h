// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SwordEnemyObjectPool.h"
#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnObject.generated.h"

UCLASS()
class PROJECT_API ASpawnObject : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnObject();

protected:
	virtual void BeginPlay() override;

public:
	void Spawn();

public:
	UPROPERTY(EditAnywhere, Category = Spawner)
		UBoxComponent* SpawnVolume;

	UPROPERTY(EditAnywhere, Category = Spawner)
		USwordEnemyObjectPool* ObjectPooler;

	UPROPERTY(EditAnywhere, Category = Spawner)
		float SpawnCooltime = 1.2f;

	FTimerHandle TimerHandle;

private:
	UPROPERTY(VisibleAnywhere)
		class UParticleSystem* VolumeParticle;

	UPROPERTY(VisibleAnywhere)
		class UParticleSystem* SpawnParticle;
	
};
