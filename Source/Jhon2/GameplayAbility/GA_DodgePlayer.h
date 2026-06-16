// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA_PlayerAttack.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GA_DodgePlayer.generated.h"

/**
 * 
 */
UCLASS()
class JHON2_API UGA_DodgePlayer : public UGameplayAbility
{
	GENERATED_BODY()

	//dodgeタイマー
	FTimerHandle DodgeTimer;

public:
	
	UGA_DodgePlayer();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Dodging,
		const FGameplayAbilityActorInfo* DodgeActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* DodgeTriggerEvent
	) override;

protected:

	//回避の開始
	void DodgeStart();

	//回避の実行
	void IsDodge();

	//回避終了
	void DodgeEnd();

	

	//回避のタグ
	FGameplayTag PlayerDodgeTag = FGameplayTag::RequestGameplayTag(FName("Dodge"));
	
	//回避アニメーション
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Dodge")
	UAnimMontage* DodgeMontage;

	//UPROPERTY(EditAnywhere,BlueprintReadWrite)
	//float DodgeDistance;

	//終了時の処理
	void DodgeMontageEnd();
	 	
};
