// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimaion.h"

//初期化
void UEnemyAnimaion::NativeInitializeAnimation()
{

}

//毎フレーム更新
void UEnemyAnimaion::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

//負荷分散用更新
void UEnemyAnimaion::NativeThreadSafeUpdateAnimation()
{
}

//アニメーション計算完了後に実行
void UEnemyAnimaion::NativePostEvaluateAnimation()
{
}

void UEnemyAnimaion::IsMovingEnemy()
{

}
