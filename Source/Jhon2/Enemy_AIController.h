// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "GameFramework/Character.h"
#include "EnemyAnimaion.h"
#include "EnemyCharacter.h"

#include "Enemy_AIController.generated.h"

/**
 * 
 */
UCLASS()
class JHON2_API AEnemy_AIController : public AAIController
{
	GENERATED_BODY()


public:
	AEnemy_AIController();

	//Perception更新時に呼ばれるイベント
	UFUNCTION()
	void OnTargetPerceived(AActor* Actor, FAIStimulus Stimulus);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AI")
	class UAIPerceptionComponent* AIPerceptionConfig;

	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gimmick", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MoveSpeed;

	UEnemyAnimaion* UAnimEnemy;
	
	class AEnemyCharacter* EnemyChara;

public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "AI")
	bool IsFindPlayer;
	
};
