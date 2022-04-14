// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FAttackHit);
DECLARE_MULTICAST_DELEGATE(FEnemyDeath);
/**
 * 
 */
UCLASS()
class PROJECT_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UEnemyAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();
	void JumpToSection(int32 SectionIndex);
	FName GetAttackMontageName(int32 SectionIndex);

	void PlayDeathMontage();

	UAnimMontage* GetAttackMontage() { return AttackMontage; }
	UAnimMontage* GetDeathMontage() { return DeathMontage; }
private:
	UFUNCTION()
		void AnimNotify_AttackHit();

	UFUNCTION()
		void AnimNotify_EnemyDeath();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		UAnimMontage* DeathMontage;

public:
	FAttackHit AttackHit;
	FEnemyDeath EnemyDeath;
};
