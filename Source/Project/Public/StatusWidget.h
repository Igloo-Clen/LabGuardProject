// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TextBlock.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UStatusWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	void RefreshUI();

	UFUNCTION()
		void Resume();

	UFUNCTION()
		void Restart();

	UFUNCTION()
		void Quit();

	UFUNCTION()
		void HpUp();

	UFUNCTION()
		void AttackUp();

	UFUNCTION()
		void SpeedUp();

	UFUNCTION()
		void FireCooltimeDown();

public:
	UTextBlock* GetMaxHpText() { return MaxHpText; }
	UTextBlock* GetAttackText() { return AttackText; }
	UTextBlock* GetSpeedText() { return SpeedText; }
	UTextBlock* GetFireCooltimeText() { return FireCooltimeText; }
	UTextBlock* GetAbillityPointText() { return AbilityPointText; }


private:
	UPROPERTY(meta = (BindWidget))
		class UButton* ResumeButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* RestartButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MaxHpText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AttackText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* SpeedText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* FireCooltimeText;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AbilityPointText;

	UPROPERTY(meta = (BindWidget))
		class UButton* HpUpButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* AttackUpButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* SpeedUpButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* FireCooltimeDownButton;

	UPROPERTY()
		class AMyGameModeBase* GameMode;

	UPROPERTY()
		class UMyGameInstance* GameInstance;

	UPROPERTY()
		class UPlayerStatComponent* Stat;



};
