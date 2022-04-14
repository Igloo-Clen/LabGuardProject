// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameModeBase.h"
#include "MyGameInstance.h"

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RestartButton->OnClicked.AddDynamic(this, &UGameOverWidget::Restart);
	MainMenuButton->OnClicked.AddDynamic(this, &UGameOverWidget::MainMenu);
}

void UGameOverWidget::Restart()
{
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	AMyGameModeBase* GameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	int32 Difficult = GameInstance->GetLevelDifficult();

	RemoveFromViewport();

	GameInstance->Reset();
	GameMode->ClearTimer();

	if (Difficult == 1)
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("NormalLevel"));
	}
	else if (Difficult == 2)
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("HardLevel"));
	}
	else
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("EasyLevel"));
	}
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameOnly());
}

void UGameOverWidget::MainMenu()
{
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	AMyGameModeBase* GameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	RemoveFromViewport();
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenu"));
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameOnly());

	GameInstance->Reset();
	GameMode->ClearTimer();
}
