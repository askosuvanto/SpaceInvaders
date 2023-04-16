#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItemBase.generated.h"

UENUM(BlueprintType)
enum EPickupItemType
{
	SpecialWeapon_Rail UMETA(DisplayName = "Railgun"),
	SpecialWeapon_Explosion UMETA(DisplayName = "Rocket")
};

class USceneComponent;
class UStaticMeshComponent;
class UBoxComponent;

/**
* Base class for pickup items.
*/
UCLASS()
class SPACEINVADERS_API APickupItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupItemBase();

	/** Static mesh of the item */
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ItemMesh;

	/** Using a box to calculate collisions */
	UPROPERTY(EditAnywhere)
	UBoxComponent* HitArea;

	/** The type of special weapon the item grants to player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Item")
	TEnumAsByte<EPickupItemType> ItemType;

	/** The amount of special ammo the item grants to player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Item")
	float SpecialWeaponAmmo = 1.0f;

	/** Default move speed of item */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Item")
	float ItemMoveSpeed = 200.0f;

	/** Identifier used in collisions */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup Item")
	FName TagName = "PickupItem";

private:
	/** Current position of item */
	FVector ItemCurrentPosition;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Called when item is picked up */
	virtual void Die();

};
