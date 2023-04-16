#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpaceInvadersGameModeBase.generated.h"

class USpaceInvadersGameInstanceBase;
class APlayerBase;

/**
 * Game mode sets player pawn and game state and loads highscores at the start of the game.
 */
UCLASS()
class SPACEINVADERS_API ASpaceInvadersGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASpaceInvadersGameModeBase();

private:
	/** Pointer to the game instance */
	USpaceInvadersGameInstanceBase* GameInst;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
