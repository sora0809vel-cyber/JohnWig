// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SlashAttackNotify.generated.h"

class UNiagaraSystem;

/**
 * 
 */
UCLASS()
class JHON2_API USlashAttackNotify : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	virtual void NotifyBegin(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		float TotalDuration,
		const FAnimNotifyEventReference& EventReference
	)override;


	virtual void NotifyEnd(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference
	)override;


	//void EnemyHit(bool HitBox, TArray<TEnumAsByte<EObjectTypeQuery>>& EnemyActor);

	void EnemyHit(bool HitBox, AActor* EnemyActor, USkeletalMeshComponent* MeshComp2);

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Effects")
	TObjectPtr<UNiagaraSystem> Effect;

};
