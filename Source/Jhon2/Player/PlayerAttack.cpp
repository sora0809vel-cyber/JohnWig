// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAttack.h"
#include "Jhon2/GameplayAbility/PlayerCombo1.h"
#include "Jhon2/GameplayAbility/PlayerCombo2.h"
#include "Jhon2/GameplayAbility/PlayerCombo3.h"
#include "PlayerManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "AbilitySystemComponent.h"


// Sets default values for this component's properties
UPlayerAttack::UPlayerAttack() 
	: CanAttacking(true)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	//AbilitySystemComponent
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));

	//ABPの設定
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBPClass(TEXT("/Script/Engine.AnimBlueprint'/Game/Animation/PlayerABP.PlayerABP'_C"));

	if (AnimBPClass.Succeeded())
	{
		//アニメーションモードを「ABPを使用」に設定し、クラスを割り当てる
		PM->GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);
	}

}


// Called when the game starts
void UPlayerAttack::BeginPlay()
{
	Super::BeginPlay();

	InitializeGAS();


	//キャラクターにアビリティを付与
	if (AbilitySystemComponent) {
		//アビリティを付与
		if (ComboAttack1)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(ComboAttack1, 1, INDEX_NONE, this));
		}
		if (ComboAttack2)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(ComboAttack2, 1, INDEX_NONE, this));
		}
		if (ComboAttack3)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(ComboAttack3, 1, INDEX_NONE, this));
		}

	}

}


// Called every frame
void UPlayerAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

//プレイヤーの攻撃処理
void UPlayerAttack::AddAttackInput()
{
	if (!AbilitySystemComponent) return;

	//コンボ進行を示すタグ。ProjectSettings > GamepalyTagsに登録しておくこと
	FGameplayTag ContinueTag1 = FGameplayTag::RequestGameplayTag(FName("Combo.Continue"));
	FGameplayTag ContinueTag2 = FGameplayTag::RequestGameplayTag(FName("Combo.Continue2"));

	FGameplayTag IsAttacked = FGameplayTag::RequestGameplayTag(FName("Combo.Attack"));

	//三段目Ability起動
	if (AbilitySystemComponent->HasMatchingGameplayTag(ContinueTag2))
	{
		if (ComboAttack3)
		{
			if (CanAttacking) {
				AbilitySystemComponent->TryActivateAbilityByClass(ComboAttack3);
				AbilitySystemComponent->RemoveLooseGameplayTag(ContinueTag2);	//コンボ終了処理
			}
		}
	}
	//二段目Ability起動
	else if (AbilitySystemComponent->HasMatchingGameplayTag(ContinueTag1))
	{
		if (ComboAttack2)
		{
			if (CanAttacking) {
				AbilitySystemComponent->TryActivateAbilityByClass(ComboAttack2);
				AbilitySystemComponent->RemoveLooseGameplayTag(ContinueTag1);	//コンボ終了処理
			}
		}
	}
	//タグがなければ一段目起動
	else
	{
		if (ComboAttack1)
		{
			if (CanAttacking) {
				AbilitySystemComponent->TryActivateAbilityByClass(ComboAttack1);
			}
		}
	}

}

//外部から呼び出してGASを初期化する
void UPlayerAttack::InitializeGAS()
{
	if (AActor* OwnerActor = GetOwner())
	{
		//親クラスがIAbilitySystemInterfaceを持っているかチェックしてキャスト
		IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(OwnerActor);
		if (ASI)
		{
			UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
			if (ASC)
			{
				//GASの初期化
				ASC->InitAbilityActorInfo(OwnerActor, OwnerActor);
			}
		}
	}
}

//アビリティを新しく付与
void UPlayerAttack::GrantAbility(TSubclassOf<class UGameplayAbility> AbilityToGrant, int32 AbilityLevel)
{
	if (!AbilityToGrant) return;

	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(GetOwner());
	if (ASI)
	{
		UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
		if (ASC)
		{
			// アビリティをキャラクターに付与する
			ASC->GiveAbility(FGameplayAbilitySpec(AbilityToGrant, AbilityLevel, static_cast<int32>(AbilityToGrant.GetDefaultObject()->GetInstancingPolicy())));
		}
	}
}

