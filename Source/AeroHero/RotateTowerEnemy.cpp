// Fill out your copyright notice in the Description page of Project Settings.

#include "RotateTowerEnemy.h"
#include "EngineMinimal.h"
#include "EngineUtils.h"
#include "App.h"

URotateTowerEnemy::URotateTowerEnemy()
{
	thetaRotation = 0;
}

// Called when the game starts
void URotateTowerEnemy::BeginPlay()
{
	Super::BeginPlay();

	TArray<UStaticMeshComponent*> Components;
	Super::GetOwner()->GetComponents<UStaticMeshComponent>(Components);

	if (Components.Num() > 0)
		StaticMesh = Components[0];
}

void URotateTowerEnemy::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/* Rotate looking at circle movement.
	* So increment thetaRotation with DeltaTime, use circle formula, and add new relative rotation value.*/
	thetaRotation += 2.0f * FApp::GetDeltaTime();
	if (thetaRotation >= 360.0f)
		thetaRotation = 0.0f;

	// Circle formula (center in (0, 0, 0)).
	FVector lookAt(RADIUS*FMath::Cos(thetaRotation), RADIUS*FMath::Sin(thetaRotation), 0.0f);

	FRotator EnemyRotation = FRotationMatrix::MakeFromX(lookAt).Rotator();
	StaticMesh->SetRelativeRotation(EnemyRotation, false, nullptr, ETeleportType::TeleportPhysics);
	
	// Prepare direction to shoot.
	ShootDirection = EnemyRotation.RotateVector(GunOffset);
}


FVector URotateTowerEnemy::PrepareShootDirection()
{
	FVector FireDirection = FVector(ShootDirection.X, ShootDirection.Y, ShootDirection.Z);
	return FireDirection;
}
