// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayAbilitySpec.h"
#include "PlayerAttack.generated.h"

//前方宣言
class PlayerManager;
class USkeletalMeshComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JHON2_API UPlayerAttack : public UActorComponent
{
	GENERATED_BODY()

	bool CanAttacking;
public:	
	// Sets default values for this component's properties
	UPlayerAttack();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Abilityシステムコンポーネントクラス変数
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;

	//攻撃開始用のアビリティタグやクラス
	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayAbility> AttackAbilityClass;

	//Abilityクラス変数
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<class UGameplayAbility> ComboAttack1;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<class UGameplayAbility> ComboAttack2;
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<class UGameplayAbility> ComboAttack3;

	//プレイヤー参照用変数
	class APlayerManager* PM;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddAttackInput();

	// 外部（キャラクターのBeginPlayやPossessedByなど）から呼び出してGASを初期化する関数
	void InitializeGAS();

	// アビリティを新しく付与する関数
	void GrantAbility(TSubclassOf<class UGameplayAbility> AbilityToGrant, int32 AbilityLevel = 1);
};
