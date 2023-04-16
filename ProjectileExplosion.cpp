// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileExplosion.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "EnemyBase.h"
#include "BunkerBase.h"

AProjectileExplosion::AProjectileExplosion()
{
	ExplosionHitArea = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionHitArea"));

	ExplosionHitArea->SetupAttachment(RootComponent);
	ExplosionHitArea->SetSphereRadius(5.0f);
}

void AProjectileExplosion::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add("Explosion");
	
	// Spawn a visual effect if RocketEffectSystem is set
	if (RocketEffectSystem != nullptr)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(RocketEffectSystem, ProjectileMesh, NAME_None, ProjectileMesh->GetRelativeLocation(), ProjectileMesh->GetRelativeRotation(), EAttachLocation::SnapToTarget, true);
	}
}

void AProjectileExplosion::CalculateCollisions()
{
	ExplosionHitArea->SetSphereRadius(ExplosionHitAreaExtend, true); // Need to change hit area size so overlapping is updated correctly

	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::White, FString::Printf(TEXT("Searching.. %f"), OverlappingActors.Num()));

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
}

void AProjectileExplosion::Die()
{
	CalculateCollisions();
	SpawnDeathEffect();

	Destroy();
}
