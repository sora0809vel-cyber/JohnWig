// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Iaido.h"
#include "Jhon2/Player/Player_2.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"

UGA_Iaido::UGA_Iaido()
{
	//State StanceState = State::Normal;

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
		if (StanceState != State::Stance)
		{

			////アニメーション再生タスク
			UAbilityTask_PlayMontageAndWait* IaidoStanceMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
				NAME_None, IaidoStanceMontage, 1.6f);

			//アニメーション停止タスク
			UAbilityTask_WaitGameplayEvent* FreezeTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
				this, FGameplayTag::RequestGameplayTag(TEXT("StanceFreeze")));

			////アニメーション終了タスク
			//UAbilityTask_WaitGameplayEvent* IaidoStanceEndTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
			//	this, FGameplayTag::RequestGameplayTag(FName("Iaido.End")));


			//通知を受け取ったらIaidoEnd関数を呼ぶ
			//IaidoStanceEndTask->EventReceived.AddDynamic(this, &UGA_Iaido::IaidoEnd);

				//実行可能にする
				IaidoStanceMontageTask->ReadyForActivation();
				//IaidoStanceEndTask->ReadyForActivation();

				//if (ACharacter* MyChar = Cast<ACharacter>(GetAvatarActorFromActorInfo())) 
				//{
				//	if (UAnimInstance* StanceInstance = MyChar->GetMesh()->GetAnimInstance())
				//	{
				//		StanceInstance->Montage_SetPlayRate(IaidoStanceMontage, 300.0f);
				//	}
				//}

				//フリーズのタスクがあったら
				if (FreezeTask)
				{



					//タグを受け取ると実行
					FreezeTask->EventReceived.AddDynamic(this, &UGA_Iaido::AnimationStop);
					FreezeTask->ReadyForActivation();
				}
				//StanceState = State::Stance;

		}
	}

}



//アニメーション停止
void UGA_Iaido::AnimationStop(FGameplayEventData Payload)
{
	//二回目の際に呼ばれるタスク
	UAbilityTask_WaitGameplayEvent* SecondAbilityTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag(FName("StanceAbility.Normal")));

	UE_LOG(LogTemp, Warning, TEXT("HIDAKA"));

	//キャラクターと
	if (ACharacter* MyChar = Cast<ACharacter>(GetAvatarActorFromActorInfo())) {
		//アニメーションインスタンスを取得
		if (UAnimInstance* StanceInstance = MyChar->GetMesh()->GetAnimInstance())
		{
			//モンタージュ取得
			UAnimMontage* NowStanceMontage = IaidoStanceMontage;

			//アニメーションを停止
			StanceInstance->Montage_SetPlayRate(NowStanceMontage, 0.f);			
		}
	}

	StanceState = State::Stance;


	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

	//タスクがあれば
	if (SecondAbilityTask)
	{
		//タグが届いたら納刀解除を実行
		SecondAbilityTask->EventReceived.AddDynamic(this, &UGA_Iaido::StanceEnd);
		SecondAbilityTask->ReadyForActivation();

	}


}

void UGA_Iaido::StanceEnd(FGameplayEventData Payload)
{
	IaidoEnd();
}


void UGA_Iaido::IaidoEnd()
{
	//キャラクターとアニメーションインスタンス取得
	if (ACharacter* MyChar = Cast<ACharacter>(GetAvatarActorFromActorInfo()))
	{
		if (UAnimInstance* AnimInstance = MyChar->GetMesh()->GetAnimInstance())
		{
			//モンタージュ取得
			UAnimMontage* NowStanceMontage = IaidoStanceMontage;
			//アニメーション速度を1にする
			AnimInstance->Montage_SetPlayRate(NowStanceMontage, 1.0f);
			AnimInstance->Montage_Resume(NowStanceMontage);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("HIRASAWA"));

	StanceState = State::Normal;

	//アビリティ終了を伝える
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

