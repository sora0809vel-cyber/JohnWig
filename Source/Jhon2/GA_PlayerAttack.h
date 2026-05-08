// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

// アビリティタスク：モンタージュ再生用
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
// アビリティタスク：GameplayEvent（Notify）待機用
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

// アビリティタスク：入力待機用
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"

// ASC（Ability System Component）の機能を使用するため
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

// タグ（FGameplayTag）を扱うため
#include "GameplayTagContainer.h"
#include "GA_PlayerAttack.generated.h"

/**
 * 
 */
UCLASS()
class JHON2_API UGA_PlayerAttack : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_PlayerAttack();

	//アビリティが開始されたとき
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	//コンボに使用するモンタージュ
	UPROPERTY(EditDefaultsOnly,Category = "Effects")
	TArray<UAnimMontage*> AttackMontage;

	//現在のコンボ段数
	int32 CurrentComboIndex = 1;

	//先行入力（バッファ）があるかどうか
	bool bComboBuffer = false;

protected:

	//入力（連打）があった時のコールバック
	UFUNCTION()
	void OnInputPressed();
	//Montage終了関数
	UFUNCTION()
	void OnMontageFinished();

	//Montage再生の制御関数
	UFUNCTION()
	void PlayComboStep(int32 Index);
};
