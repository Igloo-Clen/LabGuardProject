// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ObjectPool.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class PROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public: 
	// UI
	void RefreshUI();

	// Action Bind
	void OnFire();
	void FireCooltime();
	void Reload();
	void Status();

	void StartSprint();
	void StopSprint();

	void Death();

	// Status
	bool IsFiring = false;
	bool IsSprinting = false;
	bool IsDead = false;

	UFUNCTION()
		void MontageEnded(UAnimMontage* Montage, bool bInterrupted);

public: 
	// Movement
	void UpDown(float Value);
	void LeftRight(float Value);
	void Yaw(float Value);
	void Pitch(float Value);

	// Damage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private: 
	// Spring Arm, Camera
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
		class UGameplayStatics* GameStatic;

	// Fire Particle
	UPROPERTY(VisibleAnywhere)
		class UParticleSystem* FireParticle;


	FTimerHandle TimerHandle;


public: 
	// Projectile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, Category = Pooler)
		UObjectPool* ObjectPooler;

	UPROPERTY(EditAnywhere, Category = Pooler)
		float LifeSpan = 3.f;

public: 
	// Movement
	UPROPERTY()
		float UpDownValue = 0;

	UPROPERTY()
		float LeftRightValue = 0;

	// Player Stat
	UPROPERTY(VisibleAnywhere)
		class UPlayerStatComponent* Stat;

	
};
