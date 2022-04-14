// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
		void Restart();

	UFUNCTION()
		void MainMenu();

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* RestartButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* MainMenuButton;

};
