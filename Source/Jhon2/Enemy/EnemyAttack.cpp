// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAttack.h"
#include "EnemyManager.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UEnemyAttack::UEnemyAttack()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyAttack::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UEnemyAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEnemyAttack::Attack(AEnemyManager* EnemyCharacter, ACharacter* PlayerCharacter)
{
	EnemyCharacter->IsAttack = true;

	//プレイヤーのほうを向かせる
	FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	FVector Direction = (PlayerLocation - EnemyCharacter->GetActorLocation()).GetSafeNormal();
	FRotator NewRotation = Direction.Rotation();
	NewRotation.Pitch = 0.f;
	NewRotation.Roll = 0.f;
	EnemyCharacter->SetActorRotation(NewRotation);

	//敵の攻撃とプレイヤーの衝突判定
	bool IsInside = UKismetMathLibrary::IsPointInBox(PlayerLocation, EnemyCharacter->BoxCenter, EnemyCharacter->BoxExtents);

	//攻撃が当たっていたら
	if (IsInside)
	{
		//衝突時の判定を書く
	}

	if (EnemyCharacter->IsAttack)
	{
		if (EnemyCharacter->AttackMontage)
		{
			//アニメーションの再生時間を取得して、クールダウンタイマーに代入する
			float Duration = EnemyCharacter->PlayAnimMontage(EnemyCharacter->AttackMontage);
			EnemyCharacter->CooldownTimer = (Duration > 0.f) ? Duration : EnemyCharacter->AttackCooldownDuration;
		}
		else
		{
			//アニメーションがない場合のフォールバック
			EnemyCharacter->CooldownTimer = EnemyCharacter->AttackCooldownDuration;
		}
		EnemyCharacter->IsAttack = false;
		//攻撃が終わったらCooldownに移行
		EnemyCharacter->ChangeState(EEnemyState::Cooldown);
	}


}

