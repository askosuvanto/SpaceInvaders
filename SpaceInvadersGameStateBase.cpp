// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceInvadersGameStateBase.h"
#include "EngineUtils.h"
#include "SpawnerToolBase.h"
#include "SpaceInvadersGameInstanceBase.h"
#include "PlayerBase.h"

ASpaceInvadersGameStateBase::ASpaceInvadersGameStateBase()
{
	PlayerCurrentScore = 0.0f;
	EnemySpeedIncrease = 40.0f;
	DelayEnemyStart = 0.2f;
	EnemyActorsLeft = 0;
}

void ASpaceInvadersGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	GameInst = Cast<USpaceInvadersGameInstanceBase>(GetGameInstance());
	PlayerPtr = Cast<APlayerBase>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (GameInst != nullptr && PlayerPtr != nullptr)
	{
		PlayerCurrentScore = GameInst->PlayerAccumulatedScore;
		PlayerPtr->UpdateScoring(PlayerCurrentScore);
	}

	GetWorld()->GetTimerManager().SetTimer(EnemyStartTimer, this, &ASpaceInvadersGameStateBase::StartEnemyMovement, DelayEnemyStart, false);
}

void ASpaceInvadersGameStateBase::IncreaseEnemySpeed(float SpeedIncrease)
{
	OnIncreaseSpeed.Broadcast(SpeedIncrease);
}

void ASpaceInvadersGameStateBase::StartEnemyMovement()
{
	OnStartMoving.Broadcast(1.0f);

	GetWorld()->GetTimerManager().ClearTimer(EnemyStartTimer);
}

void ASpaceInvadersGameStateBase::IncreasePlayerScore(float Amount)
{
	PlayerCurrentScore += Amount;
	PlayerPtr->UpdateScoring(PlayerCurrentScore);
}

void ASpaceInvadersGameStateBase::AddEnemy()
{
	EnemyActorsLeft++;
}

void ASpaceInvadersGameStateBase::RemoveEnemy()
{
	EnemyActorsLeft--;

	if (EnemyActorsLeft == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("You Won! Your Score: %f"), PlayerCurrentScore));

		PlayerPtr->LevelFinished();
	}
}

void ASpaceInvadersGameStateBase::EndGame()
{
	if (!GameInst)
	{
		GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Red, TEXT("No Game Instance Found!"));
		return;
	}

	TArray<float> TempHighscores;
	TempHighscores = GameInst->GetHighscores();

	if (TempHighscores.Num() > 0)
	{
		// Add player's score to the highscores list and remove the lowest score
		TempHighscores.Add(PlayerCurrentScore);
		TempHighscores.Sort(std::greater<float>());
		TempHighscores.RemoveAt(TempHighscores.Num() - 1);
		GameInst->SetHighscores(TempHighscores);
	}
}
