// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/Character.h"
#include "Jhon2/Player/Player_2.h"
#include "EnemyCharacter.h"

AEnemyAIController::AEnemyAIController()
{
	// AIPerceptionコンポーネントを作成
	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));
	
	// 視覚設定の作成とパラメータ設定
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	if (SightConfig)
	{
		SightConfig->SightRadius = 1500.0f;								//視界の最大距離
		SightConfig->LoseSightRadius = 1800.0f;							//見失う距離
		SightConfig->PeripheralVisionAngleDegrees = 180.0f;				//視野角の半分
		SightConfig->SetMaxAge(5.0f);									//記憶の生存時間

		// 敵・味方・中立のどれを検知するか
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		// コンポーネントに視覚設定を登録
		AIPerceptionComp->ConfigureSense(*SightConfig);
		AIPerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
	}
}

//BiginPlayみたいなもの
void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// デリゲートのバインド
	if (AIPerceptionComp)
	{
		AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
	}

	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 誰かを視認、または見失った瞬間にだけ呼び出される
void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{

	// 検知したアクターがプレイヤーキャラクターか判定
	ACharacter* PlayerCharacter = Cast<ACharacter>(Actor);
	if (!PlayerCharacter || !PlayerCharacter->IsPlayerControlled()) return;

	//親クラスからBlackboardを取得
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (!BlackboardComponent) return;

	//今操作している敵の本体を取得
	AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetPawn());


	//視界に入ったかどうか
	if (Stimulus.WasSuccessfullySensed())
	{
		//プレイヤーキャラクターをメンバ変数に保存
		TargetActor = PlayerCharacter;

		//プレイヤーを発見したら、そのアクター注視する
		SetFocus(TargetActor);

		//Blackboardにプレイヤーを登録
		BlackboardComponent->SetValueAsObject(BBKeys::PlayerActor, TargetActor);

	}
	//視界外に出たら
	else
	{
		//注視を解除
		ClearFocus(EAIFocusPriority::Gameplay);

		BlackboardComponent->ClearValue(BBKeys::PlayerActor);
		BlackboardComponent->SetValueAsBool(BBKeys::CanAttack, false);

		TargetActor = nullptr;
	}
}