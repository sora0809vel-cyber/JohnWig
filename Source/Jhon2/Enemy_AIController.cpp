// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AEnemy_AIController::AEnemy_AIController() :
	MoveSpeed(0.f),
	IsFindPlayer(false)
{
	//Perceptionコンポーネントの作成
	AIPerceptionConfig = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	//視覚の設定
	SightConfig->SightRadius = 2000.0f;					 // 見える距離
	SightConfig->LoseSightRadius = 2500.0f;				 // 見失う距離
	SightConfig->PeripheralVisionAngleDegrees = 75.0f;   // 視野角
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	//コンポーネントに設定を登録
	AIPerceptionConfig->ConfigureSense(*SightConfig);
	AIPerceptionConfig->SetDominantSense(SightConfig->GetSenseImplementation());
}

void AEnemy_AIController::BeginPlay()
{
	Super::BeginPlay();
	// cppファイル内
	GetAIPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemy_AIController::OnTargetPerceived);

}

//プレイヤーを感知した時の関数
void AEnemy_AIController::OnTargetPerceived(AActor* Actor, FAIStimulus Stimulus)
{
	//認識した場合
	if (Stimulus.WasSuccessfullySensed()) {
		UE_LOG(LogTemp, Warning, TEXT("Chase"), *Actor->GetName());
		MoveToActor(Actor, MoveSpeed);	//追跡開始
		//プレイヤーを見つけた時のフラグ
		IsFindPlayer = true;
		EnemyChara->SetFindPlayerFlg(IsFindPlayer);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Target Lost"));
		StopMovement();		//見つかったら停止

		IsFindPlayer = false;
		EnemyChara->SetFindPlayerFlg(IsFindPlayer);
	}
}

