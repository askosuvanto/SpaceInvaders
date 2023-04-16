#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SpaceInvadersGameStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeSpeedDelegate, float, SpeedIncrease);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyMovementStart, float, MoveDirection);

class USpaceInvadersGameInstanceBase;
class APlayerBase;

/**
 * Game state handels moving grouped enemies in sync and keeps player's score.
 */
UCLASS()
class SPACEINVADERS_API ASpaceInvadersGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ASpaceInvadersGameStateBase();

	/** player's score */
	UPROPERTY(BlueprintReadOnly, Category = "Scoring")
	float PlayerCurrentScore;

	/** How much enemy speed increases when one is destroyed */
	UPROPERTY(BlueprintReadWrite, Category = "Difficulty")
	float EnemySpeedIncrease;

	/** Wait to make sure all enemies have been spawned so their movement can be synced */
	UPROPERTY(BlueprintReadOnly, Category = "Difficulty")
	float DelayEnemyStart;

	/** How many enemies is still left in this level */
	UPROPERTY(BlueprintReadOnly, Category = "Difficulty")
	int32 EnemyActorsLeft;

	/** Message all enemies to increase speed */
	UPROPERTY(BlueprintAssignable)
	FChangeSpeedDelegate OnIncreaseSpeed;

	/** Message all enemies to start moving */
	UPROPERTY(BlueprintAssignable)
	FEnemyMovementStart OnStartMoving;

private:
	/** Wait timer for starting enemy movement */
	FTimerHandle EnemyStartTimer;

	/** Pointer to the game instance */
	USpaceInvadersGameInstanceBase* GameInst;

	/** Pointer to player's pawn */
	APlayerBase* PlayerPtr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/** Increase enemies speed with this amount. Called when enemy dies */
	UFUNCTION(BlueprintCallable)
	void IncreaseEnemySpeed(float SpeedIncrease);

	/** Make enemies move in sync */
	UFUNCTION()
	void StartEnemyMovement();

	/** Add to player's score this amount */
	UFUNCTION()
	void IncreasePlayerScore(float Amount);

	/** Add 1 to enemy amount so we know how many enemies are in this level */
	UFUNCTION()
	void AddEnemy();

	/** Remove 1 from enemy amount so we know how many enemies are left */
	UFUNCTION()
	void RemoveEnemy();

	/** Save player's score. Called when quiting game and returning to the title screen */
	UFUNCTION(BlueprintCallable, Category = "Game")
	void EndGame();
};
