// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

//前方宣言
class UBehaviorTreeComponent;
class UBlackboardComponent;
class UBehaviorTree;

/**
 * 
 */
namespace BBKeys 
{
	const FName PlayerActor = TEXT("PlayerActor");
	const FName DistanceToPlayer = TEXT("DistanceToPlayer");
	const FName AttackRange = TEXT("AttackRange");
	const FName MonitoringRange = TEXT("MonitoringRange");
	const FName WaitTime = TEXT("WaitTime");
	const FName IsPlayerInSight = TEXT("IsPlayerInSight");
	const FName IsPlayerCanAttack = TEXT("IsPlayerCanAttack");
}
UCLASS()
class JHON2_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyAIController();

	virtual void OnPossess(APawn* InPawn) override;

private:
	//コンポーネントの保持
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = "AI|Param",meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = "AI|Param",meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBlackboardComponent> BlackboardComponent;

	//エディタ側でアタッチするBehaviorTreeアセット
	UPROPERTY(EditDefaultsOnly,Category = "AI|Param")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;
};
