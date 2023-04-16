// Fill out your copyright notice in the Description page of Project Settings.


#include "GroupedEnemy.h"
#include "SpaceInvadersGameStateBase.h"

void AGroupedEnemy::BeginPlay()
{
	Super::BeginPlay();

	BindToGameState();

	if (GameStatePtr != nullptr)
	{
		GameStatePtr->AddEnemy();
	}
}

void AGroupedEnemy::BindToGameState()
{
	if (GameStatePtr != nullptr)
	{
		GameStatePtr->OnIncreaseSpeed.AddDynamic(this, &AGroupedEnemy::GameStateIncreaseSpeedCallback);
		GameStatePtr->OnStartMoving.AddDynamic(this, &AGroupedEnemy::GameStateStartMovingCallback);
	}
}

void AGroupedEnemy::UnbindFromGameState()
{
	if (GameStatePtr != nullptr)
	{
		GameStatePtr->OnIncreaseSpeed.RemoveDynamic(this, &AGroupedEnemy::GameStateIncreaseSpeedCallback);
		GameStatePtr->OnStartMoving.RemoveDynamic(this, &AGroupedEnemy::GameStateStartMovingCallback);
	}
}

void AGroupedEnemy::GameStateIncreaseSpeedCallback(float SpeedIncrease)
{
	if (EnemyCurrentMoveSpeed < EnemyMaxMoveSpeed)
	{
		EnemyCurrentMoveSpeed += SpeedIncrease;
	}
}

void AGroupedEnemy::GameStateStartMovingCallback(float Direction)
{
	MoveDirection = Direction;
}

void AGroupedEnemy::Die()
{
	UnbindFromGameState();

	GameStatePtr->IncreasePlayerScore(ScorePoints);
	GameStatePtr->RemoveEnemy();
	GameStatePtr->IncreaseEnemySpeed(EnemyMoveSpeedIncrease);
	GameStatePtr = nullptr;

	SpawnDeathEffect();

	Destroy();
}
