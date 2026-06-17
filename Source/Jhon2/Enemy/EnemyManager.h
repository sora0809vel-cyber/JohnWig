// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyManager.generated.h"

//前方宣言
class UEnemyIdle;
class UEnemyChase;
class UEnemyAttack;
class UEnemyCooldown;

//敵の状態を列挙
UENUM(BlueprintType)
enum class EEnemyState : uint8 {
	Idle		UMETA(DisplayName = "Idle"),		//待機
	Chase		UMETA(DisplayName = "Chase"),		//追跡
	Attack		UMETA(DisplayName = "Attack"),		//攻撃
	Cooldown	UMETA(DisplayName = "Cooldown")	//後隙
};

//敵キャラクターの状態の親クラス
class EnemyState {
public:
	//各状態の振る舞いを純粋仮想関数として定義
	virtual void StateProcess(AEnemyManager* EnemyCharacter, ACharacter* PlayerCharacter)
	{

	};

};

//敵の待機クラス
class IdleState : public EnemyState {
public:
	void StateProcess(AEnemyManager* EnemyCharacter, ACharacter* PlayerCharacter) override;
};

//敵の追跡クラス
class ChaseState : public EnemyState {
public:
	void StateProcess(AEnemyManager* EnemyCharacter, ACharacter* PlayerCharacter) override;
};

//敵の攻撃クラス
class AttackState : public EnemyState {
public:
	void StateProcess(AEnemyManager* EnemyCharacter, ACharacter* PlayerCharacter) override;
};

//敵の後隙クラス
class CooldownState : public EnemyState {
public:
	void StateProcess(AEnemyManager* EnemyCharacter, ACharacter* PlayerCharacter) override;
};

UCLASS()
class JHON2_API AEnemyManager : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyManager();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//敵の状態ごとの処理
	//カプセル化を維持するため定義
	void HandleEnemyState();

	//状態を変える関数
	void ChangeState(EEnemyState newState);

	//プレイヤーの攻撃によってHPを減らす関数
	void EnemyDamaged(float DamageValue);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//視認できる範囲
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|State")
	float MonitoringRange;
	//攻撃状態に移行する範囲
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|State")
	float AttackStateRange;

	//攻撃のモンタージュ
	UPROPERTY(EditAnywhere, Category = "Enemy|Animation")
	UAnimMontage* AttackMontage;

	//攻撃後のクールダウン時間
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Param")
	float AttackCooldownDuration = 1.5f;

	//現在の状態を保存する変数
	TUniquePtr<EnemyState> CurrentState;

	//プレイヤーとの距離
	UPROPERTY(BlueprintReadOnly, Category = "Enemy|Param")
	float DistanceToPlayer;

	//プレイヤー参照用変数
	ACharacter* PlayerActor;

	//内部処理用
	float CooldownTimer;

	//敵の攻撃範囲
	UPROPERTY(BlueprintReadOnly, Category = "Enemy|Param")
	FVector AttackRange;

	//敵の攻撃範囲の矩形のサイズ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Param")
	float BoxLength;	//奥行
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Param")
	float BoxWidth;		//横幅
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Param")
	float BoxHeight;	//高さ

	//敵の攻撃範囲の矩形の中心点
	UPROPERTY(BlueprintReadOnly, Category = "Enemy|Param")
	FVector BoxCenter;

	//矩形の中心点から各壁までの距離
	//プレイヤーとの衝突判定で使う
	FVector BoxExtents;

	//敵のHP
	UPROPERTY(BlueprintReadOnly, Category = "Enemy|Param")
	float EnemyHP = 100.f;
	//現在攻撃中かどうかのフラグ
	bool IsAttack = false;

	//参照用変数
	UEnemyIdle* EnemyIdleState;
	UEnemyChase* EnemyChaseState;
	UEnemyAttack* EnemyAttackState;
	UEnemyCooldown* EnemyCooldownState;

	//ABPから読み取るための変数
	UPROPERTY(BlueprintReadOnly,Category = "Enemy|Param")
	EEnemyState ActiveState = EEnemyState::Idle;

private:

};
