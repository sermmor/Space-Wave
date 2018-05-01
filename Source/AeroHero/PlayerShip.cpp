// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerShip.h"
#include "AeroHeroProjectile.h"
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

	// Movement
	MoveSpeed = 1000.0f;
	// Weapon
	GunOffset = FVector(90.f, 0.f, 0.f);
	FireRate = 0.1f;
	bCanFire = true;
	IsFirePushed = false;
	// Camera position
	CameraBoomPosition = FVector(0.f, 0.f, 0.f);
}

// Called when the game starts or when spawned
void APlayerShip::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

	// Calculate  movement
	FVector Movement = MoveDirection * MoveSpeed * DeltaTime;

	// Quiet case.
	bool IsInQuietCase = ForwardValue == 0 && IsShirpInDownLimitCase(ForwardValue);

	// If non-zero size and movement is valid, move this actor
	if (IsShipMovementValid(Movement, RightValue, ForwardValue) || IsInQuietCase)
	{
		if (IsShirpInDownLimitCase(ForwardValue))
		{
			// Invert forward and calculate again MoveDirection and Movement.
			ForwardValue = (ForwardValue == 0)? 1 : -ForwardValue;
			MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);
			Movement = MoveDirection * MoveSpeed * DeltaTime;
		}

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
	FireShot(FireDirection);
	
}

void APlayerShip::FireShot(FVector FireDirection)
{
	// If it's ok to fire again
	if (bCanFire == true)
	{
		// If we are pressing fire stick in a direction
		if (FireDirection.SizeSquared() > 0.0f)
		{
			const FRotator FireRotation = FireDirection.Rotation();
			// Spawn projectile at an offset from this pawn
			const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				// spawn the projectile
				World->SpawnActor<AAeroHeroProjectile>(SpawnLocation, FireRotation);
			}

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &APlayerShip::ShotTimerExpired, FireRate);

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			bCanFire = false;
		}
	}
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

void APlayerShip::UpdateInputs(float forwardValue, float rightValue, bool isInvertFire)
{
	ForwardValue = forwardValue;
	RightValue = rightValue;
	
	if (isInvertFire)
		IsFirePushed = !IsFirePushed;
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

bool APlayerShip::IsShipMovementValid(FVector Movement, float Horizontal, float Vertical)
{
	return Movement.SizeSquared() > 0.0f && IsVerticalShipMovementValid(Vertical) && IsHorizontalShipMovementValid(Horizontal);
}

bool APlayerShip::IsShirpInDownLimitCase(float Vertical)
{
	FVector currentLocation = GetActorLocation();
	float diff = CameraBoomPosition.X - currentLocation.X;

	return (diff > DownLimit && Vertical <= 0);
}


