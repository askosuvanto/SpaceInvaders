#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "GroupedEnemy.generated.h"

/**
 * Standard enemy character that is derived from base enemy class.
 */
UCLASS()
class SPACEINVADERS_API AGroupedEnemy : public AEnemyBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	/** Bind this actor to game state function so it can message this actor */
	UFUNCTION()
	void BindToGameState();

	/** Remove the binding */
	UFUNCTION()
	void UnbindFromGameState();

	/** Called when this actor recieves message */
	UFUNCTION()
	void GameStateIncreaseSpeedCallback(float SpeedIncrease);

	/** Called when this actor recieves message */
	UFUNCTION()
	void GameStateStartMovingCallback(float Direction);

public:
	/** called when enemy dies */
	virtual void Die() override;
};
