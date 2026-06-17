// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyChase.h"
#include "EnemyManager.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"

// Sets default values for this component's properties
UEnemyChase::UEnemyChase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyChase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEnemyChase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEnemyChase::Chase(AEnemyManager* EnemyCharacter,ACharacter* PlayerCharacter) {
	//攻撃範囲外にいたら追跡
	if (EnemyCharacter->DistanceToPlayer > EnemyCharacter->AttackStateRange)
	{
		AAIController* AIController = Cast<AAIController>(EnemyCharacter->GetController());
		if (AIController)
		{
			//移動中（PathFollowingがアクティブ）ではない時だけ、移動命令を出す
			if (AIController->GetMoveStatus() != EPathFollowingStatus::Moving)
			{
				UAIBlueprintHelperLibrary::SimpleMoveToActor(AIController, PlayerCharacter);
			}
		}
	}
	//攻撃範囲内なら移動をやめて攻撃状態に移行
	else
	{
		EnemyCharacter->GetController()->StopMovement();
		EnemyCharacter->ChangeState(EEnemyState::Attack);
	}

}