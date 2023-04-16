// Copyright Epic Games, Inc. All Rights Reserved.


#include "SpaceInvadersGameModeBase.h"
#include "SpaceInvadersGameStateBase.h"
#include "SpaceInvadersGameInstanceBase.h"
#include "PlayerBase.h"

ASpaceInvadersGameModeBase::ASpaceInvadersGameModeBase()
{
	DefaultPawnClass = APlayerBase::StaticClass();

	GameStateClass = ASpaceInvadersGameStateBase::StaticClass();
}

// Load highscores at the start of the game
void ASpaceInvadersGameModeBase::BeginPlay()
{
	GameInst = Cast<USpaceInvadersGameInstanceBase>(GetGameInstance());

	if (GameInst != nullptr)
	{
		if (GameInst->bLoadSaveGameAtStartup)
		{
			GameInst->LoadHighscores();
			GameInst->bLoadSaveGameAtStartup = false;
		}
	}
}
