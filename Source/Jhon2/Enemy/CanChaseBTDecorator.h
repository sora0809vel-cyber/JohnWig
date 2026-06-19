// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CanChaseBTDecorator.generated.h"

/**
 * 
 */
UCLASS()
class JHON2_API UCanChaseBTDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UCanChaseBTDecorator();

protected:
	//条件のTrue,Falseを判定する関数
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	//BlackboardのPlayerActorキーを割り当てるための変数
	UPROPERTY(EditAnywhere,Category = "AI|Blackboard")
	FBlackboardKeySelector PlayerActor;
};
