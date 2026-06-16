// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "WorldTimeSetting.generated.h"

/**
 * 
 */
UCLASS()
class JHON2_API AWorldTimeSetting : public AWorldSettings
{
	GENERATED_BODY()

	void HitStopReset();

	//時間経過をはかるタイマー
	FTimerHandle HitStopTimer;

public:
	void HitStop(float StopTime);

};
