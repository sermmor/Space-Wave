// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NormalEnemy.h"
#include "AsteroidEnemy.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AEROHERO_API UAsteroidEnemy : public UNormalEnemy
{
	GENERATED_BODY()
	
public:
	UAsteroidEnemy();
	
protected:
	virtual bool CanShoot() override;
	
};
