// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "PlayerCombo3.generated.h"

/**
* 弱攻撃三段目用 Gameplay Ability
* - 最終段の攻撃を実装
* - タグ付与はせず、コンボ終了を示す
*/


UCLASS()
class JHON2_API UPlayerCombo3 : public UGameplayAbility
{
	GENERATED_BODY()

public:
	//コンストラクタ
	UPlayerCombo3();

	//Ability使用時に呼び出される処理
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

protected:
	//コンボ進行用タグ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo")
	FGameplayTag ComboContinueTag3;

	//攻撃アニメーション
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo")
	UAnimMontage* AttackMontage;

	//モンタージュ終了時の処理
	UFUNCTION()
	void OnMontageEnded();

};
