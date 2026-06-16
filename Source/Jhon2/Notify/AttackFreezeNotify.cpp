// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackFreezeNotify.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"

void UAttackFreezeNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* NowAnimation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, NowAnimation, EventReference);

	//meshがあるなら
	if (MeshComp) 	
	{
		//アクターの存在確認とその取得
		if (AActor* OwnerActor = MeshComp->GetOwner())
		{
			//アビリティシステムコンポーネントの存在の確認と取得
			if (UAbilitySystemComponent* ASComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor))
			{
				//アビリティが判別するための情報たち
				FGameplayEventData FreezeEventData;
				FreezeEventData.Instigator = OwnerActor;
				FreezeEventData.OptionalObject = NowAnimation;

				//停止のタグを飛ばす
				ASComponent->HandleGameplayEvent(FreezeTag, &FreezeEventData);
			}
		}
	}
}
