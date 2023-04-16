// Fill out your copyright notice in the Description page of Project Settings.


#include "BunkerBase.h"
#include "Components/BoxComponent.h"
#include "ProjectileBase.h"

// Sets default values
ABunkerBase::ABunkerBase()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	HitArea = CreateDefaultSubobject<UBoxComponent>(TEXT("HitArea"));
	BunkerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BunkerMesh"));

	HitArea->SetupAttachment(RootComponent);
	HitArea->SetBoxExtent(FVector(10.0f, 10.0f, 10.0f));

	BunkerMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABunkerBase::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(TagName);
	
	HitArea->OnComponentBeginOverlap.AddDynamic(this, &ABunkerBase::OnBoxBeginOverlap);
}

void ABunkerBase::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("EnemyCharacter"))
	{
		Die();
	}
}

void ABunkerBase::Die()
{
	Destroy();
}
