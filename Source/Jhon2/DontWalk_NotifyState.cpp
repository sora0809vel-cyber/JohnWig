// Fill out your copyright notice in the Description page of Project Settings.


#include "DontWalk_NotifyState.h"
#include "A_Player.h"

void UDontWalk_NotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReferecence)
{
	if (AA_Player* Player = Cast<AA_Player>(MeshComp->GetOwner()))
	{
		Player->DontMove(false);
	}
}

void UDontWalk_NotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (AA_Player* Player = Cast<AA_Player>(MeshComp->GetOwner()))
	{
		Player->DontMove(true);
	}
}
