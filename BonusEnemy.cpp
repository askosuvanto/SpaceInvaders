// Fill out your copyright notice in the Description page of Project Settings.


#include "BonusEnemy.h"
#include "SpaceInvadersGameStateBase.h"

void ABonusEnemy::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(EnemyLifeSpan);

	if (StartDirection == EStartDirection::Right)
	{
		MoveDirection = 1.0f;

		FRotator NewRotation = PawnMesh->GetRelativeRotation();
		NewRotation.Yaw += 90.0f;
		PawnMesh->SetRelativeRotation(NewRotation);
	}
	else if (StartDirection == EStartDirection::Left)
	{
		MoveDirection = -1.0f;
		CurrentPosition = GetActorLocation();
		StartingPosition = CurrentPosition;
		StartingPosition.Y -= EnemyMaxMovement;

		FRotator NewRotation = PawnMesh->GetRelativeRotation();
		NewRotation.Yaw -= 90.0f;
		PawnMesh->SetRelativeRotation(NewRotation);
	}
}

void ABonusEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveEnemy(DeltaTime);
}

void ABonusEnemy::SpawnPickup(FVector SpawnLocation, FRotator SpawnRotation)
{
	UWorld* const World = GetWorld();

	if (World != nullptr)
	{
		if (PickupsToSpawn.Num() > 0)
		{
			int NewIndex = FMath::RandRange(0, PickupsToSpawn.Num() - 1);

			TSubclassOf<AActor> ActorTypeToSpawn = PickupsToSpawn[NewIndex];

			if (ActorTypeToSpawn != nullptr)
			{
				// Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				World->SpawnActor<AActor>(ActorTypeToSpawn, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
		}
		else
		{
			// ERROR no spawnable actors found
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("No spawnable items found!"));
		}
	}
}

void ABonusEnemy::Die()
{
	GameStatePtr->IncreasePlayerScore(ScorePoints);
	GameStatePtr = nullptr;

	SpawnDeathEffect();
	SpawnPickup(GetActorLocation(), GetActorRotation());

	Destroy();
}
