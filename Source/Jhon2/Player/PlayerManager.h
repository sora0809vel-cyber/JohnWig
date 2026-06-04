// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"

#include "PlayerManager.generated.h"

//前方宣言
class UInputMappingContext;
class UInputAction;
class USkeletalMeshComponent;
class UAbilitySystemComponent;

class APlayerMove;
class APlayerLook;
class APlayerAttack;
UCLASS()
class JHON2_API APlayerManager : public ACharacter, public IAbilitySystemInterface

{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerManager();

	//追加したAbilityシステムのコンポネント取得
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

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
	
	//プレイヤーの移動量を格納する変数
	FVector2D MovementVector;

	//Abilityシステムコンポーネントクラス変数
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Attack();

	//プレイヤーのメッシュ取得関数
	void GetPlayerMesh(USkeletalMeshComponent*& PlayerMesh);

	//別クラス参照用変数
	class UPlayerMove* PMove;
	class UPlayerLook* PLook;
	class UPlayerAttack* PAttack;

};
