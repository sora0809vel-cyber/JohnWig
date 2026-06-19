// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GA_Iaido.generated.h"

/**
 * 
 */
UCLASS()
class JHON2_API UGA_Iaido : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	UGA_Iaido();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle IaidoHndle,
		const FGameplayAbilityActorInfo* IaidoActrorInfo,
		const FGameplayAbilityActivationInfo IaidoActivationInfo,
		const FGameplayEventData* IaidoTriggerEvent
	) override;

protected:

	enum class State : uint8
	{
		//通常状態
		Normal,
		//構え中
		Stance,
	};

	State StanceState = State::Normal;


	//居合のタグ
	FGameplayTag PlayerIaidoTag = FGameplayTag::RequestGameplayTag(FName("Iaido"));

	//納刀アニメーション
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Iaido")
	UAnimMontage* IaidoStanceMontage;

	//アニメーションを停止する関数
	UFUNCTION()
	void AnimationStop(FGameplayEventData Payload);

	//納刀解除時に出てくる関数
	UFUNCTION()
	void StanceEnd(FGameplayEventData Payload);



	//終了時の処理
	UFUNCTION()
	void IaidoEnd();

};
