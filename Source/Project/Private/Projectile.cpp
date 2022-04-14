


#include "Projectile.h"
#include "PlayerCharacter.h"
#include "PlayerStatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	// Collision Setting
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->InitSphereRadius(15.f);
	
	RootComponent = CollisionComponent;

	// Projectile Movement Default Setting
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->UpdatedComponent = CollisionComponent;
	ProjectileMovementComponent->InitialSpeed = 15000.f;
	ProjectileMovementComponent->MaxSpeed = 15000.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;

	// Player Stat
	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// Projectile Particle
	static ConstructorHelpers::FObjectFinder<UParticleSystem> VFX_SwordEnemyHitParticle(TEXT("ParticleSystem'/Game/Arts/ParagonMinions/FX/Particles/Minions/Shared/P_Minion_Melee_Impact.P_Minion_Melee_Impact'"));
	if (VFX_SwordEnemyHitParticle.Succeeded())
	{
		SwordEnemyHitParticle = VFX_SwordEnemyHitParticle.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> VFX_HitParticle(TEXT("ParticleSystem'/Game/Arts/ParagonMurdock/FX/Particles/Abilities/Primary/FX/P_PlasmaShot_Hit_World.P_PlasmaShot_Hit_World'"));
	if (VFX_HitParticle.Succeeded())
	{
		HitParticle = VFX_HitParticle.Object;
	}
}

void AProjectile::SetLifeSpan(float InLifeSpan)
{
	LifeSpan = InLifeSpan;
	GetWorldTimerManager().SetTimer(LifeSpanTimer, this, &AProjectile::Deactivate, LifeSpan, false);
}

void AProjectile::SetActive(bool InActive)
{
	Active = InActive;
	SetActorEnableCollision(InActive);
	SetActorHiddenInGame(!InActive);

	// Projectile Movement Resetting
	if (!InActive)
		ProjectileMovementComponent->Deactivate();
	else
	{
		ProjectileMovementComponent->Activate();
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		CollisionComponent->OnComponentHit.AddUniqueDynamic(this, &AProjectile::OnHit);
	}
}

bool AProjectile::IsActive()
{
	return Active;
}


void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectile::Deactivate()
{
	SetActive(false);
}

void AProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// Simulating Physics Only
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComponent != nullptr) && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(GetVelocity() * 10.f, GetActorLocation());
	}
	// Other Actor Hit
	else if ((OtherActor != nullptr) && (OtherActor != this))
	{
		FDamageEvent DamageEvent;
		Hit.Actor->TakeDamage(Player->Stat->GetAttack(), DamageEvent, GameStatic->GetPlayerController(GetWorld(), 0), this);

		// Enemy Hit Particle
		if (Hit.Component->GetCollisionProfileName() == TEXT("EnemyCollision"))
		{
			GameStatic->SpawnEmitterAtLocation(GetWorld(), SwordEnemyHitParticle, Hit.Location);
		}
	}
	// Hit Particle
	UE_LOG(LogTemp, Warning, TEXT("Collision! %s"), *(OtherActor->GetName()));

	Deactivate();

	GameStatic->SpawnEmitterAtLocation(GetWorld(), HitParticle, Hit.Location);
}

