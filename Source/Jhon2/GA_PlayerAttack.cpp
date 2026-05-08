// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_PlayerAttack.h"

UGA_PlayerAttack::UGA_PlayerAttack() :
	CurrentComboIndex(0),
	bComboBuffer(false)
{
	//アビリティのインスタンス化ポリシー。
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_PlayerAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData) {
	//最初の攻撃を開始
	PlayComboStep(CurrentComboIndex);

}

//ボタンが押されたら予約を入れる
void UGA_PlayerAttack::OnInputPressed() {
	bComboBuffer = true;
}

//アニメーション終了時にアビリティを終了させる
void UGA_PlayerAttack::OnMontageFinished() {
	//アニメーション中に次の入力があったか
	if (bComboBuffer) {
		bComboBuffer = false;
		CurrentComboIndex++;
		PlayComboStep(CurrentComboIndex);
	}

	else {
		//入力がなければ終了
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		CurrentComboIndex = 0;
	}
}

//Montage再生の制御をする関数
void UGA_PlayerAttack::PlayComboStep(int32 Index)
{
	UE_LOG(LogTemp, Warning, TEXT("ComboIndex: %i"), Index);
	if (!AttackMontage.IsValidIndex(Index)) {
		//Ability終了
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}

	//AbilityTask作成
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		AttackMontage[Index],
		1.0f,
		NAME_None
	);

	//終了時の処理をバインド
	Task->OnCompleted.AddDynamic(this, &UGA_PlayerAttack::OnMontageFinished);
	Task->OnInterrupted.AddDynamic(this, &UGA_PlayerAttack::OnMontageFinished);

	Task->ReadyForActivation();		//アニメーション再生
	
}
