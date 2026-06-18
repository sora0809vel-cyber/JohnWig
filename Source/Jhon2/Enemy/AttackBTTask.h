// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AttackBTTask.generated.h"

//前方宣言
class AEnemyCharacter;
/**
 * 
 */
UCLASS()
class JHON2_API UAttackBTTask : public UBTTaskNode
{
	GENERATED_BODY()
	
	UAttackBTTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	//アニメーション終了にタイマーから呼び出される関数
	void OnAttackAnimationFinished(TWeakObjectPtr<UBehaviorTreeComponent> OwnerCompPtr);

};
