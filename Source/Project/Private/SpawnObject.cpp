// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnObject.h"
#include "EnemyCharacter.h"
#include "SwordEnemyObjectPool.h"
#include "EnemyAIController.h"
#include "EnemyStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "MyGameInstance.h"

ASpawnObject::ASpawnObject()
{
	PrimaryActorTick.bCanEverTick = false;

	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	SpawnVolume->BodyInstance.SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent = SpawnVolume;

	ObjectPooler = CreateDefaultSubobject<USwordEnemyObjectPool>(TEXT("ObjectPooler"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> VFX_VolumeParticle(TEXT("ParticleSystem'/Game/Arts/ParagonMinions/FX/Particles/PlayerBuffs/Proto_DroppedBuff_BlueBuff.Proto_DroppedBuff_BlueBuff'"));
	if (VFX_VolumeParticle.Succeeded())
	{
		VolumeParticle = VFX_VolumeParticle.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> VFX_SpawnParticle(TEXT("ParticleSystem'/Game/Arts/ParagonMinions/FX/Particles/Minions/Shared/P_MinionSpawn.P_MinionSpawn'"));
	if (VFX_SpawnParticle.Succeeded())
	{
		SpawnParticle = VFX_SpawnParticle.Object;
	}
}

void ASpawnObject::BeginPlay()
{
	Super::BeginPlay();
	
	// Spawn Particle
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), VolumeParticle, RootComponent->GetComponentLocation());
}

void ASpawnObject::Spawn()
{
	AEnemyCharacter* SwordEnemyPoolable = ObjectPooler->GetSwordEnemyPooledObject();
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());

	if (SwordEnemyPoolable == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Failed"));
		return;
	}
	GameInstance->SetEnemyCount(1);
	GameInstance->SetSpawnedCount(1);
	GameInstance->RefreshUI();


	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnParticle, RootComponent->GetComponentLocation());

	// Actor Location, Movement Activate
	FVector Location = GetActorLocation();
	SwordEnemyPoolable->SetActorLocation(Location);
	SwordEnemyPoolable->GetCharacterMovement()->Activate();
	SwordEnemyPoolable->SetActive(true);
	
}

