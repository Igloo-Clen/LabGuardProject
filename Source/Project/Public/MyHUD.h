// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UMyHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UTextBlock* GetWaveText() { return WaveText; }
	UTextBlock* GetEnemyText() { return EnemyText; }
	UTextBlock* GetScoreText() { return ScoreText; }
	UTextBlock* GetHpText() { return HpText; }
	UTextBlock* GetAmmoText() { return AmmoText; }
	UImage* GetFireCooltimeBar() { return FirecooltimeBar; }

private:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* WaveText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* EnemyText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ScoreText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* HpText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AmmoText;

	UPROPERTY(meta = (BindWidget))
		class UImage* FirecooltimeBar;
};
