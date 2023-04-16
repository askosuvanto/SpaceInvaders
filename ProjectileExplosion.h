#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "ProjectileExplosion.generated.h"

class USphereComponent;
class UNiagaraSystem;

/**
 * Rocket that is derived from base projectile class.
 */
UCLASS()
class SPACEINVADERS_API AProjectileExplosion : public AProjectileBase
{
	GENERATED_BODY()
	
public:
	AProjectileExplosion();

	/** Need to extend collision area after being spawned so that the collisions are updated correctly */
	UPROPERTY(EditAnywhere, Category = "Projectile | Explosion")
	float ExplosionHitAreaExtend = 200.0f;

	/** Using a sphere to calculate collisions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile | Explosion")
	USphereComponent* ExplosionHitArea;

	/** Spawn a Niagara system when dying */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile | Explosion")
	UNiagaraSystem* RocketEffectSystem;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called when projectile dies. Finds all overlapping actors */
	UFUNCTION()
	virtual void CalculateCollisions();

public:
	/** Called when projectile dies */
	virtual void Die() override;
};
