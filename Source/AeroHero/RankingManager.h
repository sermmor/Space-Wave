// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveRanking.h"
#include "RankingManager.generated.h"

UCLASS()
class AEROHERO_API ARankingManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARankingManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Load")
	TArray<int> GetScores() const;

	UFUNCTION(BlueprintCallable, Category = "Load")
	TArray<FString> GetNames() const;

	UFUNCTION(BlueprintCallable, Category = "Save")
	void SaveNewRecord(FString name, int score);

	void OnDestroyGameController(AActor* SelfActor);
private:
	UPROPERTY()
	USaveRanking* SavesGameInstance;
	TArray<FString> NamePlayers;
	TArray<int> ScorePlayers;

	void CopyTopTen(TArray<FString> &AllNamesFrom, TArray<int> &AllScoresFrom, TArray<FString> &AllNamesTo, TArray<int> &AllScoresTo);
	void InsertNewRecordInOrder(FString name, int score);
	void ReverseScorePlayers();
	void SaveCurrentRanking();
	
	
};
