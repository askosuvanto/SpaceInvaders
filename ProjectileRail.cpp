// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileRail.h"
#include "Components/BoxComponent.h"
#include "EnemyBase.h"
#include "BunkerBase.h"

AProjectileRail::AProjectileRail()
{
	RailHitArea = CreateDefaultSubobject<UBoxComponent>(TEXT("RailHitArea"));

	RailHitArea->SetupAttachment(RootComponent);
	RailHitArea->SetBoxExtent(FVector(50.0f, 30.0f, 30.0f));
}

void AProjectileRail::BeginPlay()
{
	Super::BeginPlay();

	SpawnDeathEffect();

	CalculateCollisions();
}

void AProjectileRail::CalculateCollisions()
{
	RailHitArea->SetBoxExtent(RailHitAreaExtend, true); // Need to change hit area size so overlapping is updated correctly

	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	for (int i = 0; i < OverlappingActors.Num(); i++)
	{
		if (!OverlappingActors[i]->ActorHasTag("Projectile")) // Ignore other projectiles
		{
			if (OverlappingActors[i]->ActorHasTag("EnemyCharacter"))
			{
				AEnemyBase* EnemyActor = Cast<AEnemyBase>(OverlappingActors[i]);
				EnemyActor->Die();
			}
			else if (OverlappingActors[i]->ActorHasTag("Bunker"))
			{
				ABunkerBase* BunkerActor = Cast<ABunkerBase>(OverlappingActors[i]);
				BunkerActor->Die();
			}
		}
	}

	Die();
}

void AProjectileRail::Die()
{
	Destroy();
}
