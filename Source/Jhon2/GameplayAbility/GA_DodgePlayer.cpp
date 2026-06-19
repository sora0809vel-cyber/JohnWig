// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_DodgePlayer.h"
//#include "WorldTimeSetting.h"
#include "TimerManager.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemComponent.h"
#include "Jhon2/Player/Player_2.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"


UGA_DodgePlayer::UGA_DodgePlayer() 
	//DodgeDistance(0.f)
{

}

void UGA_DodgePlayer::ActivateAbility(
	const FGameplayAbilitySpecHandle Dodging,
	const FGameplayAbilityActorInfo* DodgeActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* DodgeTriggerEvent
)
{
	
	Super::ActivateAbility(Dodging, DodgeActorInfo, ActivationInfo, DodgeTriggerEvent);

	//プレイヤーの情報を取得
	APlayer_2* Player2 = Cast<APlayer_2>(GetAvatarActorFromActorInfo());

	//プレイヤーのstatusが、構えていないなら
	if (Player2 && Player2->GetCurrentState() != Player2State::Stance)
	{
		//モンタージュがあるか確認
		if (DodgeMontage)
		{
			//再生のタスク
			UAbilityTask_PlayMontageAndWait* DodgeMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
				NAME_None, DodgeMontage);

			//タスクがあるなら
			if (DodgeMontageTask)
			{
				//アニメーション再生
				DodgeMontageTask->ReadyForActivation();
			}
		}
	}

	DodgeStart();
}


//回避開始
void UGA_DodgePlayer::DodgeStart()
{
	//せかいからタイマーをもらう
	GetWorld()->GetTimerManager().SetTimer(DodgeTimer, this, &UGA_DodgePlayer::IsDodge, 0.001f, true);

	//数秒後終了処理
	FTimerHandle EndDodgTimer;
	GetWorld()->GetTimerManager().SetTimer(EndDodgTimer, this, &UGA_DodgePlayer::DodgeEnd, 0.2f, false);

}

//回避開始
void UGA_DodgePlayer::IsDodge()
{

	if (APlayer_2* Player = Cast<APlayer_2>(GetAvatarActorFromActorInfo()))
	{
		//プレイヤーの向きを取得
		FVector DodgeRotate = Player->GetActorForwardVector();
		//正規化
		DodgeRotate.Normalize();
		//縦方向を0に
		DodgeRotate.Z = 0.f;
		float DodgeDistance = 0.f;

		//プレイヤーに入力があるかどうか
		if (Player->PlayerInput())
		{
			DodgeDistance = 150.f;
		}
		if (!Player->PlayerInput())
		{
			DodgeDistance = -100.f;
		}

		//プレイヤーの情報を取得
		APlayer_2* Player2 = Cast<APlayer_2>(GetAvatarActorFromActorInfo());

		if (Player2 && Player2->GetCurrentState() == Player2State::Stance)
		{
			DodgeDistance /= 2;
		}

		FVector DodgeLocation = Player->GetActorLocation() + (DodgeRotate * DodgeDistance * GetWorld()->DeltaTimeSeconds);

		//回避方向に移動
		//FVector NewLocation = GetActorLocation() + DodgeDirection * 5000.0f * GetWorld()->DeltaTimeSeconds;
		//SetActorLocation(NewLocation, true);
		Player->SetActorLocation(DodgeLocation, true);

	}

	//if(APlayerController* PC = Cast<APlayerController>(GetController()))


	//DodgeMontageEnd();
}

//回避終了
void UGA_DodgePlayer::DodgeEnd()
{
	//プレイヤーのクラスのアクターを取得出来たら
	if (APlayer_2* Player2 = Cast<APlayer_2>(GetAvatarActorFromActorInfo()))
	{
		if (Player2 && Player2->GetCurrentState() != Player2State::Stance)
		{
			//ステータスをアイドルにする関数を呼び出す
			Player2->SetStateIdle();
		}
	}


	GetWorld()->GetTimerManager().ClearTimer(DodgeTimer);

	DodgeMontageEnd();

}

bool UGA_DodgePlayer::DodgeMontagePlay()
{
	//構え中の回避にの際に呼ばれるタスク
	UAbilityTask_WaitGameplayEvent* NotPlayMontageTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, FGameplayTag::RequestGameplayTag(TEXT("DodgeAbility.NotPlayMontage")));

	//タグがあるなら
	if (NotPlayMontageTask)
	{
		//トゥルー
		return true;
	}
	//あるなら
	else
	{
		//ファルス返す
		return false;
	}


}


//アニメーション終了
void UGA_DodgePlayer::DodgeMontageEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}