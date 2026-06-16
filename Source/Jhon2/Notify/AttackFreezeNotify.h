// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AttackFreezeNotify.generated.h"

/**
 * 
 */
UCLASS()
class JHON2_API UAttackFreezeNotify : public UAnimNotify
{
	GENERATED_BODY()


public:
	virtual void Notify(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* NowAnimation,
		const FAnimNotifyEventReference& EventReference
	)override;


protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AttackFreeze")
	FGameplayTag FreezeTag;


	
};
