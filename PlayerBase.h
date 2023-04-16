#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerBase.generated.h"

UENUM(BlueprintType)
enum ESpecialWeapon
{
	None UMETA(DisplayName = "None"),
	Railgun UMETA(DisplayName = "Railgun"),
	Rocket UMETA(DisplayName = "Rocket")
};

class USceneComponent;
class UCameraComponent;
class UStaticMeshComponent;
class UBoxComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class UArrowComponent;
class ASpaceInvadersGameStateBase;
class USpaceInvadersGameInstanceBase;

/**
* Base class for player's character.
*/
UCLASS()
class SPACEINVADERS_API APlayerBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerBase();

	/** Secondary 'root' point that moves with player input */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* PivotPoint;

	/** Using a box to calculate collisions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* HitArea;

	/** Static mesh of the player's character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* PawnMesh;

	/** The view point */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* PlayerCamera;

	/** Visual helper to setup the point from which player's projectiles are spawned */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UArrowComponent* FiringPoint;

	/** Using Niagara component to make engine effects */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	UNiagaraComponent* EngineEffectComponent;

	/** Using Niagara system to make player death effects */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	UNiagaraSystem* DeathEffectSystem;

	/** Spawn a Metasound sound when dying */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	USoundBase* DeathSound;

	/** Using Niagara system to make weapon firing effects */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player | Weapon")
	UNiagaraSystem* FiringFX;

	/** Spawn a Metasound sound when firing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player | Weapon")
	USoundBase* FiringSound;

	/** This projectile is spawned when firing basic weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player | Weapon")
	TSubclassOf<AActor> ProjectileToFire;

	/** Special projectile when firing railgun */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player | Weapon")
	TSubclassOf<AActor> SpecialWeaponRail;

	/** Special projectile when firing rocket */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player | Weapon")
	TSubclassOf<AActor> SpecialWeaponExplosion;

	/** Identifier used in collisions */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	FName TagName = "PlayerCharacter";

	/** Player's current lives */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float Lives = 3.0f;

	/** Default move speed of the player's character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float PlayerDefaultMoveSpeed = 500.0f;

	/** Default attack speed of the player's character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float PlayerDefaultAttackSpeed = 0.6f;

	/** Currently selected special weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	TEnumAsByte<ESpecialWeapon> SelectedSpecialWeapon;

	/** The amount of special weapon ammo */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float SpecialWeaponAmmo = 0.0f;

	/** Maximum movement distance player's character can move */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float PlayerMaxMovement = 2000.0f;

protected:
	/** Pointer to the game instace so we can save player character's status between levels */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game")
	USpaceInvadersGameInstanceBase* GameInst;

	/** Current move speed of the player's character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float PlayerCurrentMoveSpeed;

	/** Current attack speed of the player's character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	float PlayerCurrentAttackSpeed;

private:
	/** Current movement input amount */
	FVector CurrentVelocity;

	/** Wait timer between spawning projectiles */
	float FiringTimer = 0.0f;

	/** Check to see if a new projectile can be spawned */
	bool bCanFire = false;

	/** Check to allow automatic firing when basic weapon firing input is held down */
	bool bFiringTriggered = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called when basic weapon firing input is pressed */
	UFUNCTION(BlueprintCallable, Category = "Player | Actions")
	void StartPrimaryFire();

	/** Called when basic weapon firing input is released */
	UFUNCTION(BlueprintCallable, Category = "Player | Actions")
	void StopPrimaryFire();

	/** Called when special weapon firing input is pressed */
	UFUNCTION(BlueprintCallable, Category = "Player | Actions")
	void FireSpecial();

	/** Called when enemy projectile hits player's character */
	UFUNCTION(BlueprintCallable, Category = "Player | Actions")
	void DecreaseLives();

protected:
	/** Called when receving player input */
	void Move_YAxis(float AxisValue);

	/** Called when firing the basic weapon has started */
	UFUNCTION()
	virtual void FiringWeapon();

	/** Called when firing a projectile */
	UFUNCTION()
	virtual void SpawnProjectile(FVector SpawnLocation, FRotator SpawnRotation, TSubclassOf<AActor> SelectedWeapon);

public:
	/** Calculate hits to player character and use it to pickup special weapon ammo */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Called when player dies */
	UFUNCTION()
	virtual void Die();

	/** Called when player's lives has changed */
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateLivesWidget(float LivesAmount);

	/** Called when player's special ammo amount has changed */
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateAmmoWidget(float AmmoAmount);

	/** Called when player's score has changed */
	UFUNCTION(BlueprintImplementableEvent, Category = "Scoring")
	void UpdateScoring(float CurrentScore);

	/** Called when player has killed all enemies in level */
	UFUNCTION(BlueprintImplementableEvent, Category = "Game")
	void LevelFinished();

	/** Called when player has run out of lives */
	UFUNCTION(BlueprintImplementableEvent, Category = "Game")
	void GameOver();
};
