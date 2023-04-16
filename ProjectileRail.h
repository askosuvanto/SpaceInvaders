#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "ProjectileRail.generated.h"

class UBoxComponent;

/**
 * Railgun that is derived from base projectile class.
 */
UCLASS()
class SPACEINVADERS_API AProjectileRail : public AProjectileBase
{
	GENERATED_BODY()

public:
	AProjectileRail();

	/** Need to extend collision area after being spawned so that the collisions are updated correctly */
	UPROPERTY(EditAnywhere, Category = "Projectile | Rail")
	FVector RailHitAreaExtend = FVector(5000.0f, 30.0f, 30.0f);

	/** Using a box to calculate collisions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile | Rail")
	UBoxComponent* RailHitArea;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called when projectile is spawned. Finds all overlapping actors */
	UFUNCTION()
	virtual void CalculateCollisions();

public:
	/** Called when projectile dies */
	virtual void Die() override;
};
