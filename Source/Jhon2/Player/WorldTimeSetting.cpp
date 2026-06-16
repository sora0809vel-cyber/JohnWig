// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldTimeSetting.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

void AWorldTimeSetting::HitStop(float StopTime)
{
	//受け取る値が0より小さいなら辞める
	if (StopTime <= 0.f)
	{
		return;
	}

	//世界の時間の流れを変化させることができるのだ
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(),0.001f);

	//世界の時の流れに惑わされないタイマー
	GetWorldTimerManager().SetTimer(
		HitStopTimer, this, &AWorldTimeSetting::HitStopReset,
		StopTime, false, -1.f
	);

}

void AWorldTimeSetting::HitStopReset()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);

}


