// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Animation/AnimInstance.h"
#include "Player_2.generated.h"


//クラスの前方宣言
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UAbilitySystemComponent;

UENUM(BlueprintType)
enum class Player2State : uint8
{
	Idle,
	Move,
	Stance,
	Slash,
	Dodge
};



UCLASS()
class JHON2_API APlayer_2 : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()


	bool CanMove;		//移動できるか
	bool CanAttacking;	//攻撃できるか
	bool Dodging;	//回避しているか
	bool moving;	//移動しているか



	
	FVector DodgeDirection;	//回避方向を記録


public:
	// Sets default values for this character's properties
	APlayer_2();

	//追加したAbilityシステムのコンポネント取得
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;


	//変数宣言
	UPROPERTY(EditAnywhere)
	float PlayerSpeed;

protected:


	//プレイヤーstatus宣言
	//UPROPERTY(BluprintiReadOnly, Category = "PlayerState")
	Player2State CurrentPlayerState = Player2State::Idle;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//プレイヤーIMC、エディタ編集可
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player_Input")
	class UInputMappingContext* DefaultMappingContext;
	//プレイヤーの移動IA、エディタ編集可
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player_Input")
	class UInputAction* MoveAction;
	//プレイヤーの視点移動IA、エディタ編集可
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player_Input")
	class UInputAction* LookAction;
	//プレイヤーの攻撃IA
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player_Input")
	class UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player_Input")
	class UInputAction* DodgeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player_Input")
	class UInputAction* IaidoAction;





	//カメラをプレイヤーの後ろに固定するスプリングアーム
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	//Abilityシステムコンポーネントクラス変数
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;

	//攻撃開始用のアビリティタグやクラス
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TSubclassOf<class UGA_SlashAttack> AttackSlash1;
	//攻撃開始用のアビリティタグやクラス
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TSubclassOf<class UGA_SlashAttack> AttackSlash2;
	//攻撃開始用のアビリティタグやクラス
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TSubclassOf<class UGA_SlashAttack> AttackSlash3;





	//回避のアビリティ
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<class UGameplayAbility> DodgeAbility;
	//納刀のアビリティ
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<class UGameplayAbility> IaidoStanceAbility;




	//回避を一定時間ごとに実行するタイマー
	FTimerHandle DodgeTimer;
	//回避する間隔
	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	float DodgeIntervalTime;
	//回避する時間
	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	float DodgeEndTime;


	//プレイヤーの移動速度
	UPROPERTY(EditDefaultsOnly, Category = "PlayerMove")
	float PrayerMoveSpeed;





public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//移動用関数
	void Move(const FInputActionValue& Value);

	void MoveEnd(const FInputActionValue& Value);


	//視点移動関数
	void Look(const FInputActionValue& Value);
	//攻撃関数
	void Attack(const FInputActionValue& Value);

	//回避を始める瞬間を通知する処理
	void IsDodge(const FInputActionValue& Value);
	//回避から抜刀の構えと解除
	void IaidoStanceChange(const FInputActionValue& Value);
	//抜刀
	//void DrawnSword(const FInputActionValue& Value);

	//移動可能か調べる関数
	void DontMove(bool move);
	//攻撃中かどうか
	void IsAttacking(bool IsAttacking);
	//回避可能か調べる関数
	void IsDodge(bool dodge);

	//ステータスをアイドルにする関数
	void SetStateIdle();

	//ステータスを納刀にする
	void SetStateStance();

	////ステータスを変更する
	//void ChangeState(PlayerState NewState);

	//
	void StanseStart();

	//stickやWASDの入力があるかを判断する関数
	UFUNCTION(BlueprintCallable, Category = "Inputbool")
	bool PlayerInput();

	//ステートを渡す
	Player2State GetCurrentState() const { return CurrentPlayerState; }

	//UPROPERTY(EditAnywhere)
	FVector2D MovementVector;
};
