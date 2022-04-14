// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSelectWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "MainMenuGameMode.h"
#include "PlayerCharacter.h"
#include "PlayerStatComponent.h"
#include "GameFramework/Actor.h"

void ULevelSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	EasyLevelButton->OnClicked.AddDynamic(this, &ULevelSelectWidget::EasyLevelStart);
	NormalLevelButton->OnClicked.AddDynamic(this, &ULevelSelectWidget::NormalLevelStart);
	HardLevelButton->OnClicked.AddDynamic(this, &ULevelSelectWidget::HardLevelStart);
	CancelButton->OnClicked.AddDynamic(this, &ULevelSelectWidget::Cancel);

	GameMode = Cast<AMainMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

}

void ULevelSelectWidget::EasyLevelStart()
{
	GameInstance->SetLevelDifficult(0);

	Loading();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("EasyLevel"));
		}), 5.f, false);
}

void ULevelSelectWidget::NormalLevelStart()
{
	GameInstance->SetLevelDifficult(1);
	
	Loading();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("NormalLevel"));
		}), 5.f, false);
	
}

void ULevelSelectWidget::HardLevelStart()
{
	GameInstance->SetLevelDifficult(2);

	Loading();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("HardLevel"));
		}), 5.f, false);
}

void ULevelSelectWidget::Cancel()
{
	RemoveFromViewport();
}

void ULevelSelectWidget::Loading()
{
	GameMode->LoadingWidget->AddToViewport();
	GameInstance->SetLevelSelect();
}
