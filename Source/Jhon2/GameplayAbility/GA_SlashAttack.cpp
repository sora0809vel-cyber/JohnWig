// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_SlashAttack.h"
#include "Jhon2/Player/Player_2.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"


UGA_SlashAttack::UGA_SlashAttack() :
	SlashIndex(0),
	ASC(nullptr)
{

}

void UGA_SlashAttack::ActivateAbility(
	const FGameplayAbilitySpecHandle SlashjHandle,
	const FGameplayAbilityActorInfo* SlashActorInfo,
	const FGameplayAbilityActivationInfo SlashActivationInfo,
	const FGameplayEventData* SlashTriggerEventData
)
{
	//アビリティシステムの本体を格納
	ASC = GetAbilitySystemComponentFromActorInfo();

	if (!CommitAbility(SlashjHandle, SlashActorInfo, SlashActivationInfo))
	{
		//終了の関数を呼ぶ
		SlashAbilityEnd();
		return;
	}

	//再生するモンタージュ
	int32 AutoStageIndex = 0;


	//タグを判断する
	//タグが2なら
		if (AbilityTags.HasTag(FGameplayTag::RequestGameplayTag(TEXT("AbilityAttack.Slash2"))))
		{
			AutoStageIndex = 1;
		}
		else if (AbilityTags.HasTag(FGameplayTag::RequestGameplayTag(TEXT("AbilityAttack.Slash3"))))
		{
			AutoStageIndex = 2;
		}

	//関数を呼び出す
	PlayIaiStage(AutoStageIndex);
}

//
void UGA_SlashAttack::PlayIaiStage(int32 StageIndex)
{
	//配列を持っているか
	if (!SlashMontages.IsValidIndex(StageIndex) || SlashMontages[StageIndex] == nullptr)
	{
		//アビリティを終了する関数を呼び出す
		SlashAbilityEnd();
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("hikiti"));

	//子オブジェクトから再生するアニメーションを取得
	UAnimMontage* executeMontage = SlashMontages[StageIndex];

	//アニメーション再生のタスク
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, executeMontage);

	//アニメーション停止のタスク
	UAbilityTask_WaitGameplayEvent* FreezeTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, FGameplayTag::RequestGameplayTag(TEXT("AttackFreeze")));



	//現在の再生されているアニメーションの秒数を取得
	SlashIndex = StageIndex;

	//アニメーション再生するタスクがあるなら
	if (MontageTask)
	{
		//アニメーション終了のイベントを登録
		MontageTask->OnCompleted.AddDynamic(this, &UGA_SlashAttack::SlashMontageEnd);
		//アニメーション中断のイベントを登録
		MontageTask->OnInterrupted.AddDynamic(this, &UGA_SlashAttack::SlashMontageCancel);

		//実行可能にする
		MontageTask->ReadyForActivation();
	}

	//アニメーション停止のタスクがあるなら
	if (FreezeTask)
	{
		//ノーティファイを受けとった時のイベントの登録
		FreezeTask->EventReceived.AddDynamic(this, &UGA_SlashAttack::FreezeAtLastFrame);

		//実行可能にする
		FreezeTask->ReadyForActivation();

	}
}

//アニメーションを停止する処理
void UGA_SlashAttack::FreezeAtLastFrame(FGameplayEventData EventData)
{

	//キャラクターと
	if (ACharacter* MyChar = Cast<ACharacter>(GetAvatarActorFromActorInfo())) {
		//アニメーションインスタンスを取得
		if (UAnimInstance* SlashAnimInstance = MyChar->GetMesh()->GetAnimInstance())
		{
			//現在再生中のモンタージュ取得
			if (SlashMontages.IsValidIndex(SlashIndex))
			{


				//モンタージュ取得
				UAnimMontage* NowMontage = SlashMontages[SlashIndex];

				//アニメーションを停止
				SlashAnimInstance->Montage_SetPlayRate(NowMontage, 0.f);


				//タグを渡す
				ASC->AddLooseGameplayTag(SetNextSlashTag());


				//タイマーの変数
				FTimerHandle EndSlashAttack;

				if (UWorld* World = GetWorld())
				{

					//0.5秒経ったらアビリティ終了(再生)
					World->GetTimerManager().SetTimer(
						EndSlashAttack, this, &UGA_SlashAttack::SlashMontageEnd, 0.5f, false);
				}

			}
		}
	}
}

////アニメーション再開し終了
//void UGA_SlashAttack::ReleseAttack()
//{
//	//キャラクターと
//	if (ACharacter* MyChar = Cast<ACharacter>(GetAvatarActorFromActorInfo())) {
//		//アニメーションインスタンスを取得
//		if (UAnimInstance* SlashAnimInstance = MyChar->GetMesh()->GetAnimInstance())
//		{
//			//現在再生中のモンタージュ取得
//			if (SlashMontages.IsValidIndex(SlashIndex))
//			{
//				UAnimMontage* NowMontage = SlashMontages[SlashIndex];
//
//				//アニメーションの総再生時間を取得
//
//				SlashAnimInstance->Montage_SetPlayRate(NowMontage, 1.f);
//			}
//
//			SlashAbilityEnd();
//
//		}
//	}
//}

//次の攻撃に派生するタグを渡す
FGameplayTag UGA_SlashAttack::SetNextSlashTag()
{
	//派生するためのタグ
	FGameplayTag NextComboTag;

	//流れているアニメーションが
	//一つ目なら
	if (SlashIndex == 0)
	{
		NextComboTag = FGameplayTag::RequestGameplayTag(TEXT("IaiCombo.NextSlash1"));
	}
	//二つ目なら
	else if (SlashIndex == 1)
	{
		NextComboTag = FGameplayTag::RequestGameplayTag(TEXT("IaiCombo.NextSlash2"));
	}
	//三つ目なら
	else if (SlashIndex == 2)
	{
		//派生しないためのタグを渡す
		NextComboTag = FGameplayTag::RequestGameplayTag(TEXT("IaiCombo.ComboEnd"));
	}

	//タグを返す
	return NextComboTag;

}


//アビリティを終了する関数
void UGA_SlashAttack::SlashAbilityEnd()
{
	//終了
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


void UGA_SlashAttack::SlashMontageCancel()
{
	//UE_LOG(LogTemp, Warning, TEXT("攻撃が中断した"));

	SlashAbilityEnd();
}

 
//アニメーション終了時に呼ばれる関数
void UGA_SlashAttack::SlashMontageEnd()
{
	//各タグを削除
	ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("IaiCombo.NextSlash1")));
	ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("IaiCombo.NextSlash2")));
	ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("IaiCombo.ComboEnd")));


	//プレイヤーのクラスのアクターを取得出来たら
	if (APlayer_2* Player2 = Cast<APlayer_2>(GetAvatarActorFromActorInfo()))
	{

		//ステータスをアイドルにする関数を呼び出す
		Player2->SetStateIdle();
	}


	//UE_LOG(LogTemp, Warning, TEXT("攻撃が終了した"));
	//終了する関数を呼ぶ
	SlashAbilityEnd();
}
