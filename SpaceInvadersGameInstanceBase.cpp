// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceInvadersGameInstanceBase.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGameBase.h"

TArray<float> USpaceInvadersGameInstanceBase::GetHighscores() const
{
	return HighscoresArray;
}

void USpaceInvadersGameInstanceBase::SetHighscores(TArray<float> NewScores)
{
	HighscoresArray = NewScores;
}

void USpaceInvadersGameInstanceBase::SaveHighscores()
{
	NewSave = Cast<USaveGameBase>(UGameplayStatics::CreateSaveGameObject(USaveGameBase::StaticClass()));
	NewSave->SavedHighscores = HighscoresArray;

	if (!UGameplayStatics::SaveGameToSlot(NewSave, SlotName, UserIndex))
	{
		GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, TEXT("ERROR: Cannot save game!"));
	}
}

void USpaceInvadersGameInstanceBase::LoadHighscores()
{
	USaveGame* NewLoad = UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex);

	if (NewLoad != nullptr)
	{
		USaveGameBase * LoadedSave = Cast<USaveGameBase>(NewLoad);
		HighscoresArray = LoadedSave->SavedHighscores;
	}
	else
	{
		// If there is nothting to load then set highscores to 0
		HighscoresArray.Empty();

		for (int i = 0; i < 10; i++)
		{
			HighscoresArray.Add(0.0f);
		}
	}
}

int32 USpaceInvadersGameInstanceBase::GetNextLevelIndex()
{
	CurrentLevelIndex++;

	if (CurrentLevelIndex > 5)
	{
		CurrentLevelIndex = 5;
	}

	return CurrentLevelIndex;
}

void USpaceInvadersGameInstanceBase::ResetGame()
{
	CurrentLevelIndex = 0;
	PlayerAccumulatedScore = 0.0f;
	PlayerAccumulatedLives = 3.0f;
	PlayerAccumulatedAmmo = 0.0f;
	CurrentSpecialWeapon = ESpecialWeapon::None;
}

FString USpaceInvadersGameInstanceBase::FormatHighscores()
{
	FString FormattedString = "";

	for (int32 i = 0; i < HighscoresArray.Num(); i++)
	{
		FormattedString.Append(FString::FromInt((i + 1)));

		if (i < 9)
		{
			FormattedString.Append(".  ");
		}
		else
		{
			FormattedString.Append(". ");
		}

		FormattedString.Append(FString::SanitizeFloat(HighscoresArray[i], 0));
		FormattedString.Append(LINE_TERMINATOR);
	}

	return FormattedString;
}
