// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "EnemyCharacter.h"

UEnemyAnimInstance::UEnemyAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_EnemyAttack(TEXT("AnimMontage'/Game/Blueprints/Animations/AM_SwordEnemyAttack.AM_SwordEnemyAttack'"));
	if (AM_EnemyAttack.Succeeded())
	{
		AttackMontage = AM_EnemyAttack.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_EnemyDeath(TEXT("AnimMontage'/Game/Blueprints/Animations/AM_SwordEnemyDeath.AM_SwordEnemyDeath'"));
	if (AM_EnemyDeath.Succeeded())
	{
		DeathMontage = AM_EnemyDeath.Object;
	}
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();

	if (IsValid(Pawn))
	{
		Speed = Pawn->GetVelocity().Size();
	}
}

void UEnemyAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(AttackMontage))
	{
		Montage_Play(AttackMontage, 1.f);
	}
}

void UEnemyAnimInstance::JumpToSection(int32 SectionIndex)
{
	FName Name = GetAttackMontageName(SectionIndex);
	Montage_JumpToSection(Name, AttackMontage);
}

FName UEnemyAnimInstance::GetAttackMontageName(int32 SectionIndex)
{
	return FName(*FString::Printf(TEXT("Attack%d"), SectionIndex));
}

void UEnemyAnimInstance::PlayDeathMontage()
{
	if (!Montage_IsPlaying(DeathMontage))
	{
		Montage_Play(DeathMontage, 1.f);
	}
}

// Enemy Attack HitCheck
void UEnemyAnimInstance::AnimNotify_AttackHit()
{
	AttackHit.Broadcast();
}

void UEnemyAnimInstance::AnimNotify_EnemyDeath()
{
	EnemyDeath.Broadcast();
}
