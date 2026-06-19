// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bOrientRotationToMovement = true; // 移動方向に向く
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f); // 旋回速度（度/秒）好みで調整してください	}
	}
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//攻撃モンタージュの再生時間を返す
float AEnemyCharacter::PlayAttackAnimation()
{
	if (AttackMontage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		//Montageの再生時間を秒数で返す
		return PlayAnimMontage(AttackMontage);
	}
	return 0.f;
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//体力を減らす
	EnemyHP -= ActualDamage;

	//被弾ステート（のけぞり）に移行
	CurrentState = EEnemyState::Hit;

	//AIControllerを取得して、現在の行動（ビヘイビアツリー）を強制中断させる
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController && AIController->GetBrainComponent())
	{
		//ブラックボードの値を書き換える（ツリー側で感知させるため）
		UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
		if (BBComp)
		{
			// ※ブラックボードに「IsHit」などのBoolキーを追加しておくと制御が楽になります
			BBComp->SetValueAsBool(FName("IsHit"), true);
		}
	}

	//もし被弾アニメーション（モンタージュ）があればここで再生する
	// PlayAnimMontage(HitMontage);

	return EnemyHP;
}
