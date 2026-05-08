// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimaion.generated.h"

/**
 * 
 */

//Enemyの状態遷移用enmuClass
UENUM(BlueprintType)
enum class EEnemyState : uint8 {
	Unaware UMETA(DisplayName = "Unaware"),		//プレイヤーに気づいていない
	Alerted UMETA(DisplayName = "Alerted")		//プレイヤーに気づいた
};

UCLASS()
class JHON2_API UEnemyAnimaion : public UAnimInstance
{
	GENERATED_BODY()

	void NativeInitializeAnimation();		//初期化
	void NativeUpdateAnimation(float DeltaSeconds);			//毎フレーム更新
	void NativeThreadSafeUpdateAnimation();	//スレッドセーフな更新用。負荷分散に最適
	void NativePostEvaluateAnimation();		//アニメーション計算完了後に実行。ボーン操作の最終調整とか
	
public:
	//
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Animation")
	float MoveSpeed;
	//プレイヤーからの距離
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Animation")
	float DistanceToPlayer;
	//エネミーAIコントローラー取得
	UPROPERTY()
	class AEnemyCharacter* EnemyCharacter;
	//エネミーが動いているか
	UFUNCTION()
	void IsMovingEnemy();
	//現在の状態
	UPROPERTY(BlueprintReadOnly,Category = "EnemyState")
	EEnemyState CurrentState;
};
