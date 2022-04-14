// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SpawnObject.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMyGameModeBase();

protected:
	virtual void BeginPlay() override;


public:
	void WaveWait();
	void SpawnStart();
	void ClearCheck();
	void WaveClear();

	void ClearTimer();

public:
	UPROPERTY()
		TSubclassOf<UUserWidget> HUD_Class;

	UPROPERTY()
		UUserWidget* CurrentWidget;

	UPROPERTY()
		TSubclassOf<UUserWidget> GameOverWidget_Class;

	UPROPERTY()
		UUserWidget* GameOverWidget;

	UPROPERTY()
		TSubclassOf<UUserWidget> StatusWidget_Class;

	UPROPERTY()
		UUserWidget* StatusWidget;

	UPROPERTY()
		TSubclassOf<UUserWidget> GameClearWidget_Class;

	UPROPERTY()
		UUserWidget* GameClearWidget;


public:
	TArray<AActor*> Spawner;

	FTimerHandle WaitHandle;
	FTimerHandle TimerHandle;
	FTimerHandle ClearCheckTimerHandle;

private:
	UPROPERTY()
		class UMyGameInstance* GameInstance;
};
