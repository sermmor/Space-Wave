// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "AeroHeroPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"

const FName AAeroHeroPawn::MoveForwardBindingP1("MoveForward_P1");
const FName AAeroHeroPawn::MoveRightBindingP1("MoveRight_P1");
const FName AAeroHeroPawn::FireNormalP1("FireNormal_P1");
const FName AAeroHeroPawn::MoveForwardBindingP2("MoveForward_P2");
const FName AAeroHeroPawn::MoveRightBindingP2("MoveRight_P2");
const FName AAeroHeroPawn::FireNormalP2("FireNormal_P2");

const FName AAeroHeroPawn::MoveForwardBindingP3("MoveForward_P3");
const FName AAeroHeroPawn::MoveRightBindingP3("MoveRight_P3");
const FName AAeroHeroPawn::FireNormalP3("FireNormal_P3");
//const FName AAeroHeroPawn::FireForwardBinding("FireForward");

AAeroHeroPawn::AAeroHeroPawn()
{	
	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	RootComponent = CameraBoom;
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when ship does
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->RelativeRotation = FRotator(-80.f, 0.f, 0.f);//FRotator(-30.f, 0.f, 0.f);//-29.999561 °
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm

	//UE_LOG(LogTemp, Warning, TEXT("VelocityCamera: %f"), VelocityCamera);

	IsFirePushedP1 = false;
	IsFirePushedP2 = false;
	IsFirePushedP3 = false;
	AccelerationCamera = 1.0f;

}

void AAeroHeroPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up gameplay key bindings Player 1.
	PlayerInputComponent->BindAxis(MoveForwardBindingP1);
	PlayerInputComponent->BindAxis(MoveRightBindingP1);
	PlayerInputComponent->BindAction(FireNormalP1, EInputEvent::IE_Pressed, this, &AAeroHeroPawn::OnPushInFireP1);
	PlayerInputComponent->BindAction(FireNormalP1, EInputEvent::IE_Released, this, &AAeroHeroPawn::OnPushInFireP1);

	// set up gameplay key bindings Player 2.
	PlayerInputComponent->BindAxis(MoveForwardBindingP2);
	PlayerInputComponent->BindAxis(MoveRightBindingP2);
	PlayerInputComponent->BindAction(FireNormalP2, EInputEvent::IE_Pressed, this, &AAeroHeroPawn::OnPushInFireP2);
	PlayerInputComponent->BindAction(FireNormalP2, EInputEvent::IE_Released, this, &AAeroHeroPawn::OnPushInFireP2);

	// set up gameplay key bindings Player 3.
	PlayerInputComponent->BindAxis(MoveForwardBindingP3);
	PlayerInputComponent->BindAxis(MoveRightBindingP3);
	PlayerInputComponent->BindAction(FireNormalP3, EInputEvent::IE_Pressed, this, &AAeroHeroPawn::OnPushInFireP3);
	PlayerInputComponent->BindAction(FireNormalP3, EInputEvent::IE_Released, this, &AAeroHeroPawn::OnPushInFireP3);

	//PlayerInputComponent->BindAxis(FireForwardBinding);
	//PlayerInputComponent->BindAction(FireNormal, EInputEvent::IE_Repeat, this, &AAeroHeroPawn::OnPushInFire);
	//UE_LOG(LogTemp, Warning, TEXT("VelocityCamera: %f"), VelocityCamera);
}

void AAeroHeroPawn::OnPushInFireP1()
{
	IsFirePushedP1 = true;
	//UE_LOG(LogTemp, Warning, TEXT("IsFirePushed: %s"), IsFirePushed1 ? TEXT("True") : TEXT("False"));
}

void AAeroHeroPawn::OnPushInFireP2()
{
	IsFirePushedP2 = true;
	//UE_LOG(LogTemp, Warning, TEXT("IsFirePushed: %s"), IsFirePushed2 ? TEXT("True") : TEXT("False"));
}

void AAeroHeroPawn::OnPushInFireP3()
{
	IsFirePushedP3 = true;
	//UE_LOG(LogTemp, Warning, TEXT("IsFirePushed: %s"), IsFirePushed3 ? TEXT("True") : TEXT("False"));
}

void AAeroHeroPawn::Tick(float DeltaSeconds)
{
	// Find movement direction
	const float ForwardValueP1 = GetInputAxisValue(MoveForwardBindingP1);
	const float RightValueP1 = GetInputAxisValue(MoveRightBindingP1);
	const float ForwardValueP2 = GetInputAxisValue(MoveForwardBindingP2);
	const float RightValueP2 = GetInputAxisValue(MoveRightBindingP2);
	const float ForwardValueP3 = GetInputAxisValue(MoveForwardBindingP3);
	const float RightValueP3 = GetInputAxisValue(MoveRightBindingP3);
	//const float FireForwardValue = GetInputAxisValue(FireForwardBinding) > 0 ? 1 : 0;

	for (APlayerShip* MyPlayerShip : AllPlayerShips)
	{
		MyPlayerShip->UpdateInputsP1(ForwardValueP1, RightValueP1, IsFirePushedP1);
		MyPlayerShip->UpdateInputsP2(ForwardValueP2, RightValueP2, IsFirePushedP2);
		MyPlayerShip->UpdateInputsP3(ForwardValueP3, RightValueP3, IsFirePushedP3);
	}

	IsFirePushedP1 = false;
	IsFirePushedP2 = false;
	IsFirePushedP3 = false;

	// Camera forward movement.
	FVector MoveDirection = FVector(VelocityCamera * AccelerationCamera, 0, 0);
	FHitResult Hit(1.f);
	CameraBoom->MoveComponent(MoveDirection, CameraBoom->RelativeRotation, true, &Hit);

	for (APlayerShip* MyPlayerShip : AllPlayerShips)
		MyPlayerShip->UpdateCameraBoomLocation(CameraBoom->GetComponentLocation().X, 
			CameraBoom->GetComponentLocation().Y, 
			CameraBoom->GetComponentLocation().Z);

	//FVector currentLocation = CameraBoom->GetComponentLocation();
	//UE_LOG(LogTemp, Warning, TEXT("CameraBoom Location: (%f, %f, %f)"), currentLocation.X, currentLocation.Y, currentLocation.Z);
}

FVector AAeroHeroPawn::GetCameraLocation()
{
	if (CameraBoom != NULL)
		return CameraBoom->GetComponentLocation();
	else
		return FVector(0, 0, 0);
}
