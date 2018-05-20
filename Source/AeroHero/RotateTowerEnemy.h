// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TowerEnemy.h"
#include "RotateTowerEnemy.generated.h"

const float RADIUS = 1000.0f;

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AEROHERO_API URotateTowerEnemy : public UTowerEnemy
{
	GENERATED_BODY()
	
public:
	URotateTowerEnemy();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotate Tower Params")
	bool IsVerticalRotation;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual FVector PrepareShootDirection() override;

private:
	float thetaRotation;
	
	UPROPERTY()
	UStaticMeshComponent* StaticMesh;
};
