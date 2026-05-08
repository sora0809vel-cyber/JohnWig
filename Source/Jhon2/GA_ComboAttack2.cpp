// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_ComboAttack2.h"
#include "AbilitySystemComponent.h"

UGA_ComboAttack2::UGA_ComboAttack2() {
	//二段目コンボ継続タグ
	ComboContinueTag2 =
		FGameplayTag::RequestGameplayTag(FName("Combo.Continue2"));
}

void UGA_ComboAttack2::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData
) {
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//AttackMontageがあるか確認
	if (AttackMontage) {
		//アニメーション再生タスクを生成し、アニメ完了を待機
		UAbilityTask_PlayMontageAndWait* MontageTask =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
				NAME_None, AttackMontage);

		//モンタージュ終了通知に関数をバインド
		MontageTask->OnCompleted.AddDynamic(this, &UGA_ComboAttack2::OnMontageEnded);
		MontageTask->ReadyForActivation();
	}
	else {
		//アニメ無しはAbilityを終了
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UGA_ComboAttack2::OnMontageEnded() {
	if (UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo()) {
		ASC->AddLooseGameplayTag(ComboContinueTag2);
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

