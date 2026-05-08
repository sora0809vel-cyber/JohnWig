// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AbilitySystemComponent.h"
#include "AttackingNotify.generated.h"

/**
 * 
 */

//前方宣言
class AbilitySystemComponent;

UCLASS()
class JHON2_API UAttackingNotify : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReferecence) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)override;
	//入力が来たときに呼ばれる関数
	void OnInputPressed();
};
