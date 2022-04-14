

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Projectile.generated.h"


UCLASS()
class PROJECT_API AProjectile : public AActor
{
	GENERATED_BODY()

public:	
	AProjectile();

	// Projectile LifeSpan
	virtual void SetLifeSpan(float InLifeSpan) override;

	void SetActive(bool InActive);
	bool IsActive();

protected:
	virtual void BeginPlay() override;
	
	float LifeSpan = 3.f;
	FTimerHandle LifeSpanTimer;

	bool Active;
	void Deactivate();


public:	
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = Movement)
		UProjectileMovementComponent* ProjectileMovementComponent;

	void FireInDirection(const FVector& ShootDirection);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

private:
	UPROPERTY(VisibleAnywhere)
		class UParticleSystem* SwordEnemyHitParticle;

	UPROPERTY(VisibleAnywhere)
		class UParticleSystem* HitParticle;

	UPROPERTY(VisibleAnywhere)
		class APlayerCharacter* Player;

	UPROPERTY(VisibleAnywhere)
		class UGameplayStatics* GameStatic;
};
