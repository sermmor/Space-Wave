// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerShip.generated.h"

UCLASS()
class AEROHERO_API APlayerShip : public APawn
{
	GENERATED_BODY()

	/* The mesh component */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ShipMeshComponent;

public:
	// Sets default values for this pawn's properties
	APlayerShip();

	enum JUMP_STATE { NO_JUMPING, JUMP_BEGINS, JUMP_WHILE, JUMP_FALLING };

	/** Offset from the ships location to spawn projectiles */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	FVector GunOffset;

	/* How fast the weapon will fire */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float FireRate;

	/* How fast the ship will jump and fall. */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float JumpRate;

	/* The speed our ship moves around the level */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;

	/* Check if is player 1, player 3 or player 3 */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	bool IsPlayerOne;

	/* Check if is player 1, player 3 or player 3 */
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	bool IsPlayerTwo;

	/** Sound to play each time we fire */
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
	class USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vibration")
	class UForceFeedbackEffect* ForceFeedbackEffectShoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vibration")
	class UForceFeedbackEffect* ForceFeedbackEffectCollideWithSomething;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vibration")
	class UForceFeedbackEffect* ForceFeedbackEffectHighDamage;

	/* Fire a shot in the specified direction */
	void FireShot(FVector FireDirection);

	/* Handler for the fire timer expiry */
	UFUNCTION()
	void ShotTimerExpired();

	/* Handler for the hurt timer expiry */
	UFUNCTION()
	void HurtTimerExpired();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player params")
	int Life;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player params")
	int Score;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void UpdateInputsP1(float forwardValue, float rightValue, bool isInvertFire, bool isJumpPushed);
	void UpdateInputsP2(float forwardValue, float rightValue, bool isInvertFire, bool isJumpPushed);
	void UpdateInputsP3(float forwardValue, float rightValue, bool isInvertFire, bool isJumpPushed);

	void UpdateCameraBoomLocation(float x, float y, float z);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

	void UpdateScore(int PointsToAdd);

private:

	JUMP_STATE JumpState;

	/* Is in a invencible time before receive a damage. */
	bool bIsInHurtTime;

	/* Invencible time before receive a damage. */
	float HurtTimeDuration;

	APlayerController * playerCtrl;

	TSubclassOf<AActor> EnemyProjectileClass;

	// Vector Zero.
	const FVector VectorZero = FVector(0, 0, 0);

	/* Flag to control firing  */
	uint32 bCanFire : 1;

	/** Handle for efficient management of ShotTimerExpired timer */
	FTimerHandle TimerHandle_ShotTimerExpired;

	/** Handle for efficient management of HurtTimerExpired timer */
	FTimerHandle TimerHandle_HurtTimerExpired;

	/** Handle for efficient management of JumpTimerExpired timer */
	FTimerHandle TimerHandle_JumpTimerExpired;

	/** Handle for efficient management of FallTimerExpired timer */
	FTimerHandle TimerHandle_FallTimerExpired;

	FVector CameraBoomPosition;

	float ForwardValue;
	float RightValue;
	bool IsFirePushed;
	bool IsJumpPushed;

	float CheckJumpingMovement();
	void JumpTimerExpired();
	void FallTimerExpired();
	bool IsHorizontalShipMovementValid(float Horizontal);
	bool IsVerticalShipMovementValid(float Vertical);
	bool IsShirpInDownLimitCase(float Vertical);

public:
	/** Returns ShipMeshComponent subobject **/
	FORCEINLINE class UStaticMeshComponent* GetShipMeshComponent() const { return ShipMeshComponent; }
};
