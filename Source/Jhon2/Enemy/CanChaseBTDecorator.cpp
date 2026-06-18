// Fill out your copyright notice in the Description page of Project Settings.


#include "CanChaseBTDecorator.h"
#include "BehaviorTree/BlackboardComponent.h"

UCanChaseBTDecorator::UCanChaseBTDecorator()
{
	NodeName = TEXT("C++ Is Player In Sight Decorator");

	bAllowAbortNone = true;
	bAllowAbortLowerPri = true;
}

bool UCanChaseBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return false;

	//PlayerActorキーからオブジェクトを取得
	UObject* TargetObject = BlackboardComp->GetValueAsObject(PlayerActor.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(TargetObject);

	return (TargetActor != nullptr);
}
