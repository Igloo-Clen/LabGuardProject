// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MyGameInstance.h"
#include "kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "PlayerStatComponent.h"
#include "MyGameModeBase.h"

void UStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton->OnClicked.AddUniqueDynamic(this, &UStatusWidget::Resume);
	RestartButton->OnClicked.AddUniqueDynamic(this, &UStatusWidget::Restart);
	QuitButton->OnClicked.AddUniqueDynamic(this, &UStatusWidget::Quit);

	HpUpButton->OnClicked.AddUniqueDynamic(this, &UStatusWidget::HpUp);
	AttackUpButton->OnClicked.AddUniqueDynamic(this, &UStatusWidget::AttackUp);
	SpeedUpButton->OnClicked.AddUniqueDynamic(this, &UStatusWidget::SpeedUp);
	FireCooltimeDownButton->OnClicked.AddUniqueDynamic(this, &UStatusWidget::FireCooltimeDown);

	GameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	Stat = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))->Stat;
}

void UStatusWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	RefreshUI();
}

void UStatusWidget::RefreshUI()
{
	UStatusWidget* Status = Cast<UStatusWidget>(GameMode->StatusWidget);
	if (Status)
	{
		const FString HpStr = FString::Printf(TEXT("MaxHp %d"), Stat->GetMaxHealth());
		Status->GetMaxHpText()->SetText(FText::FromString(HpStr));

		const FString AttackStr = FString::Printf(TEXT("Damage %d"), Stat->GetAttack());
		Status->GetAttackText()->SetText(FText::FromString(AttackStr));

		const FString SpeedStr = FString::Printf(TEXT("Speed %.0f"), Stat->GetWalkSpeed());
		Status->GetSpeedText()->SetText(FText::FromString(SpeedStr));

		const FString FireCooltimeStr = FString::Printf(TEXT("Fire Cooltime %.1f"), Stat->GetFireCooltime());
		Status->GetFireCooltimeText()->SetText(FText::FromString(FireCooltimeStr));

		const FString AbillityPointStr = FString::Printf(TEXT("Abillity Point %d"), Stat->GetAbilityPoint());
		Status->GetAbillityPointText()->SetText(FText::FromString(AbillityPointStr));
	}
}

void UStatusWidget::Resume()
{
	RemoveFromViewport();
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameOnly());
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;
}

void UStatusWidget::Restart()
{
	int32 Difficult = GameInstance->GetLevelDifficult();

	RemoveFromViewport();

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

	GameInstance->Reset();
	GameMode->ClearTimer();
}

void UStatusWidget::Quit()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenu"));
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameOnly());

	GameInstance->Reset();
	GameMode->ClearTimer();
}

void UStatusWidget::HpUp()
{
	if (Stat->GetAbilityPoint() <= 0)
	{
		return;
	}
	Stat->SetAbilityPoint(-1);
	Stat->SetMaxHealth(50);
	Stat->SetHealth(50);
}

void UStatusWidget::AttackUp()
{
	if (Stat->GetAbilityPoint() <= 0 || Stat->GetAttack() >= 200)
	{
		return;
	}
	Stat->SetAbilityPoint(-1);
	Stat->SetAttack(10);
}

void UStatusWidget::SpeedUp()
{
	if (Stat->GetAbilityPoint() <= 0 || Stat->GetWalkSpeed() >= 700.f)
	{
		return;
	}
	Stat->SetAbilityPoint(-1);
	Stat->SetWalkSpeed(10.f);
}

void UStatusWidget::FireCooltimeDown()
{
	if (Stat->GetAbilityPoint() <= 0 || Stat->GetFireCooltime() <= 0.4f)
	{
		return;
	}
	Stat->SetAbilityPoint(-1);
	Stat->SetFireCooltime(0.2f);
}
