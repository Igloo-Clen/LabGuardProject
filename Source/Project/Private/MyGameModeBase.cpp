// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MyGameInstance.h"
#include "PlayerCharacter.h"
#include "PlayerStatComponent.h"
#include "MyHUD.h"
#include "GameOverWidget.h"
#include "StatusWidget.h"
#include "GameClearWidget.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnObject.h"
#include "EnemyCharacter.h"
#include "EnemyStatComponent.h"

AMyGameModeBase::AMyGameModeBase()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	static ConstructorHelpers::FClassFinder<UMyHUD> UI_HUD(TEXT("WidgetBlueprint'/Game/UI/WBP_HUD.WBP_HUD_C'"));
	if (UI_HUD.Succeeded())
	{
		HUD_Class = UI_HUD.Class;

		CurrentWidget = CreateWidget(GetWorld(), HUD_Class);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}

	static ConstructorHelpers::FClassFinder<UGameOverWidget> GOW(TEXT("WidgetBlueprint'/Game/UI/WBP_GameOver.WBP_GameOver_C'"));
	if (GOW.Succeeded())
	{
		GameOverWidget_Class = GOW.Class;
		GameOverWidget = CreateWidget(GetWorld(), GameOverWidget_Class);
	}

	static ConstructorHelpers::FClassFinder<UStatusWidget> SW(TEXT("WidgetBlueprint'/Game/UI/WBP_Status.WBP_Status_C'"));
	if (SW.Succeeded())
	{
		StatusWidget_Class = SW.Class;
		StatusWidget = CreateWidget(GetWorld(), StatusWidget_Class);
	}

	static ConstructorHelpers::FClassFinder<UGameClearWidget> GCW(TEXT("WidgetBlueprint'/Game/UI/WBP_GameClear.WBP_GameClear_C'"));
	if (GCW.Succeeded())
	{
		GameClearWidget_Class = GCW.Class;
		GameClearWidget = CreateWidget(GetWorld(), GameClearWidget_Class);
	}
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnObject::StaticClass(), Spawner);
}

void AMyGameModeBase::BeginPlay()
{
	WaveWait();
}

void AMyGameModeBase::WaveWait()
{
	GameInstance->RefreshUI();
	FWaveData* WaveData = GameInstance->GetWaveData(GameInstance->GetWave());

	// Actor Stat Update
	if (WaveData->HealthUp != 0 || WaveData->AttackUp != 0 || WaveData->SpeedUp != 0.f)
	{
		for (auto Actor : Spawner)
		{
			ASpawnObject* SpawnObject = Cast<ASpawnObject>(Actor);
			TArray<AEnemyCharacter*> Pool = SpawnObject->ObjectPooler->GetSwordEnemyPool();
			for (auto Enemy : Pool)
			{
				Enemy->Stat->SetMaxHealth(WaveData->HealthUp);
				Enemy->Stat->SetAttack(WaveData->AttackUp);
				Enemy->Stat->SetSpeed(WaveData->SpeedUp);
				Enemy->Stat->SetHealth(Enemy->Stat->GetMaxHealth());
			}
		}
	}
	
	// Spawn Loop
	GetWorldTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			GetWorldTimerManager().SetTimer(TimerHandle, this, &AMyGameModeBase::SpawnStart, 3.f, true);
		}), 5.f, false);
}

void AMyGameModeBase::SpawnStart()
{
	for (auto Actor : Spawner)
	{
		ASpawnObject* SpawnObject = Cast<ASpawnObject>(Actor);
		if (SpawnObject)
		{
			if (GameInstance->GetSpawnedCount() >= GameInstance->GetMaxEnemyCount())
			{
				GetWorldTimerManager().ClearTimer(TimerHandle);
				GetWorldTimerManager().SetTimer(ClearCheckTimerHandle, this, &AMyGameModeBase::ClearCheck, 1.f, true);
				return;
			}
			SpawnObject->Spawn();
		}
	}
}

void AMyGameModeBase::ClearCheck()
{
	if (GameInstance->GetEnemyCount() <= 0 && GameInstance->GetSpawnedCount() == GameInstance->GetMaxEnemyCount())
	{
		WaveClear();
		GetWorldTimerManager().ClearTimer(ClearCheckTimerHandle);
	}
}

void AMyGameModeBase::WaveClear()
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	

	GetWorldTimerManager().ClearTimer(WaitHandle);
	GameInstance->SetSpawnedCount(0);
	GameInstance->NextWave();

	if (GameInstance->GetMaxWave() < GameInstance->GetWave())
	{
		Cast<UGameClearWidget>(this->GameClearWidget)->AddToViewport();
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(FInputModeGameAndUI());
		return;
	}


	GameInstance->SetMaxEnemyCount(GameInstance->GetWave());

	// PlayerStat AbillityPoint Up
	if (GameInstance->GetLevelDifficult() < 1)
	{
		Player->Stat->SetAbilityPoint(1);
	}
	Player->Stat->SetAbilityPoint(1);

	WaveWait();
}

void AMyGameModeBase::ClearTimer()
{
	GetWorldTimerManager().ClearTimer(WaitHandle);
	GetWorldTimerManager().ClearTimer(TimerHandle);
	GetWorldTimerManager().ClearTimer(ClearCheckTimerHandle);
}
