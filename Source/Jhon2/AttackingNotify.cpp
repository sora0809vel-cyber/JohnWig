// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackingNotify.h"
#include "A_Player.h"


void UAttackingNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReferecence)
{
	if (AA_Player* Player = Cast<AA_Player>(MeshComp->GetOwner()))
	{
		Player->IsAttacking(false);
	}
}

void UAttackingNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (AA_Player* Player = Cast<AA_Player>(MeshComp->GetOwner()))
	{
		Player->IsAttacking(true);
	}
}


