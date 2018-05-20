// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "AeroHeroPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"

const FName AAeroHeroPawn::MoveForwardBindingP1("MoveForward_P1");
const FName AAeroHeroPawn::MoveRightBindingP1("MoveRight_P1");
const FName AAeroHeroPawn::FireNormalP1("FireNormal_P1");
const FName AAeroHeroPawn::JumpNormalP1("JumpNormal_P1");

const FName AAeroHeroPawn::MoveForwardBindingP2("MoveForward_P2");
const FName AAeroHeroPawn::MoveRightBindingP2("MoveRight_P2");
const FName AAeroHeroPawn::FireNormalP2("FireNormal_P2");
const FName AAeroHeroPawn::JumpNormalP2("JumpNormal_P2");

const FName AAeroHeroPawn::MoveForwardBindingP3("MoveForward_P3");
const FName AAeroHeroPawn::MoveRightBindingP3("MoveRight_P3");
const FName AAeroHeroPawn::FireNormalP3("FireNormal_P3");
const FName AAeroHeroPawn::JumpNormalP3("JumpNormal_P3");

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

	IsFirePushedP1 = IsFirePushedP2 = IsFirePushedP3 = false;
	IsPushedJumpP1 = IsPushedJumpP2 = IsPushedJumpP3 = false;
	IsDeathP1 = IsDeathP2 = IsDeathP3 = false;
	AccelerationCamera = 2.0f;

}

void AAeroHeroPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up gameplay key bindings Player 1.
	PlayerInputComponent->BindAxis(MoveForwardBindingP1);
	PlayerInputComponent->BindAxis(MoveRightBindingP1);
	PlayerInputComponent->BindAction(FireNormalP1, EInputEvent::IE_Pressed, this, &AAeroHeroPawn::OnPushInFireP1);
	PlayerInputComponent->BindAction(FireNormalP1, EInputEvent::IE_Released, this, &AAeroHeroPawn::OnPushInFireP1);
	//PlayerInputComponent->BindAction(JumpNormalP1, EInputEvent::IE_Pressed, this, &AAeroHeroPawn::OnJumpNormalP1);

	// set up gameplay key bindings Player 2.
	PlayerInputComponent->BindAxis(MoveForwardBindingP2);
	PlayerInputComponent->BindAxis(MoveRightBindingP2);
	PlayerInputComponent->BindAction(FireNormalP2, EInputEvent::IE_Pressed, this, &AAeroHeroPawn::OnPushInFireP2);
	PlayerInputComponent->BindAction(FireNormalP2, EInputEvent::IE_Released, this, &AAeroHeroPawn::OnPushInFireP2);
	//PlayerInputComponent->BindAction(JumpNormalP2, EInputEvent::IE_Pressed, this, &AAeroHeroPawn::OnJumpNormalP2);

	// set up gameplay key bindings Player 3.
	PlayerInputComponent->BindAxis(MoveForwardBindingP3);
	PlayerInputComponent->BindAxis(MoveRightBindingP3);
	PlayerInputComponent->BindAction(FireNormalP3, EInputEvent::IE_Pressed, this, &AAeroHeroPawn::OnPushInFireP3);
	PlayerInputComponent->BindAction(FireNormalP3, EInputEvent::IE_Released, this, &AAeroHeroPawn::OnPushInFireP3);
	//PlayerInputComponent->BindAction(JumpNormalP3, EInputEvent::IE_Pressed, this, &AAeroHeroPawn::OnJumpNormalP3);
	
}

void AAeroHeroPawn::OnJumpNormalP1()
{
	IsPushedJumpP1 = true;
}

void AAeroHeroPawn::OnJumpNormalP2()
{
	IsPushedJumpP2 = true;
}

void AAeroHeroPawn::OnJumpNormalP3()
{
	IsPushedJumpP3 = true;
}

void AAeroHeroPawn::OnPushInFireP1()
{
	IsFirePushedP1 = true;
}

