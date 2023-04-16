#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameBase.generated.h"

/**
 * Unreal save game class for saving highscores
 */
UCLASS()
class SPACEINVADERS_API USaveGameBase : public USaveGame
{
	GENERATED_BODY()

public:
	USaveGameBase();

	/** Array to save highscores */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Game")
	TArray<float> SavedHighscores;
};
