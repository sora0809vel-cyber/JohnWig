// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GA_ComboAttack1.generated.h"

/**
 * 一段目攻撃
 * - 攻撃アニメーション再生
 * - 終了時にコンボ継続タグを付与し、二段目の入力受付状態にする
 */

UCLASS()
class JHON2_API UGA_ComboAttack1 : public UGameplayAbility
{
	GENERATED_BODY()


public: 
	//コンストラクタ
	UGA_ComboAttack1();

	//Ability使用時に呼び出される処理
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

protected:
	//コンボ進行用タグ
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Combo")
	FGameplayTag ComboContinueTag;

	//攻撃アニメーション
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Combo")
	UAnimMontage* AttackMontage;

	//モンタージュ終了時の処理
	UFUNCTION()
	void OnMontageEnded();
	
};
