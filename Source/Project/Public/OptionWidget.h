// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UOptionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	UFUNCTION()
		void MasterChange(float Value);

	UFUNCTION()
		void MusicChange(float Value);

	UFUNCTION()
		void EffectsChange(float Value);

	UFUNCTION()
		void FullScreen();

	UFUNCTION()
		void WindowedFullScreen();

	UFUNCTION()
		void Windowed();

	UFUNCTION()
		void Save();
private:
	UPROPERTY(meta = (BindWidget))
		class USlider* MasterSlider;

	UPROPERTY(meta = (BindWidget))
		class USlider* MusicSlider;

	UPROPERTY(meta = (BindWidget))
		class USlider* EffectsSlider;

	UPROPERTY(meta = (BindWidget))
		class UButton* FullScreenButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* WindowedFullScreenButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* WindowedButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* ApplyButton;

	UPROPERTY()
		class UMyGameInstance* GameInstance;
};
