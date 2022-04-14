// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "EnemyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "EnemyStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "EnemyWidget.h"
#include "MyGameInstance.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -70.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	// Collision Setting
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyCollision"));
	GetCapsuleComponent()->SetCapsuleRadius(30.f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(72.f);

	Stat = CreateDefaultSubobject<UEnemyStatComponent>(TEXT("Stat"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/Arts/ParagonMinions/Characters/Minions/Down_Minions/Meshes/Minion_Lane_Melee_Core_Dawn.Minion_Lane_Melee_Core_Dawn'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}

	// Movement Setting
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = Stat->GetSpeed();

	// AI Controller
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	// HpBar UI Setting
	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
	HpBar->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("WidgetBlueprint'/Game/UI/WBP_EnemyHpBar.WBP_EnemyHpBar_C'"));
	if (UW.Succeeded())
	{
		HpBar->SetWidgetClass(UW.Class);
		HpBar->SetDrawSize(FVector2D(100.f, 30.f));
		HpBarVisibility(false);
	}
}

void AEnemyCharacter::SetActive(bool InActive)
{
	Active = InActive;
	SetActorEnableCollision(InActive);
	SetActorHiddenInGame(!InActive);

	IsDead = !InActive;
}

bool AEnemyCharacter::IsActive()
{
	return Active;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &AEnemyCharacter::MontageEnded);
		AnimInstance->AttackHit.AddUObject(this, &AEnemyCharacter::AttackCheck);
	}
	
	HpBar->InitWidget();
	auto HpWidget = Cast<UEnemyWidget>(HpBar->GetUserWidgetObject());
	if (HpWidget)
	{
		HpWidget->BindHealth(Stat);
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Stat->OnEnemyDeath.AddUObject(this, &AEnemyCharacter::Death);
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::Attack()
{
	if (IsDead)
		return;

	if (AnimInstance)
	{
		AnimInstance->PlayAttackMontage();
		AnimInstance->JumpToSection(AttackIndex);
	}

	AttackIndex = (AttackIndex + 1) % 4;
}

void AEnemyCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 120.f;
	float AttackRadius = 60.f;

	// Trace Channel Hit Result
	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	// Debug Drawing
	/*FVector Vec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + Vec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
	FColor DrawColor;
	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, Rotation, DrawColor, false, 0.2f);*/
	
	// Take Damage
	if (bResult && HitResult.Actor.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.Actor->GetName());

		FDamageEvent DamageEvent;
		HitResult.Actor->TakeDamage(Stat->GetAttack(), DamageEvent, GetController(), this);
	}
}

void AEnemyCharacter::Death()
{
	if (AnimInstance)
	{
		AnimInstance->PlayDeathMontage();
	}

	GetCharacterMovement()->Deactivate();
}

AEnemyAIController* AEnemyCharacter::GetAIController()
{
	auto AIController = Cast<AEnemyAIController>(GetController());

	return AIController;
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Stat->OnAttacked(DamageAmount);
	HpBarVisibility(true);

	return DamageAmount;
}

void AEnemyCharacter::HpBarVisibility(bool bActive)
{
	HpBar->SetVisibility(bActive, true);

	if (bActive)
	{
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				HpBar->SetVisibility(false, true);
			}), 1.f, false);
	}
}


void AEnemyCharacter::MontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AnimInstance->GetAttackMontage())
	{
		AttackEnd.Broadcast();
	}
		

	if (Montage == AnimInstance->GetDeathMontage())
	{
		UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
		GameInstance->SetScore(1500);
		GameInstance->SetEnemyCount(-1);
		GameInstance->RefreshUI();
		Deactivate();
	}
}
void AEnemyCharacter::Deactivate()
{
	SetActive(false);
}


