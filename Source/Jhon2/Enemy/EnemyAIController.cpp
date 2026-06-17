// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AEnemyAIController::AEnemyAIController() 
{
	// コンポーネントの作成
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Behavior Tree アセットが設定されているかチェック
	if (BehaviorTreeAsset)
	{
		// Blackboardの初期化（BTに紐付いているBlackboardデータを使用）
		if (BehaviorTreeAsset->GetBlackboardAsset())
		{
			BlackboardComponent->InitializeBlackboard(*BehaviorTreeAsset->GetBlackboardAsset());
		}

		// Behavior Tree の実行開始
		BehaviorTreeComponent->StartTree(*BehaviorTreeAsset);

		// 補足：C++側からBlackboardに値をセットする例（キー名が "HomePosition" の場合）
		// BlackboardComponent->SetValueAsVector(TEXT("HomePosition"), InPawn->GetActorLocation());
	}
}
