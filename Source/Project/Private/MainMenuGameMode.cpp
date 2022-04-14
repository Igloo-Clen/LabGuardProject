// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "Blueprint/UserWidget.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> UI(TEXT("WidgetBlueprint'/Game/UI/WBP_Option.WBP_Option_C'"));
	if (UI.Succeeded())
	{
		WidgetClass = UI.Class;

		CurrentWidget = CreateWidget(GetWorld(), WidgetClass);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> LevelUI(TEXT("WidgetBlueprint'/Game/UI/WBP_LevelSelect.WBP_LevelSelect_C'"));
	if (LevelUI.Succeeded())
	{
		LevelWidgetClass = LevelUI.Class;

		LevelWidget = CreateWidget(GetWorld(), LevelWidgetClass);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> LoadingUI(TEXT("WidgetBlueprint'/Game/UI/WBP_Loading.WBP_Loading_C'"));
	if (LoadingUI.Succeeded())
	{
		LoadingWidgetClass = LoadingUI.Class;

		LoadingWidget = CreateWidget(GetWorld(), LoadingWidgetClass);
	}
}