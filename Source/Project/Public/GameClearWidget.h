// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameClearWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UGameClearWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
		void MainMenu();

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* MainMenuButton;
};
