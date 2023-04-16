#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class USceneComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

/**
* Base class for projectiles.
*/

UCLASS()
class SPACEINVADERS_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

	/** Using a sphere to calculate collisions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	USphereComponent* HitArea;

	/** Static mesh of the projectile */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	UStaticMeshComponent* ProjectileMesh;

	/** Moving the projectile with Unreal's Projectile Movement Component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile | Movement")
	UProjectileMovementComponent* ProjectileMovementComponent;

	/** Spawn a Niagara system when dying */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	UNiagaraSystem* DeathEffectSystem;

	/** Spawn a Metasound sound when dying */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	USoundBase* DeathSound;

	/** Identifier used in collisions */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	FName TagName = "Projectile";

	/** The speed of the projectile */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileSpeed = 600.0f;

	/** The damage the projectile does when hitting enemy character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileDamage = 10.0f;

	/** Maximum time the projectie can exist. 0 means infinite */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileLifeTime = 10.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Create visuals when dying */
	UFUNCTION()
	virtual void SpawnDeathEffect();

public:
	/** Called when colliding with something */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Called when projectile dies */
	UFUNCTION()
	virtual void Die();

};
