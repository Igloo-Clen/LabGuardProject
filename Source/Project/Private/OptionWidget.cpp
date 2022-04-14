// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionWidget.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"
#include "MyGameInstance.h"

void UOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	MasterSlider->SetValue(GameInstance->GetMasterVolume());
	MusicSlider->SetValue(GameInstance->GetMusicVolume());
	EffectsSlider->SetValue(GameInstance->GetEffectsVolume());

	MasterSlider->OnValueChanged.AddDynamic(this, &UOptionWidget::MasterChange);
	MusicSlider->OnValueChanged.AddDynamic(this, &UOptionWidget::MusicChange);
	EffectsSlider->OnValueChanged.AddDynamic(this, &UOptionWidget::EffectsChange);

	FullScreenButton->OnClicked.AddUniqueDynamic(this, &UOptionWidget::FullScreen);
	WindowedFullScreenButton->OnClicked.AddUniqueDynamic(this, &UOptionWidget::WindowedFullScreen);
	WindowedButton->OnClicked.AddUniqueDynamic(this, &UOptionWidget::Windowed);

	ApplyButton->OnClicked.AddUniqueDynamic(this, &UOptionWidget::Save);
}

void UOptionWidget::MasterChange(float Value)
{
	GameInstance->SetMasterVolume(Value);
}

void UOptionWidget::MusicChange(float Value)
{
	GameInstance->SetMusicVolume(Value);
}

void UOptionWidget::EffectsChange(float Value)
{
	GameInstance->SetEffectsVolume(Value);
}

void UOptionWidget::FullScreen()
{
	UGameUserSettings::GetGameUserSettings()->SetFullscreenMode(EWindowMode::Fullscreen);
	UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
	GameInstance->SetFullScreenType(0);
}

void UOptionWidget::WindowedFullScreen()
{
	UGameUserSettings::GetGameUserSettings()->SetFullscreenMode(EWindowMode::WindowedFullscreen);
	UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
	GameInstance->SetFullScreenType(1);
}

void UOptionWidget::Windowed()
{
	UGameUserSettings::GetGameUserSettings()->SetFullscreenMode(EWindowMode::Windowed);
	UGameUserSettings::GetGameUserSettings()->SetScreenResolution(FIntPoint(1600.f, 900.f));
	UGameUserSettings::GetGameUserSettings()->ApplySettings(false);
	GameInstance->SetFullScreenType(2);
}

void UOptionWidget::Save()
{
	GameInstance->SaveGame();
	RemoveFromViewport();
}
