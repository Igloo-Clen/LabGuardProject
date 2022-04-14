// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerDeath);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_API UPlayerStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerStatComponent();

public:
	void SetAbilityPoint(int32 Point);
	void SetHealth(int32 Hp);
	void SetMaxHealth(int32 MaxHp);
	void SetAttack(int32 AttackDamage);
	void SetAmmo(int32 NewAmmo);
	void SetFireCooltime(float Cooltime);
	void OnAttacked(float DamageAmount);
	void SetWalkSpeed(float NewWalkSpeed);
	void SetSprintSpeed(float NewSprintSpeed);

	float GetFireCooltimeRatio(float Remain);

	int32 GetAbilityPoint() { return AbilityPoint; }
	int32 GetHealth() { return Health; }
	int32 GetMaxHealth() { return MaxHealth; }
	int32 GetAttack() { return Attack; }
	int32 GetAmmo() { return Ammo; }
	int32 GetMaxAmmo() { return MaxAmmo; }
	float GetFireCooltime() { return FireCooltime; }
	float GetWalkSpeed() { return WalkSpeed; }
	float GetSprintSpeed() { return SprintSpeed; }

private:
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 AbilityPoint;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 Health;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 MaxHealth;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 Attack;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 Ammo;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
		int32 MaxAmmo;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
		float FireCooltime;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
		float WalkSpeed;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
		float SprintSpeed;

public:
	FOnPlayerDeath OnPlayerDeath;
};
