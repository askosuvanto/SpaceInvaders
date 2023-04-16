// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "ProjectileBase.h"
#include "SpaceInvadersGameStateBase.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	PawnMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PawnMesh"));
	HitArea = CreateDefaultSubobject<UBoxComponent>(TEXT("HitArea"));
	FiringPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("FiringPoint"));

	PawnMesh->SetupAttachment(RootComponent);

	HitArea->SetupAttachment(RootComponent);
	HitArea->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	HitArea->SetGenerateOverlapEvents(true);
	HitArea->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	HitArea->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	
	FiringPoint->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	Tags.Add(TagName);

	EnemyCurrentMoveSpeed = EnemyDefaultMoveSpeed;
	EnemyCurrentAttackSpeed = EnemyDefaultAttackSpeed;
	FiringTimer = EnemyCurrentAttackSpeed;
	StartingPosition = GetActorLocation();
	CurrentPosition = StartingPosition;

	if (GameStatePtr == nullptr)
	{
		GameStatePtr = Cast<ASpaceInvadersGameStateBase>(GetWorld()->GetGameState());
	}
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveEnemy(DeltaTime);

	FireWeapon(DeltaTime);
}

bool AEnemyBase::CheckForAnotherEnemyInFront()
{
	bool ReturnValue = false;

	FHitResult Hit;
	FVector TraceStart = GetActorLocation() + FiringPoint->GetRelativeLocation();
	FVector TraceEnd = FiringPoint->GetForwardVector() * 5000.0f;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannelProperty, QueryParams);

	if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
	{
		if (Hit.GetActor()->ActorHasTag("EnemyCharacter"))
		{
			ReturnValue = true;
		}
	}

	return ReturnValue;
}

void AEnemyBase::MoveEnemy(float DeltaTime)
{
	if (MoveDirection > 0)
	{
		if (CurrentPosition.Y < StartingPosition.Y + EnemyMaxMovement)
		{
			CurrentPosition.Y += DeltaTime * EnemyCurrentMoveSpeed;

			RootComponent->SetRelativeLocation(CurrentPosition);
		}
		else
		{
			MoveDirection = -1.0f;
			CurrentPosition.X -= EnemyHeightOffset;
		}
	}
	else if (MoveDirection < 0)
	{
		if (CurrentPosition.Y > StartingPosition.Y)
		{
			CurrentPosition.Y -= DeltaTime * EnemyCurrentMoveSpeed;

			RootComponent->SetRelativeLocation(CurrentPosition);
		}
		else
		{
			MoveDirection = 1.0f;
			CurrentPosition.X -= EnemyHeightOffset;
		}
	}
}

void AEnemyBase::FireWeapon(float DeltaTime)
{
	if (bCanFire)
	{
		if (!CheckForAnotherEnemyInFront()) //Check if there is another enemy in front of this one
		{
			SpawnProjectile(GetActorLocation() + FiringPoint->GetRelativeLocation(), FiringPoint->GetRelativeRotation());
			bCanFire = false;
		}
	}
	else
	{
		if (FiringTimer > 0.0f)
		{
			FiringTimer -= DeltaTime;
		}
		else
		{
			bCanFire = true;
			FiringTimer = EnemyCurrentAttackSpeed;
		}
	}
}

void AEnemyBase::SpawnProjectile(FVector SpawnLocation, FRotator SpawnRotation)
{
	UWorld* const World = GetWorld();

	if (World != nullptr)
	{
		if (ProjectileToFire != nullptr)
		{
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			// Spawn a new actor and save it's pointer if succesful so we know when it is destroyed
			AActor* NewActor;
			NewActor = World->SpawnActor<AActor>(ProjectileToFire, SpawnLocation, SpawnRotation, ActorSpawnParams);

			if (NewActor != nullptr)
			{
				NewActor->Tags.Add(TagName);
			}

			// Spawn a visual effect if FiringFX is set
			if (FiringFX != nullptr)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FiringFX, SpawnLocation, SpawnRotation);
			}

			// Spawn a sound if FiringSound is set
			if (FiringSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FiringSound, GetActorLocation());
			}
		}
	}
}

void AEnemyBase::SpawnDeathEffect()
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

void AEnemyBase::Die()
{
	GameStatePtr->IncreasePlayerScore(ScorePoints);
	GameStatePtr = nullptr;

	SpawnDeathEffect();

	Destroy();
}
