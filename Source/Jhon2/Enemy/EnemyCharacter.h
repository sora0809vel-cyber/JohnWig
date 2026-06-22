// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"
UENUM(BlueprintType)
enum class EEnemyState : uint8 {
	Idle			UMETA(DisplayName = "Idle"),		//待機
	Chase			UMETA(DisplayName = "Chase"),		//追跡
	Attack			UMETA(DisplayName = "Attack"),		//攻撃
	Cooldown		UMETA(DisplayName = "Cooldown"),	//後隙
	Hit				UMETA(DisplayName = "Hit")			//被弾
};

UCLASS()
class JHON2_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//攻撃モンタージュの再生時間を返す関数
	float PlayAttackAnimation();

	//受けた攻撃分体力を減らし、現在HPを返す
	virtual float TakeDamage(float DamageAmount,FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
public:
	//敵のHP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Param")
	float EnemyHP;

	//敵の攻撃範囲
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "AI|Param")
	float AttackRange;

	//現在の状態
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "AI|Param")
	EEnemyState CurrentState = EEnemyState::Idle;

	//敵の攻撃判定の大きさ(幅、高さ、奥行)
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "AI|Param")
	float BoxWidth;		//幅
	UPROPERTY(EditAnywhere,	BlueprintReadOnly, Category = "AI|Param")
	float BoxHeight;	//高さ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Param")
	float BoxLength;	//奥行
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Param")
	float BoxOffSet;	//矩形をどれだけ前に出すか


protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "AI|Attack")
	UAnimMontage* AttackMontage;
};
