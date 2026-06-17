// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCooldown.h"
#include "EnemyManager.h"
#include "Kismet/GameplayStatics.h" //プレイヤーを検索するために必要

// Sets default values for this component's properties
UEnemyCooldown::UEnemyCooldown()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyCooldown::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEnemyCooldown::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//後隙状態に呼ばれる関数
void UEnemyCooldown::Cooldown(AEnemyManager* EnemyCharacter)
{
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(EnemyCharacter);
	EnemyCharacter->CooldownTimer -= DeltaTime;
	if (EnemyCharacter->CooldownTimer <= 0.f)
	{
		//プレイヤーとの距離が監視範囲より遠かったら待機
		if (EnemyCharacter->DistanceToPlayer < EnemyCharacter->MonitoringRange)
		{
			//状態を待機に変更
			EnemyCharacter->ChangeState(EEnemyState::Idle);
		}
		//監視範囲内かつ攻撃範囲外にいたら追跡
		else if (EnemyCharacter->DistanceToPlayer < EnemyCharacter->AttackStateRange)
		{
			//状態を追跡に変更
			EnemyCharacter->ChangeState(EEnemyState::Chase);
		}

	}

	EnemyCharacter->IsAttack = false;

}

