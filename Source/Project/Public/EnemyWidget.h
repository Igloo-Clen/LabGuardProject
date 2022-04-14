// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UEnemyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindHealth(class UEnemyStatComponent* StatComp);

	void UpdateHealth();

private:
	// 약한 포인터
	TWeakObjectPtr<class UEnemyStatComponent> CurrentStatComp;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* PB_HpBar;
};
