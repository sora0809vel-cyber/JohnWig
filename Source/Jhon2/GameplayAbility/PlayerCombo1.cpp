// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombo1.h"
#include "AbilitySystemComponent.h"

//コンストラクタ内でコンボ継続タグを初期化
UPlayerCombo1::UPlayerCombo1()
{
	//コンボ続行を示すタグ。EditorかDataTableで追加管理したほうが拡張しやすい
	ComboContinueTag = FGameplayTag::RequestGameplayTag(FName("Combo.Continue"));
}

void UPlayerCombo1::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//AttackMontageがあるか確認
	if (AttackMontage) {
		//アニメーション再生タスクを生成し、アニメ完了を待機
		UAbilityTask_PlayMontageAndWait* MontageTask =
			UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,
				NAME_None, AttackMontage);

		//モンタージュ終了通知に関数をバインド
		MontageTask->OnCompleted.AddDynamic(this, &UPlayerCombo1::OnMontageEnded);
		MontageTask->ReadyForActivation();
	}
	else {
		//アニメ無しはAbilityを終了
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}

}

void UPlayerCombo1::OnMontageEnded()
{
}
