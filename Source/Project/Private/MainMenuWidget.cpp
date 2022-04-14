// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameModeBase.h"
#include "MyHUD.h"
#include "MainMenuGameMode.h"
#include "Blueprint/UserWidget.h"



void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StartButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::Start);
	OptionButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::Option);
	QuitButton->OnClicked.AddUniqueDynamic(this, &UMainMenuWidget::Quit);

	GameMode = Cast<AMainMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void UMainMenuWidget::Start()
{
	GameMode->LevelWidget->AddToViewport();
}

void UMainMenuWidget::Option()
{
	GameMode->CurrentWidget->AddToViewport();
}

void UMainMenuWidget::Quit()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}
