// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerShip.h"
#include "HPItem.h"
#include "AeroHeroGameConstants.h"
#include "PlayerProjectile.h"
#include "PlayerMissileProjectile.h"
#include "MissileItem.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

// Sets default values
APlayerShip::APlayerShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("/Game/TwinStick/Meshes/TwinStickUFO.TwinStickUFO"));
	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);

	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

	// Classes references for events.
	auto EnemyProjectileLoadClass = ConstructorHelpers::FClassFinder<AActor>(TEXT("/Script/AeroHero.EnemyProjectile"));
	if (EnemyProjectileLoadClass.Succeeded())
		EnemyProjectileClass = EnemyProjectileLoadClass.Class;

	auto HPItemLoadClass = ConstructorHelpers::FClassFinder<AActor>(TEXT("/Script/AeroHero.HPItem"));
	if (HPItemLoadClass.Succeeded())
		HPItemClass = HPItemLoadClass.Class;

	// Movement
	MoveSpeed = 1000.0f;
	JumpRate = 0.3f;
	JumpState = NO_JUMPING;
	IsJumpPushed = false;

	// Weapon
	GunOffset = FVector(90.f, 0.f, 0.f);
	GunLeftOffset = FVector(90.f, -20.f, 0.f);
	GunRightOffset = FVector(90.f, 20.f, 0.f);
	FireRate = 0.1f;
	TypeFire = FIRE_NORMAL;
	bCanFire = true;
	IsFirePushed = false;

	// Camera position
	CameraBoomPosition = FVector(0.f, 0.f, 0.f);

	// Hurt.
	bIsInHurtTime = false;
	HurtTimeDuration = 0.7f;

	// Life & Score.
	Life = MaxPlayerLife;
	Score = 0;
}

// Called when the game starts or when spawned
void APlayerShip::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	playerCtrl = World->GetFirstPlayerController();
	World = NULL;
}

// Called every frame
void APlayerShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If Jump is pushed, do jump.
	if (JumpState == NO_JUMPING && IsJumpPushed)
		JumpState = JUMP_BEGINS;

	if (!IsPlayerOne && !IsPlayerTwo) // If is player 3, is mouse, so increase ForwardValue.
	{
		if (ForwardValue > 0.1f)
			ForwardValue *= 3.f;
		if (RightValue > 0.1f)
			RightValue *= 10000.f;
	}

	// Get de current jump movement.
	float jumpMovement = CheckJumpingMovement();

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	FVector MoveDirection = FVector(ForwardValue, RightValue, jumpMovement).GetClampedToMaxSize(1.0f);

	// Calculate  movement
	FVector Movement = MoveDirection * MoveSpeed * DeltaTime;

	// Quiet case.
	bool IsInQuietCase = ForwardValue == 0 && IsShirpInDownLimitCase(ForwardValue);

	// If non-zero size and movement is valid, move this actor
	if (Movement.SizeSquared() > 0.0f || IsInQuietCase)
	{
		if (!IsHorizontalShipMovementValid(RightValue))
		{
			// Put RightValue to zero and calculate again MoveDirection and Movement.
			RightValue = 0;
			MoveDirection = FVector(ForwardValue, RightValue, jumpMovement).GetClampedToMaxSize(1.0f);
			Movement = MoveDirection * MoveSpeed * DeltaTime;
		}

		if (!IsVerticalShipMovementValid(ForwardValue))
		{
			// Put LeftValue to zero and calculate again MoveDirection and Movement.
			ForwardValue = 0;
			MoveDirection = FVector(ForwardValue, RightValue, jumpMovement).GetClampedToMaxSize(1.0f);
			Movement = MoveDirection * MoveSpeed * DeltaTime;
		}

		if (IsShirpInDownLimitCase(ForwardValue))
		{
			// Invert forward and calculate again MoveDirection and Movement.
			ForwardValue = (ForwardValue == 0)? 1 : -ForwardValue;
			MoveDirection = FVector(ForwardValue, RightValue, jumpMovement).GetClampedToMaxSize(1.0f);
			Movement = MoveDirection * MoveSpeed * DeltaTime;
		}

		// Do Movement.
		const FRotator NewRotation = VectorZero.Rotation();
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);

		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, NewRotation, true);
		}
	}

	// Create fire direction vector
	float FireForwardValue = IsFirePushed ? 1 : 0;
	const FVector FireDirection = FVector(FireForwardValue, 0, 0.f);

	// Try and fire a shot
	if (TypeFire == FIRE_DOUBLE)
		bCanFire = FireDoubleShot(FireDirection, bCanFire);
	else
		bCanFire = FireShot(FireDirection, bCanFire);
}

