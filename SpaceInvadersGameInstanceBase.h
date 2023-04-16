#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PlayerBase.h"
#include "SpaceInvadersGameInstanceBase.generated.h"

class USaveGameBase;
//class APlayerBase;

/**
 * The game instance handels all variables that should stay between levels.
 * It also handles saving and loading top highscores.
 */
UCLASS()
class SPACEINVADERS_API USpaceInvadersGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()
	
public:
	/** Make sure saved scores are loaded only at the beginnig of the game */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Game")
	bool bLoadSaveGameAtStartup = true;

	/** Save game data structure */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Game")
	USaveGameBase* NewSave;

	/** The score player got from this level to be saved for the next level */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	float PlayerAccumulatedScore = 0.0f;

	/** The lives player got left to be saved for the next level */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	float PlayerAccumulatedLives = 3.0f;

	/** The special weapon ammo player got left from this level to be saved for the next level */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	float PlayerAccumulatedAmmo = 0.0f;

	/** The selected special weapon that player has to be saved for the next level */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	TEnumAsByte<ESpecialWeapon> CurrentSpecialWeapon = ESpecialWeapon::None;

private:
	/** Record of 10 highest scores */
	TArray<float> HighscoresArray;

	/** Currently loaded level where 0 is title level */
	int32 CurrentLevelIndex = 0;

	/** Save game slot name */
	FString SlotName = "SpaceInvaders";

	/** Save game slot index */
	int32 UserIndex = 0;

public:
	/** Return array of highscore values */
	UFUNCTION(BlueprintCallable, Category = "Scoring")
	TArray<float> GetHighscores() const;

	/** Set new highscore array */
	UFUNCTION(BlueprintCallable, Category = "Scoring")
	void SetHighscores(TArray<float> NewScores);

	/** Save highscores to Unreal save slot */
	UFUNCTION(BlueprintCallable, Category = "Save Game")
	void SaveHighscores();

	/** Load highscores from Unreal save slot */
	UFUNCTION(BlueprintCallable, Category = "Save Game")
	void LoadHighscores();

	/** Get the index of the next level and increase index */
	UFUNCTION(BlueprintCallable, Category = "Game")
	int32 GetNextLevelIndex();

	/** Reset saved items to their default values */
	UFUNCTION(BlueprintCallable, Category = "Game")
	void ResetGame();

	/** Format highscore array to text */
	UFUNCTION(BlueprintCallable, Category = "Scoring")
	FString FormatHighscores();
};
