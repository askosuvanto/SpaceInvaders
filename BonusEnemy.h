#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "BonusEnemy.generated.h"

UENUM(BlueprintType)
enum EStartDirection
{
	Right UMETA(DisplayName = "To Right"),
	Left UMETA(DisplayName = "To Left")
};

/**
 * Bonus enemy character that is derived from base enemy class.
 */
UCLASS()
class SPACEINVADERS_API ABonusEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:
	/** Time until this enemy actor automatically destroyed. 0 means never */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bonus Enemy")
	float EnemyLifeSpan = 0.0f;

	/** The direction that the bonus enemy character starts moving to */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bonus Enemy")
	TEnumAsByte<EStartDirection> StartDirection;

	/** The type of pickup item to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bonus Enemy")
	TArray<TSubclassOf<AActor>> PickupsToSpawn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	/** Create pickup item when dying */
	UFUNCTION()
	virtual void SpawnPickup(FVector SpawnLocation, FRotator SpawnRotation);

public:
	/** called when enemy dies */
	virtual void Die() override;
	
};
