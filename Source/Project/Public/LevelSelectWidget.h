// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelSelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API ULevelSelectWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
		void EasyLevelStart();

	UFUNCTION()
		void NormalLevelStart();

	UFUNCTION()
		void HardLevelStart();

	UFUNCTION()
		void Cancel();

	void Loading();

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* EasyLevelButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* NormalLevelButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* HardLevelButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* CancelButton;

	UPROPERTY()
		class AMainMenuGameMode* GameMode;

	UPROPERTY()
		class UMyGameInstance* GameInstance;

	FTimerHandle TimerHandle;
};
