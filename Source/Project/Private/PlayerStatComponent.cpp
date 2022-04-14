// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatComponent.h"

// Sets default values for this component's properties
UPlayerStatComponent::UPlayerStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// Default
	AbilityPoint = 0;

	MaxHealth = 200;
	Health = MaxHealth;

	Attack = 20;
	MaxAmmo = 10;
	Ammo = MaxAmmo;
	FireCooltime = 2.f;

	WalkSpeed = 500.f;
	SprintSpeed = 800.f;
}

void UPlayerStatComponent::SetAbilityPoint(int32 Point)
{
	AbilityPoint += Point;
}

void UPlayerStatComponent::SetHealth(int32 Hp)
{
	Health += Hp;
	if (Health >= MaxHealth)
		Health = MaxHealth;
}

void UPlayerStatComponent::SetMaxHealth(int32 MaxHp)
{
	MaxHealth += MaxHp;
}

void UPlayerStatComponent::SetAttack(int32 AttackDamage)
{
	Attack += AttackDamage;
}

void UPlayerStatComponent::SetAmmo(int32 NewAmmo)
{
	Ammo = NewAmmo;
}

void UPlayerStatComponent::SetFireCooltime(float Cooltime)
{
	FireCooltime -= Cooltime;
}

void UPlayerStatComponent::OnAttacked(float DamageAmount)
{
	Health -= DamageAmount;
	if (Health <= 0)
	{
		Health = 0;
		OnPlayerDeath.Broadcast();
	}
		

	UE_LOG(LogTemp, Warning, TEXT("OnAttacked %d"), Health);
}

void UPlayerStatComponent::SetWalkSpeed(float NewWalkSpeed)
{
	WalkSpeed += NewWalkSpeed;
}

void UPlayerStatComponent::SetSprintSpeed(float NewSprintSpeed)
{
	SprintSpeed = NewSprintSpeed;
}

float UPlayerStatComponent::GetFireCooltimeRatio(float Remain)
{
	return Remain / FireCooltime;
}
