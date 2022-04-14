// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AMainMenuGameMode();

public:
	UPROPERTY()
		TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY()
		UUserWidget* CurrentWidget;

	UPROPERTY()
		TSubclassOf<UUserWidget> LevelWidgetClass;

	UPROPERTY()
		UUserWidget* LevelWidget;

	UPROPERTY()
		TSubclassOf<UUserWidget> LoadingWidgetClass;

	UPROPERTY()
		UUserWidget* LoadingWidget;
};
