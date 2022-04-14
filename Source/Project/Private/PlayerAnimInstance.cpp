// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "GameFrameWork/Character.h"
#include "GameFrameWork/PawnMovementComponent.h"
#include "PlayerCharacter.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_Fire(TEXT("AnimMontage'/Game/Blueprints/Animations/AM_Fire.AM_Fire'"));
	if (AM_Fire.Succeeded())
	{
		FireMontage = AM_Fire.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_Reload(TEXT("AnimMontage'/Game/Blueprints/Animations/AM_Reload.AM_Reload'"));
	if (AM_Reload.Succeeded())
	{
		ReloadMontage = AM_Reload.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_OnAttacked(TEXT("AnimMontage'/Game/Blueprints/Animations/AM_OnAttacked.AM_OnAttacked'"));
	if (AM_OnAttacked.Succeeded())
	{
		OnAttackedMontage = AM_OnAttacked.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_Death(TEXT("AnimMontage'/Game/Blueprints/Animations/AM_Death.AM_Death'"));
	if (AM_Death.Succeeded())
	{
		DeathMontage = AM_Death.Object;
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();

	if (IsValid(Pawn))
	{
		// Get Speed (Walk or Idle)
		Speed = Pawn->GetVelocity().Size();

		auto Character = Cast<APlayerCharacter>(Pawn);
		if (Character)
		{
			// Blueprint Read
			IsFalling = Character->GetMovementComponent()->IsFalling();
			IsRunning = Character->IsSprinting;

			// Blend Space
			Vertical = Character->UpDownValue;
			Horizontal = Character->LeftRightValue;

			// Aim Offset
			FRotator ControlRotation = Character->GetControlRotation();
			FRotator ActorRotation = Character->GetActorRotation();
			FRotator Target = ControlRotation - ActorRotation;
			FRotator AimRotation = FMath::RInterpTo(FRotator(Pitch, 0.f, 0.f), Target, DeltaSeconds, 15.f);
			Pitch = FMath::ClampAngle(AimRotation.Pitch, -90.f, 90.f);
		}
	}
}

// 
void UPlayerAnimInstance::PlayFireMontage()
{
	if (!Montage_IsPlaying(FireMontage))
	{
		Montage_Play(FireMontage, 1.f);
	}
}

void UPlayerAnimInstance::PlayReloadMontage()
{
	if (!Montage_IsPlaying(ReloadMontage))
	{
		Montage_Play(ReloadMontage, 1.f);
	}
}

void UPlayerAnimInstance::PlayOnAttackedMontage()
{
	if (!Montage_IsPlaying(OnAttackedMontage))
	{
		Montage_Play(OnAttackedMontage, 1.f);
	}
}

void UPlayerAnimInstance::PlayDeathMontage()
{
	if (!Montage_IsPlaying(DeathMontage))
	{
		Montage_Play(DeathMontage, 1.f);
	}
}