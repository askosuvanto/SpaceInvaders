#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BunkerBase.generated.h"

class USceneComponent;
class UBoxComponent;
class UStaticMeshComponent;

/**
* Base class for bunkers.
*/
UCLASS()
class SPACEINVADERS_API ABunkerBase : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ABunkerBase();

	/** Using a box to calculate collisions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* HitArea;

	/** Static mesh of the bunker */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* BunkerMesh;

	/** Identifier used in collisions */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bunker")
	FName TagName = "Bunker";

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/** Called when enemy character collides with the bunker */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Called when bunker is destroyed */
	virtual void Die();

};
