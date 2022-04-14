// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "MyGameInstance.generated.h"

USTRUCT()
struct FWaveData : public FTableRowBase
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Wave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MaxWave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MaxEnemyCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 HealthUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 AttackUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SpeedUp;
};
/**
 * 
 */
UCLASS()
class PROJECT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMyGameInstance();

	//UPROPERTY()
		//class UMySaveGame* SaveGame;

	UFUNCTION(BlueprintCallable, Category = "Save Game")
		void SaveGame();

	UFUNCTION(BlueprintCallable, Category = "Save Game")
		void LoadGame();
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Volume Setting")
		void SetMasterVolume(float Volume);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Volume Setting")
		void SetMusicVolume(float Volume);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Volume Setting")
		void SetEffectsVolume(float Volume);

	UFUNCTION(BlueprintCallable, Category = "Volume Setting")
		void SetMasterVolumeValue(float Value);

	UFUNCTION(BlueprintCallable, Category = "Volume Setting")
		void SetMusicVolumeValue(float Value);

	UFUNCTION(BlueprintCallable, Category = "Volume Setting")
		void SetEffectsVolumeValue(float Value);

	UFUNCTION(BlueprintCallable, Category = "FullScreen")
		void SetFullScreenType(int32 Type);




	UFUNCTION(BlueprintCallable, Category = "Volume Setting")
		float GetMasterVolume() { return MasterVolume; }

	UFUNCTION(BlueprintCallable, Category = "Volume Setting")
		float GetMusicVolume() { return MusicVolume; }

	UFUNCTION(BlueprintCallable, Category = "Volume Setting")
		float GetEffectsVolume() { return EffectsVolume; }

	UFUNCTION(BlueprintCallable, Category = "FullScreen")
		int32 GetFullScreenType() { return FullScreenType; }

public:
	void RefreshUI();

	void SetLevelDifficult(int32 Num);
	void SetLevelSelect();
	
	void NextWave();
	void Reset();
	void SetMaxWave(int32 WaveCount);
	void SetEnemyCount(int32 Count);
	void SetMaxEnemyCount(int32 WaveCount);
	void SetSpawnedCount(int32 Count);
	void SetScore(int32 PlusScore);

	FWaveData* GetWaveData(int32 Wave);

	int32 GetLevelDifficult() { return LevelDifficult; }
	int32 GetWave() { return Wave; }
	int32 GetMaxWave() { return MaxWave; }
	int32 GetEnemyCount() { return EnemyCount; }
	int32 GetMaxEnemyCount() { return MaxEnemyCount; }
	int32 GetSpawnedCount() { return SpawnedCount; }
	int32 GetScore() { return Score; }

private:
	UPROPERTY(EditAnywhere, Category = "Sound Volume")
		float MasterVolume = 1.f;

	UPROPERTY(EditAnywhere, Category = "Sound Volume")
		float MusicVolume = 1.f;

	UPROPERTY(EditAnywhere, Category = "Sound Volume")
		float EffectsVolume = 1.f;

	UPROPERTY(EditAnywhere, Category = "FullScreen")
		int32 FullScreenType = 2;

private:
	UPROPERTY()
		class UDataTable* WaveData;

	UPROPERTY()
		class UDataTable* EasyWaveData;

	UPROPERTY()
		class UDataTable* NormalWaveData;

	UPROPERTY()
		class UDataTable* HardWaveData;

	// 0 = Easy, 1 = Normal, 2 = Hard
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
		int32 LevelDifficult;
	
	UPROPERTY(EditAnywhere, Category = Wave, Meta = (AllowPrivateAccess = true))
		int32 Wave;

	UPROPERTY(EditAnywhere, Category = Wave, Meta = (AllowPrivateAccess = true))
		int32 MaxWave;

	UPROPERTY(EditAnywhere, Category = Wave, Meta = (AllowPrivateAccess = true))
		int32 EnemyCount;

	UPROPERTY(EditAnywhere, Category = Wave, Meta = (AllowPrivateAccess = true))
		int32 MaxEnemyCount;

	UPROPERTY(EditAnywhere, Category = Wave, Meta = (AllowPrivateAccess = true))
		int32 SpawnedCount;

	UPROPERTY(EditAnywhere, Category = Score, Meta = (AllowPrivateAccess = true))
		int32 Score;
};
