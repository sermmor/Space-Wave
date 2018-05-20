// Fill out your copyright notice in the Description page of Project Settings.

#include "RankingManager.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ARankingManager::ARankingManager() : SavesGameInstance(0)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARankingManager::BeginPlay()
{
	Super::BeginPlay();

	if (UGameplayStatics::DoesSaveGameExist(DefaultSaveSlotName, DefaultUserIndex))
	{
		SavesGameInstance = Cast<USaveRanking>(UGameplayStatics::LoadGameFromSlot(DefaultSaveSlotName, DefaultUserIndex));
	}
	else
	{
		// If not exist saves, we create the first saves.
		SavesGameInstance = Cast<USaveRanking>(UGameplayStatics::CreateSaveGameObject(USaveRanking::StaticClass()));
		UGameplayStatics::SaveGameToSlot(SavesGameInstance, DefaultSaveSlotName, DefaultUserIndex);
	}
	CopyTopTen(SavesGameInstance->NamePlayer, SavesGameInstance->ScorePlayer, NamePlayers, ScorePlayers);
}

void ARankingManager::CopyTopTen(TArray<FString> &AllNamesFrom, TArray<int> &AllScoresFrom,
	TArray<FString> &AllNamesTo, TArray<int> &AllScoresTo)
{
	if (AllScoresFrom.Num() > 0)
	{
		int end = AllScoresFrom.Num() < 10 ? AllScoresFrom.Num() : 10;
		for (int i = 0; i < end; i++)
		{
			if (AllNamesTo.Num() <= i)
			{
				AllNamesTo.Add(AllNamesFrom[i]);
				AllScoresTo.Add(AllScoresFrom[i]);
			}
			else
			{
				AllNamesTo[i] = AllNamesFrom[i];
				AllScoresTo[i] = AllScoresFrom[i];
			}

		}
	}
}

// Called every frame
void ARankingManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<int> ARankingManager::GetScores() const
{
	return ScorePlayers;
}

TArray<FString> ARankingManager::GetNames() const
{
	return NamePlayers;
}

void ARankingManager::SaveNewRecord(FString name, int score)
{
	InsertNewRecordInOrder(name, score);
	SaveCurrentRanking();
}

void ARankingManager::InsertNewRecordInOrder(FString name, int score)
{
	if (ScorePlayers.Num() == 0)
	{
		NamePlayers.Add(name);
		ScorePlayers.Add(score);
	}
	else
	{
		/* Insert O(n): insert score at last, order score list (a < b), reverse (a > b) and search position for to know where insert name. */
		ScorePlayers.Add(score);
		ScorePlayers.Sort();
		ReverseScorePlayers();
		int position = ScorePlayers.Find(score);

		if (position < NamePlayers.Num())
			NamePlayers.Insert(name, position);
		else
			NamePlayers.Add(name);
	}
}

void ARankingManager::ReverseScorePlayers()
{
	TArray<int> NewScorePlayers;

	int init = (ScorePlayers.Num() - 1) > 9 ? 9 : (ScorePlayers.Num() - 1);

	for (int i = ScorePlayers.Num() - 1; i >= 0; i--)
	{
		NewScorePlayers.Add(ScorePlayers[i]);
	}

	ScorePlayers = NewScorePlayers;
}

void ARankingManager::SaveCurrentRanking()
{
	CopyTopTen(NamePlayers, ScorePlayers, SavesGameInstance->NamePlayer, SavesGameInstance->ScorePlayer);
	UGameplayStatics::SaveGameToSlot(SavesGameInstance, SavesGameInstance->SaveSlotName, SavesGameInstance->UserIndex);
}

void ARankingManager::OnDestroyGameController(AActor* SelfActor)
{
	// Cleaned all dynamic memory.
	if (SavesGameInstance != NULL)
	{
		SavesGameInstance = NULL;
	}
}