float APlayerShip::CheckJumpingMovement()
{
	const float jumpImpulse = 0.5f;

	if (JumpState == JUMP_BEGINS)
	{
		// Prepare jump.
		TWeakObjectPtr<UWorld> World = GetWorld();
		World->GetTimerManager().SetTimer(TimerHandle_JumpTimerExpired, this, &APlayerShip::JumpTimerExpired, JumpRate);
		JumpState = JUMP_WHILE;
	}
	else if (JumpState == JUMP_WHILE)
	{
		// Jumping.
		return jumpImpulse;
	}
	else if (JumpState == JUMP_FALLING)
	{
		// Falling.
		return -jumpImpulse;
	}

	return 0.f;
}

void APlayerShip::JumpTimerExpired()
{
	TWeakObjectPtr<UWorld> World = GetWorld();
	World->GetTimerManager().SetTimer(TimerHandle_FallTimerExpired, this, &APlayerShip::FallTimerExpired, JumpRate);
	JumpState = JUMP_FALLING;
}

void APlayerShip::FallTimerExpired()
{
	JumpState = NO_JUMPING;
}


bool APlayerShip::FireShot(FVector FireDirection, bool canFire)
{
	bool canFireToReturn = canFire;
	// If it's ok to fire again
	if (canFireToReturn == true)
	{
		// If we are pressing fire stick in a direction
		if (FireDirection.SizeSquared() > 0.0f)
		{
			const FRotator FireRotation = FireDirection.Rotation();
			// Spawn projectile at an offset from this pawn
			const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

			TWeakObjectPtr<UWorld> World = GetWorld();

			if (World != NULL)
			{
				// spawn the projectile
				if (TypeFire == FIRE_MISSILE)
				{
					TWeakObjectPtr<APlayerMissileProjectile> playerProjectile = World->SpawnActor<APlayerMissileProjectile>(SpawnLocation, FireRotation);
					playerProjectile->SetPlayerShip(this);
				}
				else
				{
					TWeakObjectPtr<APlayerProjectile> playerProjectile = World->SpawnActor<APlayerProjectile>(SpawnLocation, FireRotation);
					playerProjectile->SetPlayerShip(this);
				}
				World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &APlayerShip::ShotTimerExpired, FireRate);
			}

			canFireToReturn = false;

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			canFireToReturn = false;
		}
	}
	return canFireToReturn;
}

bool APlayerShip::FireDoubleShot(FVector FireDirection, bool canFire)
{
	bool canFireToReturn = canFire;
	// If it's ok to fire again
	if (canFireToReturn == true)
	{
		// If we are pressing fire stick in a direction
		if (FireDirection.SizeSquared() > 0.0f)
		{
			const FRotator FireRotation = FireDirection.Rotation();
			// Spawn projectile at an offset from this pawn
			const FVector SpawnLocationLeft = GetActorLocation() + FireRotation.RotateVector(GunLeftOffset);
			const FVector SpawnLocationRight = GetActorLocation() + FireRotation.RotateVector(GunRightOffset);

			TWeakObjectPtr<UWorld> World = GetWorld();

			if (World != NULL)
			{
				// spawn the projectiles
				TWeakObjectPtr<APlayerProjectile> playerProjectileLeft = World->SpawnActor<APlayerProjectile>(SpawnLocationLeft, FireRotation);
				playerProjectileLeft->SetPlayerShip(this);
				TWeakObjectPtr<APlayerProjectile> playerProjectileRight = World->SpawnActor<APlayerProjectile>(SpawnLocationRight, FireRotation);
				playerProjectileRight->SetPlayerShip(this);
				World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &APlayerShip::ShotTimerExpired, FireRate);
			}

			canFireToReturn = false;

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			canFireToReturn = false;
		}
	}
	return canFireToReturn;
}

void APlayerShip::ShotTimerExpired()
{
	bCanFire = true;
}

// Called to bind functionality to input
void APlayerShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
}

