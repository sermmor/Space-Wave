// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "AeroHeroPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"

const FName AAeroHeroPawn::MoveForwardBinding("MoveForward");
const FName AAeroHeroPawn::MoveRightBinding("MoveRight");
//const FName AAeroHeroPawn::FireForwardBinding("FireForward");
const FName AAeroHeroPawn::FireNormal("FireNormal");

AAeroHeroPawn::AAeroHeroPawn()
{	
	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	RootComponent = CameraBoom;
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when ship does
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->RelativeRotation = FRotator(-80.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm

	UE_LOG(LogTemp, Warning, TEXT("VelocityCamera: %f"), VelocityCamera);

	IsFirePushed = false;
	AccelerationCamera = 1.0f;
}

void AAeroHeroPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	//PlayerInputComponent->BindAxis(FireForwardBinding);
	PlayerInputComponent->BindAction(FireNormal, EInputEvent::IE_Pressed, this, &AAeroHeroPawn::OnPushInFire);
	PlayerInputComponent->BindAction(FireNormal, EInputEvent::IE_Released, this, &AAeroHeroPawn::OnPushInFire);
	//PlayerInputComponent->BindAction(FireNormal, EInputEvent::IE_Repeat, this, &AAeroHeroPawn::OnPushInFire);
	UE_LOG(LogTemp, Warning, TEXT("VelocityCamera: %f"), VelocityCamera);
}

void AAeroHeroPawn::OnPushInFire()
{
	IsFirePushed = true;
	//UE_LOG(LogTemp, Warning, TEXT("IsFirePushed: %s"), IsFirePushed ? TEXT("True") : TEXT("False"));
}

void AAeroHeroPawn::Tick(float DeltaSeconds)
{
	// Find movement direction
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);
	//const float FireForwardValue = GetInputAxisValue(FireForwardBinding) > 0 ? 1 : 0;

	MyPlayerShip->UpdateInputs(ForwardValue, RightValue, IsFirePushed);

	IsFirePushed = false;

	// Camera forward movement.
	FVector MoveDirection = FVector(VelocityCamera * AccelerationCamera, 0, 0);
	FHitResult Hit(1.f);
	CameraComponent->MoveComponent(MoveDirection, CameraBoom->RelativeRotation, true, &Hit);
}


