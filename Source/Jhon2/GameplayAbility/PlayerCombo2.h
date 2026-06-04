// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "PlayerCombo2.generated.h"

/**
 * 二段目
 * - 一段目同様の構造
 * - アニメーション再生後、三段目コンボタグを付与
 */
UCLASS()
class JHON2_API UPlayerCombo2 : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPlayerCombo2();

    //Abilityコンポーネント
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData
    ) override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
    UAnimMontage* AttackMontage;        //アニメーションモンタージュ

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo")
    FGameplayTag ComboContinueTag2;     //コンボ進行用タグ

    UFUNCTION()
    void OnMontageEnded();

};
