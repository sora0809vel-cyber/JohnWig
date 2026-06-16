// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkAnimInstance.h"
//プレイヤーファイルをインクルード
#include "Player_2.h"
#include "GameFramework/CharacterMovementComponent.h"

//コンストラクタ
UWalkAnimInstance::UWalkAnimInstance():

	//各変数の初期化
	CorrectionVector(0.0f),
	MoveComponent(nullptr),
	StickVector(FVector::ZeroVector),
	StickMagnitude(0.0f)
{
}

//毎フレーム更新
void UWalkAnimInstance::NativeUpdateAnimation(float Seconds)
{
	Super::NativeUpdateAnimation(Seconds);

	//componentを取得する関数を呼び出し、
	//コンポーネントを取得していなかったらreturn
	if (!GetCharacterMovementComponent()) 
	{
		return;
	}

	//stickの入力の値を獲得
	GetStickInput();

	//スティックの入力を補正
	CorrectStickInput();

	//値を代入
	AssignmentMagnitude();
	
}

//----------------------------------------------------------------------------------------------------------------------------------
//関数
//--------------------------------------------------------------------------------------------------------------------------------


//コンポーネントの情報を取得する関数
bool UWalkAnimInstance::GetCharacterMovementComponent()
{
	//親クラスをPlyaer_2クラスに変換？？？
	APlayer_2* ThisPlayer = Cast<APlayer_2>(TryGetPawnOwner());

	//変換できていなかったらfalseを返す
	if (ThisPlayer == nullptr)
	{
		return false;
	}

	//キャラクタームーブメントコンポーネント取得
	MoveComponent = ThisPlayer->GetCharacterMovement();

	//無かったらfalseを返す
	if (MoveComponent == nullptr)
	{
		return false;
	}

	//どちらもあったら
	return true;
}


//stickの値を取得する関数
void UWalkAnimInstance::GetStickInput() 
{
	//コンポーネントからstickの入力の値を取得
	StickVector = MoveComponent->GetLastInputVector();

	//補正用の変数にスティックの入力の大きさを取得
	StickMagnitude = StickVector.Size();

}

//stickの値を受け取り補正をかける関数
void UWalkAnimInstance::CorrectStickInput()
{
	//stickの入力されていたら
	if (StickMagnitude > 0.f)
	{
		//一定の範囲に
		CorrectionVector = FMath::Clamp(StickMagnitude, 0.f, 1.2f);

		//補正した値を代入
		StickMagnitude = CorrectionVector;
	}
	else
	{

		// スティックを離したら停止
		CorrectionVector = 0.0f;

		//補正した値を代入
		StickMagnitude = CorrectionVector;

	}
}

//値を代入する関数
void UWalkAnimInstance::AssignmentMagnitude()
{
	//値を代入
	DashAnimationSpeed = StickMagnitude;
	WalkAnimationSpeed = StickMagnitude * 2;
}


//--------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------