void AAeroHeroPawn::OnPushInFireP2()
{
	IsFirePushedP2 = true;
}

void AAeroHeroPawn::OnPushInFireP3()
{
	IsFirePushedP3 = true;
}

void AAeroHeroPawn::Tick(float DeltaSeconds)
{
	if (AllPlayerShips.Num() == 0)
	{
		if (CameraBoom != NULL)
		{
			// Camera forward movement.
			FVector MoveDirection = FVector(VelocityCamera * AccelerationCamera * 2, 0, 0);
			FHitResult Hit(1.f);
			CameraBoom->MoveComponent(MoveDirection, CameraBoom->RelativeRotation, true, &Hit);
		}

		return;
	}

	// Find movement direction
	const float ForwardValueP1 = GetInputAxisValue(MoveForwardBindingP1);
	const float RightValueP1 = GetInputAxisValue(MoveRightBindingP1);
	const float ForwardValueP2 = GetInputAxisValue(MoveForwardBindingP2);
	const float RightValueP2 = GetInputAxisValue(MoveRightBindingP2);
	const float ForwardValueP3 = GetInputAxisValue(MoveForwardBindingP3);
	const float RightValueP3 = GetInputAxisValue(MoveRightBindingP3);
	
	CheckPlayersDeaths();

	int index = 0;
	for (APlayerShip* MyPlayerShip : AllPlayerShips)
	{
		if (MyPlayerShip == NULL || (index == 0 && IsDeathP1) || (index == 1 && IsDeathP2) || (index == 2 && IsDeathP3))
			continue; // CHECK IF A PLAYER IS DEATH.

		MyPlayerShip->UpdateInputsP1(ForwardValueP1, RightValueP1, IsFirePushedP1, IsPushedJumpP1);
		MyPlayerShip->UpdateInputsP2(ForwardValueP2, RightValueP2, IsFirePushedP2, IsPushedJumpP2);
		MyPlayerShip->UpdateInputsP3(ForwardValueP3, RightValueP3, IsFirePushedP3, IsPushedJumpP3);
		index++;
	}

	IsFirePushedP1 = IsFirePushedP2 = IsFirePushedP3 = false;
	IsPushedJumpP1 = IsPushedJumpP2 = IsPushedJumpP3 = false;

	// Camera forward movement.
	FVector MoveDirection = FVector(VelocityCamera * AccelerationCamera, 0, 0);
	FHitResult Hit(1.f);
	CameraBoom->MoveComponent(MoveDirection, CameraBoom->RelativeRotation, true, &Hit);
	
	index = 0;
	for (APlayerShip* MyPlayerShip : AllPlayerShips)
	{
		if (MyPlayerShip == NULL || (index == 0 && IsDeathP1) || (index == 1 && IsDeathP2) || (index == 2 && IsDeathP3))
			continue; // CHECK IF A PLAYER IS DEATH.

		MyPlayerShip->UpdateCameraBoomLocation(CameraBoom->GetComponentLocation().X, 
			CameraBoom->GetComponentLocation().Y, 
			CameraBoom->GetComponentLocation().Z);

		index++;
	}
}

void AAeroHeroPawn::CheckPlayersDeaths()
{
	if (AllPlayerShips.Num() == 0)
	{
		IsDeathP1 = IsDeathP2 = IsDeathP3 = true;
		return;
	}

	if (!IsDeathP1 && AllPlayerShips.Num() > 0 && AllPlayerShips[0]->IsDeath)
		IsDeathP1 = true;

	if (!IsDeathP2 && AllPlayerShips.Num() > 1 && AllPlayerShips[1]->IsDeath)
		IsDeathP2 = true;

	if (!IsDeathP3 && AllPlayerShips.Num() > 2 && AllPlayerShips[2]->IsDeath)
		IsDeathP3 = true;
}

FVector AAeroHeroPawn::GetCameraLocation()
{
	if (CameraBoom != NULL)
		return CameraBoom->GetComponentLocation();
	else
		return FVector(0, 0, 0);
}
