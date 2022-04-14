// Fill out your copyright notice in the Description page of Project Settings.


#include "GameClearWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "MyGameModeBase.h"

void UGameClearWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MainMenuButton->OnClicked.AddUniqueDynamic(this, &UGameClearWidget::MainMenu);
}

void UGameClearWidget::MainMenu()
{
	AMyGameModeBase* GameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	GameInstance->Reset();
	GameMode->ClearTimer();

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MeinMenu"));
}
