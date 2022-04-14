// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameModeBase.h"
#include "MyHUD.h"
#include "MySaveGame.h"

UMyGameInstance::UMyGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_LevelEasy(TEXT("DataTable'/Game/Data/DT_EasyLevel.DT_EasyLevel'"));
	if (DT_LevelEasy.Succeeded())
	{
		EasyWaveData = DT_LevelEasy.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_LevelNormal(TEXT("DataTable'/Game/Data/DT_NormalLevel.DT_NormalLevel'"));
	if (DT_LevelNormal.Succeeded())
	{
		NormalWaveData = DT_LevelNormal.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_LevelHard(TEXT("DataTable'/Game/Data/DT_HardLevel.DT_HardLevel'"));
	if (DT_LevelHard.Succeeded())
	{
		HardWaveData = DT_LevelHard.Object;
	}

	// Default
	Wave = 1;
	EnemyCount = 0;
	Score = 0;
}

void UMyGameInstance::SaveGame()
{
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));

	SaveGameInstance->MasterVolume = MasterVolume;
	SaveGameInstance->MusicVolume = MusicVolume;
	SaveGameInstance->EffectsVolume = EffectsVolume;
	SaveGameInstance->FullScreenType = FullScreenType;

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);
}

void UMyGameInstance::LoadGame()
{
	UMySaveGame* LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));

	if (LoadGameInstance)
	{
		MasterVolume = LoadGameInstance->MasterVolume;
		MusicVolume = LoadGameInstance->MusicVolume;
		EffectsVolume = LoadGameInstance->EffectsVolume;
		FullScreenType = LoadGameInstance->FullScreenType;
	}
	
}

void UMyGameInstance::SetMasterVolumeValue(float Value)
{
	MasterVolume = Value;
}

void UMyGameInstance::SetMusicVolumeValue(float Value)
{
	MusicVolume = Value;
}

void UMyGameInstance::SetEffectsVolumeValue(float Value)
{
	EffectsVolume = Value;
}

void UMyGameInstance::SetFullScreenType(int32 Type)
{
	FullScreenType = Type;
}

void UMyGameInstance::RefreshUI()
{
	AMyGameModeBase* GameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		UMyHUD* MyHUD = Cast<UMyHUD>(GameMode->CurrentWidget);
		if (MyHUD)
		{
			const FString WaveStr = FString::Printf(TEXT("Wave %d"), GetWave());
			MyHUD->GetWaveText()->SetText(FText::FromString(WaveStr));

			const FString EnemyStr = FString::Printf(TEXT("Enemy : %d"), GetEnemyCount());
			MyHUD->GetEnemyText()->SetText(FText::FromString(EnemyStr));

			const FString ScoreStr = FString::Printf(TEXT("Score : %d"), GetScore());
			MyHUD->GetScoreText()->SetText(FText::FromString(ScoreStr));
		}
	}
}

void UMyGameInstance::SetLevelDifficult(int32 Num)
{
	LevelDifficult = Num;
}

void UMyGameInstance::SetLevelSelect()
{
	if (LevelDifficult == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Easy"));
		WaveData = EasyWaveData;
	}
	else if (LevelDifficult == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Normal"));
		WaveData = NormalWaveData;
	}
	else if (LevelDifficult == 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hard"));
		WaveData = HardWaveData;
	}

	SetMaxEnemyCount(Wave);
	SetMaxWave(Wave);
}

void UMyGameInstance::NextWave()
{
	Wave++;
}

void UMyGameInstance::Reset()
{
	Wave = 1;
	EnemyCount = 0;
	SpawnedCount = 0;
	Score = 0;
	SetMaxEnemyCount(Wave);
}

void UMyGameInstance::SetMaxWave(int32 WaveCount)
{
	auto Data = GetWaveData(WaveCount);
	if (Data)
	{
		MaxWave = Data->MaxWave;
	}
	
}

void UMyGameInstance::SetEnemyCount(int32 Count)
{
	if (Count == 0)
		EnemyCount = 0;
	else
		EnemyCount += Count;
}

void UMyGameInstance::SetMaxEnemyCount(int32 WaveCount)
{
	auto Data = GetWaveData(WaveCount);
	if (Data)
	{
		MaxEnemyCount = Data->MaxEnemyCount;
	}
}

void UMyGameInstance::SetSpawnedCount(int32 Count)
{
	if (Count == 0)
		SpawnedCount = 0;
	else
		SpawnedCount += Count;
}

void UMyGameInstance::SetScore(int32 PlusScore)
{
	Score += PlusScore;
}

FWaveData* UMyGameInstance::GetWaveData(int32 WaveCount)
{
	return WaveData->FindRow<FWaveData>(*FString::FromInt(WaveCount), TEXT(""));
}
