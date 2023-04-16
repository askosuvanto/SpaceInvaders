// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItemBase.h"
#include "Components/BoxComponent.h"

// Sets default values
APickupItemBase::APickupItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	HitArea = CreateDefaultSubobject<UBoxComponent>(TEXT("HitArea"));

	HitArea->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	HitArea->SetupAttachment(RootComponent);

	ItemMesh->SetGenerateOverlapEvents(false);
	ItemMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APickupItemBase::BeginPlay()
{
	Super::BeginPlay();
	
	Tags.Add(TagName);

	SetLifeSpan(10.0f); // Destroy the item if no one has picked it up

	ItemCurrentPosition = GetActorLocation();
}

// Called every frame
void APickupItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Make the pickup item "drop down"
	ItemCurrentPosition.X -= ItemMoveSpeed * DeltaTime;
	RootComponent->SetRelativeLocation(ItemCurrentPosition);
}

void APickupItemBase::Die()
{
	Destroy();
}
