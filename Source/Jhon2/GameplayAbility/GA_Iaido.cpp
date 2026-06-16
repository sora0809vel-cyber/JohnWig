// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Iaido.h"
#include "Jhon2/Player/Player_2.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"

UGA_Iaido::UGA_Iaido()
{

}


void UGA_Iaido::ActivateAbility(
	const FGameplayAbilitySpecHandle IaidoHndle,
	const FGameplayAbilityActorInfo* IaidoActrorInfo,
	const FGameplayAbilityActivationInfo IaidoActivationInfo,
	const FGameplayEventData* IaidoTriggerEvent
)
{
	Super::ActivateAbility(IaidoHndle, IaidoActrorInfo, IaidoActivationInfo, IaidoTriggerEvent);
	


	if (IaidoStanceMontage)
	{

		UE_LOG(LogTemp, Warning, TEXT("yyyyy"));

		////アニメーション再生タスク
		//UAbilityTask_PlayMontageAndWait* IaidoStanceMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
		//	NAME_None, IaidoStanceMontage);

		//アニメーション終了タスク
		UAbilityTask_WaitGameplayEvent* IaidoStanceEndTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			this,
			FGameplayTag::RequestGameplayTag(FName("Iaido.End"))
		);

		//通知を受け取ったらIaidoEnd関数を呼ぶ
		IaidoStanceEndTask->EventReceived.AddDynamic(this, &UGA_Iaido::IaidoEnd);
		//終了のタスク有効化
		IaidoStanceEndTask->ReadyForActivation();




		////アニメーションタスク実行
		////IaidoStanceMontageTask->OnCompleted.AddDynamic(this, &UGA_Iaido::IaidoEnd);
		//IaidoStanceMontageTask->ReadyForActivation();

		//キャラクターとアニメーションインスタンス取得
		if (ACharacter* MyChar = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
		{
			if (UAnimInstance* AnimInstance = MyChar->GetMesh()->GetAnimInstance())
			{
				//アニメーション再生
				AnimInstance->Montage_Play(IaidoStanceMontage);

				//アニメーションの全体の秒数を取得
				float MontageLength = IaidoStanceMontage->GetPlayLength();

				//再生位置を最後の秒数に
				AnimInstance->Montage_SetPosition(IaidoStanceMontage, MontageLength);

				//アニメーション速度を0にする
				AnimInstance->Montage_SetPlayRate(IaidoStanceMontage, 0.0f);
			}
		}
	}
	else
	{
		//DodgeMontageEnd();
	}
}


void UGA_Iaido::IaidoEnd(FGameplayEventData Payload)
{
	//キャラクターとアニメーションインスタンス取得
	if (ACharacter* MyChar = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		if (UAnimInstance* AnimInstance = MyChar->GetMesh()->GetAnimInstance())
		{
			//アニメーション速度を1にする
			AnimInstance->Montage_SetPlayRate(IaidoStanceMontage, 1.0f);
		}
	}

	//アビリティ終了を伝える
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
