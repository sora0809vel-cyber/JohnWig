// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h" //プレイヤーを検索するために必要
#include "EnemyIdle.h"
#include "EnemyChase.h"
#include "EnemyAttack.h"
#include "EnemyCooldown.h"

// Sets default values
AEnemyManager::AEnemyManager()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();

	EnemyIdleState = FindComponentByClass<class UEnemyIdle>();
	EnemyChaseState = FindComponentByClass<class UEnemyChase>();
	EnemyAttackState = FindComponentByClass<class UEnemyAttack>();
	EnemyCooldownState = FindComponentByClass<class UEnemyCooldown>();

	PlayerActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//敵の状態をアイドル状態で初期化
	//EMCurrentStateはEnemyManagerでの敵の状態参照用
	CurrentState = MakeUnique<IdleState>();

	//攻撃範囲の初期化
	BoxLength = 500.f;	//奥行
	BoxHeight = 200.f;	//高さ
	BoxWidth = 200.f;	//横幅



}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//矩形の中心点を定義
	BoxCenter = GetActorLocation() + (GetActorForwardVector() * (BoxLength / 2.f));
	//中心点から各壁までの距離
	BoxExtents = { BoxLength / 2.f, BoxWidth / 2.f, BoxHeight / 2.f };

	//敵とプレイヤーの現在位置を取得して比較
	if (PlayerActor)
	{
		FVector EnemyLocation = GetActorLocation();
		FVector PlayerLocation = PlayerActor->GetActorLocation();

		DistanceToPlayer = FVector::Dist2D(EnemyLocation, PlayerLocation);

	}

	//敵の状態を監視
	if (CurrentState)
	{
		HandleEnemyState();
	}

	//PIE中に攻撃範囲を床に円で描画する
	if (GetWorld())
	{
		FVector Center = GetActorLocation();
		// 円を床（足元）に合わせるために、少しZ軸を下げる（必要に応じて調整）
		Center.Z -= 90.f;

		// 足元に水平な円を描画
		DrawDebugCircle(
			GetWorld(),
			Center,
			AttackStateRange,                // 描画する半径
			32,                              // 円の滑らかさ（線の数）
			FColor::Red,                     // 線の色
			false,                           // 永続的に残すか（毎フレーム呼ぶのでfalse）
			-1.f,                            // 表示時間（秒）
			0,                               // 描画の優先順位
			2.f,                             // 線の太さ
			FVector(0, 1, 0),                // 円の向き
			FVector(1, 0, 0),
			false
		);
	}

	//PIE中に監視範囲を床に円で描画する
	if (GetWorld())
	{
		FVector Center = GetActorLocation();
		// 円を床（足元）に合わせるために、少しZ軸を下げる（必要に応じて調整）
		Center.Z -= 90.f;

		// 足元に水平な円を描画
		DrawDebugCircle(
			GetWorld(),
			Center,
			MonitoringRange,                 // 描画する半径
			32,                              // 円の滑らかさ（線の数）
			FColor::Red,                     // 線の色
			false,                           // 永続的に残すか（毎フレーム呼ぶのでfalse）
			-1.f,                            // 表示時間（秒）
			0,                               // 描画の優先順位
			2.f,                             // 線の太さ
			FVector(0, 1, 0),                // 円の向き
			FVector(1, 0, 0),
			false
		);
	}

	//敵の攻撃範囲を描画
	if (GetWorld())
	{
		if (IsAttack)
		{
			FQuat BoxRotation = GetActorQuat();

			DrawDebugBox(
				GetWorld(),
				BoxCenter,          // 矩形の中心座標
				BoxExtents,         // 矩形のサイズ（中心から各辺への半分の長さ）
				BoxRotation,		// 矩形の回転度合い
				FColor::Red,		// 線の色
				false,				// 毎フレーム描画するので永続化はfalse
				1.f,				// 表示時間(秒)
				0,					// 優先順位
				2.f					// 線の太さ
			);
		}
	}
}

// Called to bind functionality to input
void AEnemyManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//敵の状態ごとの処理
void AEnemyManager::HandleEnemyState()
{
	CurrentState->StateProcess(this, PlayerActor);
}

//敵の状態を変える関数
void AEnemyManager::ChangeState(EEnemyState newState)
{
	switch (newState)
	{
	default:
		break;
		//待機
	case EEnemyState::Idle:
		CurrentState = MakeUnique<IdleState>();
		ActiveState = EEnemyState::Idle;
		break;
		//追跡
	case EEnemyState::Chase:
		CurrentState = MakeUnique<ChaseState>();
		ActiveState = EEnemyState::Chase;
		break;
		//攻撃
	case EEnemyState::Attack:
		CurrentState = MakeUnique<AttackState>();
		ActiveState = EEnemyState::Attack;
		break;
		//後隙
	case EEnemyState::Cooldown:
		CurrentState = MakeUnique<CooldownState>();
		ActiveState = EEnemyState::Cooldown;
		break;
	}

}

//敵のHP減少関数
void AEnemyManager::EnemyDamaged(float DamageValue)
{
	EnemyHP -= DamageValue;
}

//待機状態の処理
void IdleState::StateProcess(AEnemyManager* EnemyCharacter, ACharacter* PlayerCharacter)
{
	if (EnemyCharacter && EnemyCharacter->EnemyIdleState)
	{
		EnemyCharacter->EnemyIdleState->Idle(EnemyCharacter);
	}
}

//追跡状態の処理
void ChaseState::StateProcess(AEnemyManager* EnemyCharacter, ACharacter* PlayerCharacter)
{
	if (EnemyCharacter && EnemyCharacter->EnemyChaseState)
	{
		EnemyCharacter->EnemyChaseState->Chase(EnemyCharacter, PlayerCharacter);
	}
}

//攻撃状態の処理
void AttackState::StateProcess(AEnemyManager* EnemyCharacter, ACharacter* PlayerCharacter)
{
	if (EnemyCharacter && EnemyCharacter->EnemyAttackState)
	{
		EnemyCharacter->EnemyAttackState->Attack(EnemyCharacter, PlayerCharacter);
	}
}

//後隙状態の処理
void CooldownState::StateProcess(AEnemyManager* EnemyCharacter, ACharacter* PlayerCharacter)
{
	if (EnemyCharacter && EnemyCharacter->EnemyCooldownState)
	{
		EnemyCharacter->EnemyCooldownState->Cooldown(EnemyCharacter);
	}
}