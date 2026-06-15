// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyIdle.h"
#include "EnemyManager.h"

// Sets default values for this component's properties
UEnemyIdle::UEnemyIdle()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyIdle::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEnemyIdle::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//待機状態で呼ばれる関数
void UEnemyIdle::Idle(AEnemyManager* EnemyCharacter)
{
	//プレイヤーとの距離が監視範囲より遠かったら
	if (EnemyCharacter->DistanceToPlayer > EnemyCharacter->MonitoringRange)
	{
		EnemyCharacter->GetController()->StopMovement();

		FRotator IdleRotation = { 0,90,0 };
		//敵の回転を代入
		EnemyCharacter->SetActorRotation(IdleRotation);
	}
	//監視範囲内に入ったら
	else
	{
		//現在のステートをChaseに変更
		EnemyCharacter->ChangeState(State::Chase);
	}

}

