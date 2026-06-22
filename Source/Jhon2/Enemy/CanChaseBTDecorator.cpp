// Fill out your copyright notice in the Description page of Project Settings.


#include "CanChaseBTDecorator.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

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
	if (!TargetActor) return false;

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return false;

	APawn* EnemyPawn = AIController->GetPawn();
	if (!EnemyPawn) return false;

	float Distance = FVector::Dist(EnemyPawn->GetActorLocation(), TargetActor->GetActorLocation());

	return (Distance <= ChaseRange);
}
