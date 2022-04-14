// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWidget.h"
#include "EnemyStatComponent.h"
#include "Components/ProgressBar.h"

void UEnemyWidget::BindHealth(class UEnemyStatComponent* StatComp)
{
	CurrentStatComp = StatComp;
	StatComp->OnHealthChanged.AddUObject(this, &UEnemyWidget::UpdateHealth);
}

void UEnemyWidget::UpdateHealth()
{
	if (CurrentStatComp.IsValid())
	{
		PB_HpBar->SetPercent(CurrentStatComp->GetHealthRatio());
	}
}