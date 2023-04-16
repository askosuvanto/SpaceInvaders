#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemyBase.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UBoxComponent;
class UNiagaraSystem;
class UArrowComponent;
class ASpaceInvadersGameStateBase;

/**
* Base class for enemy characters.
*/
UCLASS()
class SPACEINVADERS_API AEnemyBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyBase();

	/** Static mesh for the enemy character */
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PawnMesh;

	/** Using a box to calculate collisions */
	UPROPERTY(EditAnywhere)
	UBoxComponent* HitArea;

	/** Spawn a Niagara system when dying */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	UNiagaraSystem* DeathEffectSystem;

	/** Spawn a Metasound sound when dying */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	USoundBase* DeathSound;

	/** Using Niagara system to make weapon firing effects */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy | Weapon")
	UNiagaraSystem* FiringFX;

	/** Spawn a Metasound sound when firing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy | Weapon")
	USoundBase* FiringSound;

	/** This projectile is spawned when enemy character is firing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy | Weapon")
	TSubclassOf<AActor> ProjectileToFire;

	/**  Visual helper to setup the point from which enemy character's projectiles are spawned */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy | Weapon")
	UArrowComponent* FiringPoint;

	/** Notice only actors that use this collision channel */
	UPROPERTY(EditAnywhere, Category = "Enemy | Weapon")
	TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECollisionChannel::ECC_Pawn;

	/** Identifier used in collisions */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	FName TagName = "EnemyCharacter";

	/** The health of enemy character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float Health = 10.0f;

	/** How much points does player gain from destroying this enemy */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float ScorePoints = 10.0f;

	/** Default move speed of the enemy character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float EnemyDefaultMoveSpeed = 300.0f;

	/** How much does other enemies' move speed increase when this enemy is destroyed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float EnemyMoveSpeedIncrease = 40.0f;

	/** Cap the enemy character's move speed so it can not increase endlessly */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float EnemyMaxMoveSpeed = 1500.0f;

	/** Default attack speed of the enemy character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float EnemyDefaultAttackSpeed = 2.0f;

	/** The amount the enemy character moves down when reaching the end of the movement */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float EnemyHeightOffset = 50.0f;

	/** The amount the enemy character moves from side to side */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float EnemyMaxMovement = 1000.0f;

protected:
	/** Pointer to the game state */
	ASpaceInvadersGameStateBase* GameStatePtr;

	/** Move direction of the enemy character. 1.0f moves the enemy character to right and -1.0f moves to the left */
	float MoveDirection = 0.0f;

	/** Current move speed of the enemy character */
	float EnemyCurrentMoveSpeed;

	/** Current attack speed of the enemy character */
	float EnemyCurrentAttackSpeed;

	/** The starting position of the enemy character */
	FVector StartingPosition;

	/** Current position of the enemy character */
	FVector CurrentPosition;

private:
	/** Wait timer between spawning projectiles */
	float FiringTimer = 0.0f;

	/** Check to see if a new projectile can be spawned */
	bool bCanFire = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	/** Called when enemy characer is trying to fire */
	bool CheckForAnotherEnemyInFront();

protected:
	/** Called every frame to move the enemy character */
	UFUNCTION()
	virtual void MoveEnemy(float DeltaTime);

	/** Called every frame to make the enemy character fire a projectile */
	UFUNCTION()
	virtual void FireWeapon(float DeltaTime);

	/** Called when the enemy character is firing a projectile */
	UFUNCTION()
	virtual void SpawnProjectile(FVector SpawnLocation, FRotator SpawnRotation);

	/** Create visuals when the enemy character dies */
	UFUNCTION()
	virtual void SpawnDeathEffect();

public:
	/** Called when enemy dies */
	UFUNCTION()
	virtual void Die();
};
