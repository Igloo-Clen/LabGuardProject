// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHealthChanged);
DECLARE_MULTICAST_DELEGATE(FOnEnemyDeath);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_API UEnemyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyStatComponent();

public:	
	void SetHealth(int32 NewHealth);
	void SetMaxHealth(int32 NewMaxHealth);
	void SetAttack(int32 NewAttack);
	void OnAttacked(float DamageAmount);
	void SetSpeed(float NewSpeed);

	int32 GetHealth() { return Health; }
	int32 GetMaxHealth() { return MaxHealth; }
	float GetHealthRatio() { return Health / (float)MaxHealth; }
	int32 GetAttack() { return Attack; }
	float GetSpeed() { return Speed; }

private:
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 Health;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 MaxHealth;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 Attack;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
		float Speed;

public:
	FOnHealthChanged OnHealthChanged;
	FOnEnemyDeath OnEnemyDeath;
};
