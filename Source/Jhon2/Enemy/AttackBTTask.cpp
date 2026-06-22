// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackBTTask.h"
#include "AIController.h"
#include "TimerManager.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

UAttackBTTask::UAttackBTTask()
{
	NodeName = TEXT("C++ Attack Task");

	//時間を要するタスクであることを示すフラグを立てる
	bNotifyTick = false;
}


EBTNodeResult::Type UAttackBTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	AEnemyCharacter* EnemyChar = Cast<AEnemyCharacter>(AIController->GetPawn());
	if (!EnemyChar) return EBTNodeResult::Failed;

	//攻撃中は注視を解除
	AIController->ClearFocus(EAIFocusPriority::Gameplay);

	//ABPでの状態確認用のStateを設定
	EnemyChar->CurrentState = EEnemyState::Attack;


	// アニメーションを再生し、長さを取得
	float MontageLength = EnemyChar->PlayAttackAnimation();

	if (MontageLength <= 0.0f) return EBTNodeResult::Failed;

	float DelayTime = 1.2f;
	FTimerHandle AttackDelayHandle;

	TWeakObjectPtr<AEnemyCharacter> WeakEnemyChar(EnemyChar);

	FTimerDelegate AttackDelegate = FTimerDelegate::CreateLambda([WeakEnemyChar]() {
		if (!WeakEnemyChar.IsValid()) return;
		AEnemyCharacter* Enemy = WeakEnemyChar.Get();
		UWorld* World = Enemy->GetWorld();
		if (!World) return;

		// 矩形の中心点を計算
		FVector EnemyLocation = Enemy->GetActorLocation();
		FVector EnemyForward = Enemy->GetActorForwardVector();
		float ForwardOffset = Enemy->BoxOffSet;
		FVector BoxCenter = EnemyLocation + (EnemyForward * ForwardOffset);

		// 矩形のサイズを設定
		FVector BoxHalfSize = FVector(Enemy->BoxLength, Enemy->BoxWidth, Enemy->BoxHeight);

		// 判定にひっかけるアクターの種類を指定
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

		// 無視するアクター
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(Enemy);

		// 判定内にいるアクターを格納する配列
		TArray<AActor*> OverlappedActors;

		// 矩形判定を実行
		bool bHit = UKismetSystemLibrary::BoxOverlapActors(
			World, BoxCenter, BoxHalfSize, ObjectTypes,
			ACharacter::StaticClass(), ActorsToIgnore, OverlappedActors
		);

		// デバッグ用の矩形を表示 (1.5秒間)
		UKismetSystemLibrary::DrawDebugBox(
			World, BoxCenter, BoxHalfSize, FLinearColor::Red,
			Enemy->GetActorRotation(), 1.5f, 1.0f
		);

		// 引っかかったアクターへの処理
		if (bHit)
		{
			for (AActor* HitActor : OverlappedActors)
			{
				ACharacter* PlayerChar = Cast<ACharacter>(HitActor);
				if (PlayerChar && PlayerChar->IsPlayerControlled())
				{
					UGameplayStatics::ApplyDamage(
						HitActor, 40.f,
						Enemy->GetController(), Enemy,
						UDamageType::StaticClass()
					);
				}
			}
		}
		});

	//タイマーstart
	EnemyChar->GetWorldTimerManager().SetTimer(AttackDelayHandle, AttackDelegate, DelayTime, false);

	// アニメーションの長さが終了したらタスクを完了させるタイマーを設定
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UAttackBTTask::OnAttackAnimationFinished, TWeakObjectPtr<UBehaviorTreeComponent>(&OwnerComp));

	// 敵キャラクターの世界のタイマーマネージャーを利用
	EnemyChar->GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, MontageLength, false);

	//タスクが実行中であることを返す
	return EBTNodeResult::InProgress;

}

void UAttackBTTask::OnAttackAnimationFinished(TWeakObjectPtr<UBehaviorTreeComponent> OwnerCompPtr)
{
	if (OwnerCompPtr.IsValid())
	{
		UBehaviorTreeComponent* BTComp = OwnerCompPtr.Get();
		AAIController* AIController = BTComp->GetAIOwner();

		if (AIController)
		{
			UBlackboardComponent* BBComp = BTComp->GetBlackboardComponent();
			if (BBComp)
			{
				AActor* PlayerActor = Cast<AActor>(BBComp->GetValueAsObject(FName("PlayerActor")));
				if (PlayerActor)
				{
					AIController->SetFocus(PlayerActor);
				}
			}
		}

		//Behavior Treeに終了したと通知する
		FinishLatentTask(*OwnerCompPtr.Get(), EBTNodeResult::Succeeded);
	}
}
