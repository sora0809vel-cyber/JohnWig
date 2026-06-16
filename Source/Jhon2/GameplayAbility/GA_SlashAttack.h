// Fill out your copyright notice in the Description page of Project Settings.cpp

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GA_SlashAttack.generated.h"

/**
 *
 */
UCLASS()
class JHON2_API UGA_SlashAttack : public UGameplayAbility
{
	GENERATED_BODY()

public:

	//コンストラクタ
	UGA_SlashAttack();


	//Ability使用時に呼び出される処理
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle SlashjHandle,
		const FGameplayAbilityActorInfo* SlashActorInfo,
		const FGameplayAbilityActivationInfo SlashActivationInfo,
		const FGameplayEventData* SlashTriggerEventData
	) override;


	//子のクラスが使う共通のアニメーションモンタージュたち
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slash")
	TArray<class UAnimMontage*> SlashMontages;

	//子のクラスが呼び出す関数
	UFUNCTION(BlueprintCallable, Category = "Slash")
	void PlayIaiStage(int32 StageIndex);

	//アビリティを終了させる処理
	UFUNCTION()
	void SlashAbilityEnd();

	//アニメーション終了時に呼ばれる処理
	UFUNCTION()
	void SlashMontageEnd();

	//アニメーション中断時に呼ばれる関数
	UFUNCTION()
	void SlashMontageCancel();

	UFUNCTION()
	void FreezeAtLastFrame(FGameplayEventData EventData);

	//次に再生するアニメーションのタグを渡す関数
	UFUNCTION()
	FGameplayTag SetNextSlashTag();

	//アタック終わり
	//UFUNCTION()
	//void ReleseAttack();

protected:
	int32 SlashIndex;


private:

	UAbilitySystemComponent* ASC;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttackFreeze")
	//FGameplayTag FreezeTag;


};
