// Fill out your copyright notice in the Description page of Project Settings.


#include "SlashAttackNotify.h"
#include "NiagaraSystem.h" 

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Jhon2/Player/WorldTimeSetting.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void USlashAttackNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	//ボックストレースを使用して攻撃判定を出す
	//ボックストレースの各値を設定(のちにhに移行予定)


	if (AActor* MyPlayer = MeshComp->GetOwner())
	{
		if(UWorld* World = MyPlayer->GetWorld())
		{
			//箱のサイズ
			FVector BoxSize = FVector(200.f, 300.f, 100.f);

			//プレイヤーの位置(箱を出す位置に使う)
			FVector PlayerLocation = MyPlayer->GetActorLocation();
			//プレイヤーの向き(箱を出す向きに使う)
			FVector PlayerForward = MyPlayer->GetActorForwardVector();

			//箱の出す位置
			FVector BoxPosition = PlayerLocation + (PlayerForward * 200.f);
			
			//箱の向き(プレイヤーに合わせる)
			FRotator BoxRotation = MyPlayer->GetActorRotation();

			//ボックストレースで当たったものの情報を書き込んだりしてくれるものらしい
			FHitResult HitResult;
			TArray<AActor*> ActorToIgnore;
			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
			ActorToIgnore.Add(MyPlayer);


			//攻撃判定をだし結果をフラグにいれる
			bool Hit = UKismetSystemLibrary::BoxTraceSingleForObjects(
				World,BoxPosition,BoxPosition,BoxSize,BoxRotation,
				ObjectTypes,false,ActorToIgnore,EDrawDebugTrace::ForDuration,HitResult,true
			);

			EnemyHit(Hit , HitResult.GetActor(),MeshComp);

			if (Effect)
			{
				UNiagaraFunctionLibrary::SpawnSystemAttached(
					Effect, MeshComp, TEXT("BladeSocet"),
					FVector::ZeroVector,
					FRotator::ZeroRotator,
					EAttachLocation::KeepRelativeOffset,
					true
				);
			}


		}


	}


	//UE_LOG(LogTemp, Warning, TEXT("はじまり"));
}


void USlashAttackNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)

{
	//UE_LOG(LogTemp, Warning, TEXT("おわり"));
}

//void USlashAttackNotify::EnemyHit(bool HitBox, TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectType)
//{
//	if (HitBox) 
//	{
//		UGameplayStatics::ApplyDamage(
//			ObjectType,
//
//		)
//
//
//		UE_LOG(LogTemp, Warning, TEXT("あたりました"));
//	}
//}


//敵にヒットしたときに呼ばれる関数
void USlashAttackNotify::EnemyHit(bool HitBox, AActor* EnemyActor ,USkeletalMeshComponent* MeshComp2)
{
	if (HitBox)
	{
		//エネミーにダメージを渡す
		UGameplayStatics::ApplyDamage(
			EnemyActor,
			10.f,
			nullptr,
			MeshComp2->GetOwner(),
			nullptr
		);

		if (AActor* WorldActor = UGameplayStatics::GetActorOfClass(GetWorld(), AWorldTimeSetting::StaticClass()))
		{
			//世界から情報を受け取りヒットストップを起こす
			if (AWorldTimeSetting* WorldStop = Cast<AWorldTimeSetting>(WorldActor))
			{
				WorldStop->HitStop(8.f);
			}
		}

			//UE_LOG(LogTemp, Warning, TEXT("あたりました"));
	}
}

