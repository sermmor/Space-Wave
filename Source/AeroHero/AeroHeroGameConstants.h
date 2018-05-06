// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#pragma region Camera Limits Constants 
const float LeftLimit = 500.f;//1000.f;
const float RightLimit = -500.f;//-1000.f;
const float UpLimit = -1600.f;//-675.f;
const float DownLimit = 600.f;
#pragma endregion 

#pragma region Shoot Constants 
const float ShootLifeNormalEnemy = 0.4f;
const float ShootLifePlayer = 0.4f;
const float ShootLifeTowerEnemy = 0.2f;
#pragma endregion 

const int MaxPlayerLife = 100;
const int DefaultDamageProjectile = 1;
const int DefaultDamageMissile = 5;

class AEROHERO_API AeroHeroGameConstants
{
public:
	AeroHeroGameConstants();
	~AeroHeroGameConstants();
};
