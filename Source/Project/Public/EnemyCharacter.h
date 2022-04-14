// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnemyAIController.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FAttackEnd);

UCLASS()
class PROJECT_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

public:
	// Object Pooling
	void SetActive(bool InActive);
	bool IsActive();

protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Attack();
	void AttackCheck();

	void Death();
	AEnemyAIController* GetAIController();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void HpBarVisibility(bool bActive);

	UFUNCTION()
		void MontageEnded(UAnimMontage* Montage, bool bInterrupted);

	FAttackEnd AttackEnd;

	UPROPERTY(VisibleAnywhere)
		class UEnemyStatComponent* Stat;

	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* HpBar;

	bool IsDead = false;

protected:
	bool Active;
	void Deactivate();

private:
	class UEnemyAnimInstance* AnimInstance;

	UPROPERTY()
		int32 AttackIndex = 0;
};
