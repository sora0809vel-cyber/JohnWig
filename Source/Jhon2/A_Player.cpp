// Fill out your copyright notice in the Description page of Project Settings.


#include "A_Player.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GA_ComboAttack1.h"
#include "GA_ComboAttack2.h"
#include "GA_ComboAttack3.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AA_Player::AA_Player() :
	CanMove(true),
	CanAttacking(true)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//プレイヤーのカメラの回転を軸ごとに許可するかどうか
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//SpringArmの設定
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;		//カメラのアームを回す
	CameraBoom->TargetArmLength = 400.0f;			//プレイヤーからのカメラの距離

	//キャラクターの移動の方向に応じてキャラクターの向きを変える
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	//Meshコンポーネントに対してABPを適用する
	//ABPの検索
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBPClass(TEXT("/Script/Engine.AnimBlueprint'/Game/Animation/ABP_PlayerMove.ABP_PlayerMove'_C"));
	if (AnimBPClass.Succeeded())
	{
		//アニメーションモードを「ABPを使用」に設定し、クラスを割り当てる
		GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);
	}

	//AbilitySystemComponent
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));

}

UAbilitySystemComponent* AA_Player::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void AA_Player::BeginPlay()
{
	Super::BeginPlay();

	//キャラクターにアビリティを付与
	if (AbilitySystemComponent) {
		//アビリティを付与
		if (ComboAttack1) {
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(ComboAttack1, 1, INDEX_NONE, this));
		}
		if (ComboAttack2) {
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(ComboAttack2, 1, INDEX_NONE, this));
		}
		if (ComboAttack3) {
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(ComboAttack3, 1, INDEX_NONE, this));
		}

	}

	//MappingContextの登録
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

// Called every frame
void AA_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AA_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//アクションのバインド
	if (UEnhancedInputComponent* EnhancedInputComponent =
		CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		//IAで設定されたキーが押されたらMove()呼び出し
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AA_Player::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AA_Player::Look);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AA_Player::Attack);
	}
}



//プレイヤーの移動関数
void AA_Player::Move(const FInputActionValue& Value) {
	//Vector2Dとして入力値を取得
	FVector2D MovementVector = Value.Get<FVector2D>();

	//攻撃中は歩けない
	if (!CanMove) return;

	if (Controller != nullptr) {

		//コントローラーの向きから移動方向を計算
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//前方向ベクトル
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//右方向ベクトル
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		//移動入力を追加
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

//プレイヤーの視点移動関数
void AA_Player::Look(const FInputActionValue& Value) {
	//マウス、Padの入力取得
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	//プレイヤーのコントローラー取得したら
	if (GetController() != nullptr) {
		//マウス、Padの上下左右の入力
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);

	}
}

//プレイヤー攻撃関数
void AA_Player::Attack(const FInputActionValue& Value) {
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

//移動可能かどうか
void AA_Player::DontMove(bool move)
{
	CanMove = move;
}

void AA_Player::IsAttacking(bool IsAttacking)
{
	CanAttacking = IsAttacking;
}


