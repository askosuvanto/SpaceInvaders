// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "ProjectileBase.h"
#include "PickupItemBase.h"
#include "SpaceInvadersGameInstanceBase.h"

// Sets default values
APlayerBase::APlayerBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PivotPoint = CreateDefaultSubobject<USceneComponent>(TEXT("PivotPoint"));
	PawnMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PawnMesh"));
	HitArea = CreateDefaultSubobject<UBoxComponent>(TEXT("HitArea"));
	FiringPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("FiringPoint"));
	EngineEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EngineEffect"));

	PlayerCamera->SetupAttachment(RootComponent);
	PlayerCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 4000.0f));
	PlayerCamera->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

	PivotPoint->SetupAttachment(RootComponent);
	PivotPoint->SetRelativeLocation(FVector(-1800.0f, 0.0f, 0.0f));

	HitArea->SetupAttachment(PivotPoint);
	HitArea->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	HitArea->SetGenerateOverlapEvents(true);
	HitArea->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	HitArea->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);

	PawnMesh->SetupAttachment(PivotPoint);

	FiringPoint->SetupAttachment(PawnMesh);

	EngineEffectComponent->SetupAttachment(PivotPoint);
}

// Called when the game starts or when spawned
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();

	const FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	HitArea->AttachToComponent(PivotPoint, Rules);
	PawnMesh->AttachToComponent(PivotPoint, Rules);
	FiringPoint->AttachToComponent(PawnMesh, Rules);
	
	Tags.Add(TagName);
	HitArea->OnComponentBeginOverlap.AddDynamic(this, &APlayerBase::OnBoxBeginOverlap);

	PlayerCurrentMoveSpeed = PlayerDefaultMoveSpeed;
	PlayerCurrentAttackSpeed = PlayerDefaultAttackSpeed;

	if (GameInst == nullptr)
	{
		GameInst = Cast<USpaceInvadersGameInstanceBase>(GetGameInstance());
	}

	Lives = GameInst->PlayerAccumulatedLives;
	SpecialWeaponAmmo = GameInst->PlayerAccumulatedAmmo;
	SelectedSpecialWeapon = GameInst->CurrentSpecialWeapon;

	UpdateLivesWidget(Lives);
	UpdateAmmoWidget(SpecialWeaponAmmo);
}

// Called every frame
void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CurrentVelocity.IsZero())
	{
		FVector NewLocation = PivotPoint->GetRelativeLocation() + (CurrentVelocity * DeltaTime);

		if (NewLocation.Y < PlayerMaxMovement && NewLocation.Y > -PlayerMaxMovement)
		{
			PivotPoint->SetRelativeLocation(NewLocation);
		}
	}

	if (bCanFire)
	{
		if (bFiringTriggered)
		{
			FiringWeapon();
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
			FiringTimer = PlayerCurrentAttackSpeed;
		}
	}
}

// Called to bind functionality to input
void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MoveY", this, &APlayerBase::Move_YAxis);

	InputComponent->BindAction("FirePrimary", IE_Pressed, this, &APlayerBase::StartPrimaryFire);
	InputComponent->BindAction("FirePrimary", IE_Released, this, &APlayerBase::StopPrimaryFire);
	InputComponent->BindAction("FireSpecial", IE_Pressed, this, &APlayerBase::FireSpecial);
}

void APlayerBase::StartPrimaryFire()
{
	bFiringTriggered = true;
}

void APlayerBase::StopPrimaryFire()
{
	bFiringTriggered = false;
}

void APlayerBase::FireSpecial()
{
	if (SelectedSpecialWeapon == ESpecialWeapon::Railgun && SpecialWeaponAmmo > 0)
	{
		SpawnProjectile(PivotPoint->GetRelativeLocation() + FiringPoint->GetRelativeLocation(), FiringPoint->GetRelativeRotation(), SpecialWeaponRail);
		SpecialWeaponAmmo--;
		UpdateAmmoWidget(SpecialWeaponAmmo);
	}
	else if (SelectedSpecialWeapon == ESpecialWeapon::Rocket && SpecialWeaponAmmo > 0)
	{
		SpawnProjectile(PivotPoint->GetRelativeLocation() + FiringPoint->GetRelativeLocation(), FiringPoint->GetRelativeRotation(), SpecialWeaponExplosion);
		SpecialWeaponAmmo--;
		UpdateAmmoWidget(SpecialWeaponAmmo);
	}
}

void APlayerBase::DecreaseLives()
{
	Lives--;
	UpdateLivesWidget(Lives);

	// Spawn a sound if DeathSound is set
	if (DeathSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}

	if (Lives <= 0)
	{
		GameOver();
	}
}

void APlayerBase::Move_YAxis(float AxisValue)
{
	CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * PlayerCurrentMoveSpeed;
}

void APlayerBase::FiringWeapon()
{
	SpawnProjectile(PivotPoint->GetRelativeLocation() + FiringPoint->GetRelativeLocation(), FiringPoint->GetRelativeRotation(), ProjectileToFire);
}

void APlayerBase::SpawnProjectile(FVector SpawnLocation, FRotator SpawnRotation, TSubclassOf<AActor> SelectedWeapon)
{
	UWorld* const World = GetWorld();

	if (World != nullptr)
	{
		if (SelectedWeapon != nullptr)
		{
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			// Spawn a new actor and save it's pointer if succesful so we know when it is destroyed
			AActor* NewActor;
			NewActor = World->SpawnActor<AActor>(SelectedWeapon, SpawnLocation, SpawnRotation, ActorSpawnParams);

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

void APlayerBase::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("PickupItem"))
	{
		APickupItemBase* PickedUpItem = Cast<APickupItemBase>(OtherActor);

		if (PickedUpItem->ItemType == EPickupItemType::SpecialWeapon_Rail)
		{
			SelectedSpecialWeapon = ESpecialWeapon::Railgun;
			SpecialWeaponAmmo += PickedUpItem->SpecialWeaponAmmo;
			UpdateAmmoWidget(SpecialWeaponAmmo);
		}
		else if (PickedUpItem->ItemType == EPickupItemType::SpecialWeapon_Explosion)
		{
			SelectedSpecialWeapon = ESpecialWeapon::Rocket;
			SpecialWeaponAmmo += PickedUpItem->SpecialWeaponAmmo;
			UpdateAmmoWidget(SpecialWeaponAmmo);
		}

		PickedUpItem->Die();
	}
	else if (OtherActor->ActorHasTag("EnemyCharacter") && !OtherActor->ActorHasTag("Projectile")) // When enemy hits player
	{
		GameOver();
	}
}

void APlayerBase::Die()
{
	Destroy();
}
