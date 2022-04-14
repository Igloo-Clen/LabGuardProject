// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;


public:
	UFUNCTION()
		void Start();

	UFUNCTION()
		void Option();

	UFUNCTION()
		void Quit();

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* OptionButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* QuitButton;

	UPROPERTY()
		class AMainMenuGameMode* GameMode;
};
