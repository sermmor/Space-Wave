// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PlayerShip.h"
#include "GameFramework/Character.h"
#include "AeroHeroPawn.generated.h"

UCLASS(Blueprintable)
class AAeroHeroPawn : public APawn
{
	GENERATED_BODY()

	/* The mesh component */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ShipMeshComponent;

	/** The camera */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Game Params")
	float VelocityCamera;// = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Pawns")
	TArray<APlayerShip*> AllPlayerShips;

	AAeroHeroPawn();
	
	void OnJumpNormalP1();
	void OnJumpNormalP2();
	void OnJumpNormalP3();
	void OnPushInFireP1();
	void OnPushInFireP2();
	void OnPushInFireP3();
	FVector GetCameraLocation();

	/** Sound to play each time we fire */
	UPROPERTY(Category = Audio, EditAnywhere, BlueprintReadWrite)
	class USoundBase* FireSound;

	// Begin Actor Interface
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End Actor Interface

	// Static names for axis bindings
	static const FName MoveForwardBindingP1;
	static const FName MoveRightBindingP1;
	static const FName FireNormalP1;
	static const FName JumpNormalP1;
	static const FName MoveForwardBindingP2;
	static const FName MoveRightBindingP2;
	static const FName FireNormalP2;
	static const FName JumpNormalP2;
	static const FName MoveForwardBindingP3;
	static const FName MoveRightBindingP3;
	static const FName FireNormalP3;
	static const FName JumpNormalP3;
	//static const FName FireForwardBinding;

private:
	bool IsPushedJumpP1, IsPushedJumpP2, IsPushedJumpP3;
	bool IsFirePushedP1, IsFirePushedP2, IsFirePushedP3;
	bool IsDeathP1, IsDeathP2, IsDeathP3;

	void CheckPlayersDeaths();
public:
	float AccelerationCamera;

	/** Returns CameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};

