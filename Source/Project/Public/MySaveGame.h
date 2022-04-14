// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UMySaveGame();
	
	UPROPERTY(VisibleAnywhere, Category = "Save")
		FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = "Save")
		int32 UserIndex;

public:
	UPROPERTY(BlueprintReadWrite, Category = "Sound Volume")
		float MasterVolume;

	UPROPERTY(BlueprintReadWrite, Category = "Sound Volume")
		float MusicVolume;

	UPROPERTY(BlueprintReadWrite, Category = "Sound Volume")
		float EffectsVolume;

	UPROPERTY(BlueprintReadWrite, Category = "FullScreen")
		int32 FullScreenType;
};