void APlayerShip::UpdateInputsP1(float forwardValue, float rightValue, bool isInvertFire, bool isJumpPushed)
{
	if (IsPlayerOne)
	{
		ForwardValue = forwardValue;
		RightValue = rightValue;
		IsJumpPushed = isJumpPushed;
	
		if (isInvertFire)
			IsFirePushed = !IsFirePushed;
	}
}

void APlayerShip::UpdateInputsP2(float forwardValue, float rightValue, bool isInvertFire, bool isJumpPushed)
{
	if (!IsPlayerOne && IsPlayerTwo)
	{
		ForwardValue = forwardValue;
		RightValue = rightValue;
		IsJumpPushed = isJumpPushed;

		if (isInvertFire)
			IsFirePushed = !IsFirePushed;
	}
}

void APlayerShip::UpdateInputsP3(float forwardValue, float rightValue, bool isInvertFire, bool isJumpPushed)
{
	if (!IsPlayerOne && !IsPlayerTwo)
	{
		ForwardValue = forwardValue;
		RightValue = rightValue;
		IsJumpPushed = isJumpPushed;

		if (isInvertFire)
			IsFirePushed = !IsFirePushed;
	}
}

void APlayerShip::UpdateCameraBoomLocation(float x, float y, float z)
{
	CameraBoomPosition.X = x;
	CameraBoomPosition.Y = y;
	CameraBoomPosition.Z = z;
}

bool APlayerShip::IsHorizontalShipMovementValid(float Horizontal)
{
	FVector currentLocation = GetActorLocation();
	float diff = CameraBoomPosition.Y - currentLocation.Y;

	if (diff > LeftLimit && Horizontal < 0) // Left Limit.
	{
		return false;
	}
	else if (diff < RightLimit && Horizontal > 0) // Right Limit.
	{
		return false;
	}

	return true;
}

bool APlayerShip::IsVerticalShipMovementValid(float Vertical)
{
	FVector currentLocation = GetActorLocation();
	float diff = CameraBoomPosition.X - currentLocation.X;
	
	if (diff < UpLimit && Vertical > 0) // Up Limit.
	{
		return false;
	}

	return true;
}

bool APlayerShip::IsShirpInDownLimitCase(float Vertical)
{
	FVector currentLocation = GetActorLocation();
	float diff = CameraBoomPosition.X - currentLocation.X;

	return (diff > DownLimit && Vertical <= 0);
}

float APlayerShip::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{

	if (bIsInHurtTime)
	{
		return 0.f;
	}

	// Vibration
	if (IsPlayerOne && playerCtrl != NULL)
	{
		if (DamageCauser == NULL || DamageCauser->IsA(EnemyProjectileClass)) // It's a bullet (could be destroyed before).
		{
			FName tag = "testShake";
			playerCtrl->ClientPlayForceFeedback(ForceFeedbackEffectShoot, false, true, tag);
			//UE_LOG(LogTemp, Warning, TEXT("VIBRATION PROJECTILE. Player recieve %f damage."), DamageAmount);
		}
		else // Collission with enemy case.
		{
			FName tag = "testShake";
			playerCtrl->ClientPlayForceFeedback(ForceFeedbackEffectCollideWithSomething, false, true, tag);
			//UE_LOG(LogTemp, Warning, TEXT("VIBRATION ENEMY. Player recieve %f damage."), DamageAmount);
		}
	}

	// Decrease player life.
	Life -= DamageAmount;
	if (Life < 0)
		Destroy(); // TODO CREATE EXPLOSION PARTICLES.

	// Put in hurt time.
	bIsInHurtTime = true;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_HurtTimerExpired, this, &APlayerShip::HurtTimerExpired, HurtTimeDuration);
	
	return 0.f;
}

void APlayerShip::UpdateScore(int PointsToAdd)
{
	Score += PointsToAdd;
}

void APlayerShip::HurtTimerExpired()
{
	bIsInHurtTime = false;
}

void APlayerShip::GetHPItem(int pointsToRecover)
{
	Life += pointsToRecover;
	if (Life > MaxPlayerLife)
		Life = MaxPlayerLife;
}

void APlayerShip::SetDoubleShootMode()
{
	TypeFire = FIRE_DOUBLE;
}

void APlayerShip::SetMissileShootMode()
{
	TypeFire = FIRE_MISSILE;
}
