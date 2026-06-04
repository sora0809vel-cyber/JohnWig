// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombo3.h"
#include "AbilitySystemComponent.h"

UPlayerCombo3::UPlayerCombo3()
{
}

void UPlayerCombo3::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//AttackMontageがあるか確認
	if (AttackMontage) {
		//アニメーション再生タスクを生成し、アニメ完了を待機
		UAbilityTask_PlayMontageAndWait* MontageTask =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
				NAME_None, AttackMontage);

		//モンタージュ終了通知に関数をバインド
		MontageTask->OnCompleted.AddDynamic(this, &UPlayerCombo3::OnMontageEnded);
		MontageTask->ReadyForActivation();
	}
	else {
		//アニメ無しはAbilityを終了
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UPlayerCombo3::OnMontageEnded()
{
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo()) {
		ASC->AddLooseGameplayTag(ComboContinueTag3);
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
