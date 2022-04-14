// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "PlayerAnimInstance.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "MyGameInstance.h"
#include "ObjectPool.h"
#include "PlayerStatComponent.h"
#include "MyGameModeBase.h"
#include "MyHUD.h"
#include "Components/TextBlock.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// SpringArm, Camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	// SpringArm Setting, Camera Attachment
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = 200.f;
	SpringArm->SetRelativeLocation(FVector(0.f, 30.f, 100.f));
	SpringArm->SetRelativeRotation(FRotator(-5.f, 0.f, 0.f));
	SpringArm->bUsePawnControlRotation = true; 

	// Character Location, Rotation
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));


	// Skeleton Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/Arts/ParagonMurdock/Characters/Heroes/Murdock/Skins/CS_MatteBlack/Mesh/Murdock_SF.Murdock_SF'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}

	// Fire Paricle
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS(TEXT("ParticleSystem'/Game/Arts/ParagonMurdock/FX/Particles/Abilities/Primary/FX/P_Murdock_STD_MUZZLE.P_Murdock_STD_MUZZLE'"));
	if (PS.Succeeded())
	{
		FireParticle = PS.Object;
	}

	// Projectile Pooling
	ObjectPooler = CreateDefaultSubobject<UObjectPool>(TEXT("ObjectPooler"));

	// Stat
	Stat = CreateDefaultSubobject<UPlayerStatComponent>(TEXT("Stat"));

	// Stat -> Movement Spped Setting
	GetCharacterMovement()->MaxWalkSpeed = Stat->GetWalkSpeed();
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// OnMontageEnded Delegate
	auto AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &APlayerCharacter::MontageEnded);
	}

	// Death Notify Delegate
	Stat->OnPlayerDeath.AddUObject(this, &APlayerCharacter::Death);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RefreshUI();
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Action Input Bind
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &APlayerCharacter::OnFire);
	PlayerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Reload);
	PlayerInputComponent->BindAction(TEXT("Pause"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Status);

	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &APlayerCharacter::StartSprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &APlayerCharacter::StopSprint);


	// Input Bind
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &APlayerCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &APlayerCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &APlayerCharacter::Yaw);
	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &APlayerCharacter::Pitch);

}

// HUD UI Refresh
void APlayerCharacter::RefreshUI()
{
	AMyGameModeBase* GameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		UMyHUD* MyHUD = Cast<UMyHUD>(GameMode->CurrentWidget);
		if (MyHUD)
		{
			const FString HpStr = FString::Printf(TEXT("%d"), Stat->GetHealth());
			MyHUD->GetHpText()->SetText(FText::FromString(HpStr));

			const FString AmmoStr = FString::Printf(TEXT("%d / %d"), Stat->GetAmmo(), Stat->GetMaxAmmo());
			MyHUD->GetAmmoText()->SetText(FText::FromString(AmmoStr));

			float Remain = GetWorldTimerManager().GetTimerRemaining(TimerHandle);
			float Cooltime = Stat->GetFireCooltimeRatio(Remain);

			// FireCooltime Crosshair
			MyHUD->GetFireCooltimeBar()->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Gauge"), Cooltime);
		}
	}
}

void APlayerCharacter::OnFire()
{
	// isCooltime or isRunning?
	if (IsFiring || IsSprinting)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cooltime or Sprinting"));
		return;
	}
		

	auto AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	if (AnimInstance)
	{
		if (AnimInstance->Montage_IsPlaying(AnimInstance->GetReloadMontage()))
			return;

		AnimInstance->PlayFireMontage();
	}

	// Projectile Ammo Check
	if (Stat->GetAmmo() <= 0)
	{
		Reload();
		return;
	}
	else
	{
		Stat->SetAmmo(Stat->GetAmmo() - 1);
	}

	// Fire Effect
	GameStatic->SpawnEmitterAttached(FireParticle, GetMesh(), FName("Muzzle"));

	// Pooled Projectile Activate
	if (ObjectPooler->PooledObjectSubclass != nullptr)
	{
		UWorld* const World = GetWorld();
		AProjectile* Poolable = ObjectPooler->GetPooledObject();

		if (World != nullptr)
		{
			// 스폰 위치
			const FVector MuzzleLocation = GetMesh()->GetSocketLocation(FName("Muzzle"));
			const FRotator SpawnRotation = GetControlRotation();
			const FVector SpawnLocation = MuzzleLocation + SpawnRotation.RotateVector(MuzzleOffset);

			if (Poolable == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Spawn Failed"));
				return;
			}
			
			Poolable->SetActorLocation(SpawnLocation);
			Poolable->SetActorRotation(SpawnRotation);
			Poolable->FireInDirection(SpawnRotation.Vector());
			
			Poolable->SetLifeSpan(LifeSpan);
			Poolable->SetActive(true);
		}
	}

	// FireCooltime Start
	IsFiring = true;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APlayerCharacter::FireCooltime, Stat->GetFireCooltime(), false);
	
}

// FireCooltime End
void APlayerCharacter::FireCooltime()
{
	IsFiring = false;
}

// Reload Action
void APlayerCharacter::Reload()
{
	auto AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	if (Stat->GetAmmo() >= Stat->GetMaxAmmo())
		return;

	if (AnimInstance)
	{
		AnimInstance->PlayReloadMontage();
	}

	Stat->SetAmmo(Stat->GetMaxAmmo());
}

// Status Widget Action
void APlayerCharacter::Status()
{
	Cast<AMyGameModeBase>(GameStatic->GetGameMode(GetWorld()))->StatusWidget->AddToViewport();
	GameStatic->GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeUIOnly());
	GameStatic->SetGamePaused(GetWorld(), true);
	GameStatic->GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;
}

// Sprint Action
void APlayerCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = Stat->GetSprintSpeed();
	IsSprinting = true;
}

void APlayerCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = Stat->GetWalkSpeed();
	IsSprinting = false;
}

// Death Action
void APlayerCharacter::Death()
{
	auto AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->PlayDeathMontage();
	}
	GameStatic->GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeUIOnly());
	IsDead = true;
}

// OnMontageEnded
void APlayerCharacter::MontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	auto AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	if (Montage == AnimInstance->GetDeathMontage())
	{
		Cast<AMyGameModeBase>(GameStatic->GetGameMode(GetWorld()))->GameOverWidget->AddToViewport();
		GameStatic->SetGamePaused(GetWorld(), true);
		GameStatic->GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;
		GameStatic->GetPlayerController(GetWorld(), 0)->bEnableClickEvents = true;
		GameStatic->GetPlayerController(GetWorld(), 0)->bEnableMouseOverEvents = true;
	}
}

// Up Down Movement
void APlayerCharacter::UpDown(float Value)
{
	UpDownValue = Value;
	AddMovementInput(GetActorForwardVector(), Value);

	if (UpDownValue < 1)
	{
		StopSprint();
	}
}

// Left Right Movement
void APlayerCharacter::LeftRight(float Value)
{
	LeftRightValue = Value;
	AddMovementInput(GetActorRightVector(), Value);
}

// Mouse
void APlayerCharacter::Yaw(float Value)
{
	AddControllerYawInput(Value);
}

void APlayerCharacter::Pitch(float Value)
{
	AddControllerPitchInput(Value);
}

// OnAttacked
float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	auto AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance && !IsDead)
	{
		AnimInstance->PlayOnAttackedMontage();
	}

	Stat->OnAttacked(DamageAmount);
	return DamageAmount;
}

