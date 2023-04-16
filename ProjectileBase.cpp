// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BunkerBase.h"
#include "EnemyBase.h"
#include "PlayerBase.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	HitArea = CreateDefaultSubobject<USphereComponent>(TEXT("HitArea"));
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));

	HitArea->SetSphereRadius(10.0f);
	HitArea->SetupAttachment(RootComponent);

	ProjectileMesh->SetGenerateOverlapEvents(false);
	ProjectileMesh->SetupAttachment(RootComponent);

	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->InitialSpeed = ProjectileSpeed;
	ProjectileMovementComponent->MaxSpeed = ProjectileSpeed;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(TagName);
	
	HitArea->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnSphereBeginOverlap);

	SetLifeSpan(ProjectileLifeTime);

	ProjectileMovementComponent->InitialSpeed = ProjectileSpeed;
	ProjectileMovementComponent->MaxSpeed = ProjectileSpeed;

	ProjectileMovementComponent->Velocity = GetActorRotation().Vector() * ProjectileMovementComponent->InitialSpeed;
}

void AProjectileBase::SpawnDeathEffect()
{
	// Spawn a visual effect if DeathEffectSystem is set
	if (DeathEffectSystem != nullptr)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, DeathEffectSystem, GetActorLocation(), GetActorRotation());
	}

	// Spawn a sound if DeathSound is set
	if (DeathSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}
}

void AProjectileBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag("Projectile")) // Ignore other projectiles
	{
		if (OtherActor->ActorHasTag("Bunker"))
		{
			ABunkerBase* BunkerActor = Cast<ABunkerBase>(OtherActor);
			BunkerActor->Die();
			Die();
		}
		else if (OtherActor->ActorHasTag("EnemyCharacter") && ActorHasTag("PlayerCharacter"))
		{
			AEnemyBase* EnemyActor = Cast<AEnemyBase>(OtherActor);
			EnemyActor->Die();
			Die();
		}
		else if (OtherActor->ActorHasTag("PlayerCharacter"))
		{
			APlayerBase* PlayerActor = Cast<APlayerBase>(OtherActor);
			PlayerActor->DecreaseLives();
			Die();
		}
	}
}

void AProjectileBase::Die()
{
	SpawnDeathEffect();

	Destroy();
}
