// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "A_Player.generated.h"

//クラスの前方宣言
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UAbilitySystemComponent;

UCLASS()
class JHON2_API AA_Player : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()


	
	bool CanMove;		//移動できるか
	bool CanAttacking;	//攻撃できるか



public:
	// Sets default values for this character's properties
	AA_Player();

	//追加したAbilityシステムのコンポネント取得
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;


	//変数宣言
	UPROPERTY(EditAnywhere)
	float PlayerSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//プレイヤーIMC、エディタ編集可
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;
	//プレイヤーの移動IA、エディタ編集可
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* MoveAction;
	//プレイヤーの視点移動IA、エディタ編集可
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* LookAction;
	//プレイヤーの攻撃IA
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* AttackAction;

	//カメラをプレイヤーの後ろに固定するスプリングアーム
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	//Abilityシステムコンポーネントクラス変数
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;

	//攻撃開始用のアビリティタグやクラス
	UPROPERTY(EditAnywhere,Category = "GAS")
	TSubclassOf<class UGameplayAbility> AttackAbilityClass;


	//Abilityクラス変数
	UPROPERTY(EditDefaultsOnly,Category = "Abilities")
	TSubclassOf<class UGameplayAbility> ComboAttack1;
	UPROPERTY(EditDefaultsOnly,Category = "Abilities")
	TSubclassOf<class UGameplayAbility> ComboAttack2;
	UPROPERTY(EditDefaultsOnly,Category = "Abilities")
	TSubclassOf<class UGameplayAbility> ComboAttack3;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//移動用関数
	void Move(const FInputActionValue& Value);
	//視点移動関数
	void Look(const FInputActionValue& Value);
	//攻撃関数
	void Attack(const FInputActionValue& Value);
	//移動可能か調べる関数
	void DontMove(bool move);
	//攻撃中かどうか
	void IsAttacking(bool IsAttacking);
};
