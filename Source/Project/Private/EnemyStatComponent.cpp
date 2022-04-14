// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStatComponent.h"

// Sets default values for this component's properties
UEnemyStatComponent::UEnemyStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 40;
	SetHealth(MaxHealth);
	Attack = 30;
	Speed = 650;
}

void UEnemyStatComponent::SetHealth(int32 NewHealth)
{
	Health = NewHealth;
	if (Health <= 0)
	{
		Health = 0;
		OnEnemyDeath.Broadcast();
	}
	OnHealthChanged.Broadcast();
}

void UEnemyStatComponent::SetMaxHealth(int32 NewMaxHealth)
{
	MaxHealth += NewMaxHealth;
}

void UEnemyStatComponent::SetAttack(int32 NewAttack)
{
	Attack += NewAttack;
}

void UEnemyStatComponent::OnAttacked(float DamageAmount)
{
	int32 NewHealth = Health - DamageAmount;
	SetHealth(NewHealth);

	UE_LOG(LogTemp, Warning, TEXT("OnAttacked %d"), Health);
}

void UEnemyStatComponent::SetSpeed(float NewSpeed)
{
	Speed += NewSpeed;
}
