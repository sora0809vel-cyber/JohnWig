// Fill out your copyright notice in the Description page of Project Settings.

#include "Player_2.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Jhon2/GameplayAbility/GA_SlashAttack.h"
#include "Jhon2/GameplayAbility/GA_DodgePlayer.h"
#include "Jhon2/GameplayAbility/GA_Iaido.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"

// Sets default values
APlayer_2::APlayer_2() :
	CanMove(true),
	CanAttacking(false),
	Dodging(false),
	DodgeIntervalTime(0.016f),
	DodgeEndTime(0.2f),
	moving(false)
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
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 2000.0f, 0.0f);



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

UAbilitySystemComponent* APlayer_2::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void APlayer_2::BeginPlay()
{
	Super::BeginPlay();

	//プレイヤーの速度を初期化
	GetCharacterMovement()->MaxWalkSpeed = PrayerMoveSpeed;

	//キャラクターにアビリティを付与
	if (AbilitySystemComponent) {
		////アビリティを付与
		if (AttackSlash1) {
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(AttackSlash1, 1, INDEX_NONE, this));
		}
		if (AttackSlash2) {
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(AttackSlash2, 1, INDEX_NONE, this));
		}
		if (AttackSlash3) {
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(AttackSlash3, 1, INDEX_NONE, this));
		}
		if (DodgeAbility) 
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(DodgeAbility, 1, INDEX_NONE, this));
		}

		if(IaidoStanceAbility)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(IaidoStanceAbility, 1, INDEX_NONE, this));
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
void APlayer_2::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	//今の状態を調べるデバッグ
	//if (CurrentPlayerState == PlayerState::Stance)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Stance"));
	//}
	//if (CurrentPlayerState == PlayerState::Idle)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Idle"));
	//}
	//if (CurrentPlayerState == PlayerState::Dodge)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Dodge"));
	//}
	//if (CurrentPlayerState == PlayerState::Slash)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Slash"));
	//}
	//if (CurrentPlayerState == PlayerState::Move)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Move"));
	//}


}

// Called to bind functionality to input
void APlayer_2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//アクションのバインド
	if (UEnhancedInputComponent* EnhancedInputComponent =
		CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		//IAで設定されたキーが押されたらMove()呼び出し
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayer_2::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APlayer_2::MoveEnd);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayer_2::Look);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APlayer_2::Attack);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &APlayer_2::IsDodge);
		EnhancedInputComponent->BindAction(IaidoAction, ETriggerEvent::Started, this, &APlayer_2::IaidoStanceChange);
		//EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Completed, this, &APlayer_2::cancelStance);

	}
}



//プレイヤーの移動関数
void APlayer_2::Move(const FInputActionValue& Value) {
	

	//if (CurrentPlayerState != PlayerState::Idle || CurrentPlayerState == PlayerState::Slash)return;

	//回避する方向を正面に設定
	DodgeDirection = GetActorForwardVector();
	//上下に動かないように
	DodgeDirection.Z = 0.f;
	//回避する方向を正規化
	DodgeDirection.Normalize();

	//FGameplayTag DodgeTag = FGameplayTag::RequestGameplayTag(FName("Ability.Dodge"));


	//攻撃中、歩けない
	//if (!CanMove) return;

	if (Controller != nullptr) {
		//コントローラーの向きから移動方向を計算
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//前方向ベクトル
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		//右方向ベクトル
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);


		//Vector2Dとして入力値を取得
		MovementVector = Value.Get<FVector2D>();

		//納刀中は向きのみ変更
		if (CurrentPlayerState == Player2State::Stance)
		{
			FVector RotattionOnlyVector = (ForwardDirection * MovementVector.Y) + (RightDirection * MovementVector.X);
			RotattionOnlyVector.Z = 0.f;

			FRotator StanceRotation = RotattionOnlyVector.Rotation();

			SetActorRotation(StanceRotation);
		}

		else if(CurrentPlayerState == Player2State::Idle || CurrentPlayerState == Player2State::Move)
		{
			CurrentPlayerState = Player2State::Move;

			//移動入力を追加
			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);



			//moving = tr
		}
	}
}

void APlayer_2::MoveEnd(const FInputActionValue& Value)
{
	if (CurrentPlayerState != Player2State::Move)
	{

		return;
	}

	else if (CurrentPlayerState == Player2State::Move)
	{

		CurrentPlayerState = Player2State::Idle;
	}

	
}


//プレイヤーの視点移動関数
void APlayer_2::Look(const FInputActionValue& Value) {
	//マウス、Padの入力取得
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	//プレイヤーのコントローラー取得したら
	if (GetController() != nullptr) {
		//マウス、Padの上下左右の入力
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);

	}
}

//プレイヤー攻撃関数(いずれまるごとアビリティに移動予定)
void APlayer_2::Attack(const FInputActionValue& Value) {


	//アビリティシステムコンポーネントが存在せず、プレイヤーのステータスが構えでなかったら発動しない
	if (!AbilitySystemComponent || (CurrentPlayerState != Player2State::Stance && CurrentPlayerState != Player2State::Slash && CurrentPlayerState != Player2State::Dodge)) return;


	//三段攻撃進行タグを登録
	FGameplayTag NextSlashTag1 = FGameplayTag::RequestGameplayTag(FName("IaiCombo.NextSlash1"));
	FGameplayTag NextSlashTag2 = FGameplayTag::RequestGameplayTag(FName("IaiCombo.NextSlash2"));
	FGameplayTag SlashEndTag = FGameplayTag::RequestGameplayTag(FName("IaiCombo.ComboEnd"));



	//現在のコンボ進行タグが終わりなら
	if (AbilitySystemComponent->HasMatchingGameplayTag(SlashEndTag))
	{
		UE_LOG(LogTemp, Warning, TEXT("000"));

		
		//何も再生しない
		return;
	}

	//現在のコンボ進行タグが2なら
	else if (AbilitySystemComponent->HasMatchingGameplayTag(NextSlashTag2))
	{
		UE_LOG(LogTemp, Warning, TEXT("333"));


		//攻撃3段階目を実行
		AbilitySystemComponent->TryActivateAbilityByClass(AttackSlash3);
		//タグを消去
		AbilitySystemComponent->RemoveLooseGameplayTag(NextSlashTag2);
	}

	//現在のコンボ進行タグが1なら
	else if (AbilitySystemComponent->HasMatchingGameplayTag(NextSlashTag1))
	{
		UE_LOG(LogTemp, Warning, TEXT("222"));


		//攻撃2段階目を実行
		AbilitySystemComponent->TryActivateAbilityByClass(AttackSlash2);
		//タグを消去
		AbilitySystemComponent->RemoveLooseGameplayTag(NextSlashTag1);
	}

	//コンボ進行タグがどれでもなく、かつ構え中の場合(最初の入力だけ飯能するようにするため)
	else if(CurrentPlayerState == Player2State::Stance)
	{

		UE_LOG(LogTemp, Warning, TEXT("111"));

		//攻撃1段階目を実行
		AbilitySystemComponent->TryActivateAbilityByClass(AttackSlash1);
	}

	StanseStart();


	//ステータスを攻撃に
	CurrentPlayerState = Player2State::Slash;

}

//回避
void APlayer_2::IsDodge(const FInputActionValue& Value)
{
	//float NextTimer = 1.f;

	////現在の経過時間を取得
	//float CurrentTime = GetWorld()->GetTimeSeconds();


	//if (CurrentTime < NextTimer) 
	//{
	//	return;
	//}

	//現在のステートがスラッシュならreturn
	if ( CurrentPlayerState == Player2State::Slash ) return;
	
	//ステート構えでないなら回避にする
	if (CurrentPlayerState != Player2State::Stance)
	{
		//現在のstatusを通常状態へ
		CurrentPlayerState = Player2State::Dodge;

	}
	

	//回避のアビリティを呼び出す
	AbilitySystemComponent->TryActivateAbilityByClass(DodgeAbility);

	 
}

//ボタンを押している間の納刀状態の関数
void APlayer_2::IaidoStanceChange(const FInputActionValue& Value)
{


	//statusが回避状態で、攻撃中でないなら
	if (CurrentPlayerState != Player2State::Slash && CurrentPlayerState != Player2State::Dodge)
	{

		//タグ作成
		FGameplayTag StanceFreezeTag = FGameplayTag::RequestGameplayTag(FName("StanceAbility.Freeze"));

		//タグを持っていたら
		if (AbilitySystemComponent->HasMatchingGameplayTag(StanceFreezeTag))
		{

			//よくわからないデータ
			FGameplayEventData Payload;
			Payload.Instigator = this;

			//状態をアイドルに
			CurrentPlayerState = Player2State::Idle;


			//アビりティ再開のタグ
			FGameplayTag StanceStartTag = FGameplayTag::RequestGameplayTag(FName("StanceAbility.Normal"));

			//アビリティにイベントの通知を送る
			AbilitySystemComponent->HandleGameplayEvent(StanceStartTag, &Payload);

		}

		//statusを納刀にする
		//CurrentPlayerState = PlayerState::Stance;

		//アビリティシステムコンポーネントと、納刀の構えのアビリティがあれば
		else if(!AbilitySystemComponent->HasMatchingGameplayTag(StanceFreezeTag))
		{
			//アビリティを呼び出す
			AbilitySystemComponent->TryActivateAbilityByClass(IaidoStanceAbility);

			//状態をアイドルに
			CurrentPlayerState = Player2State::Stance;


		}
	}
}





//腑に落ちていないので後から何とかします
bool APlayer_2::PlayerInput()
{
	//if (APlayerController* PC = Cast<APlayerController>(GetController()))
	//{
	//	if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PC->InputComponent))
	//	{
	//		if (MoveAction)
	//		{
	//			auto RawValue = EnhancedInputComp->GetBoundActionValue(MoveAction);
	//			FVector2D RawVector = RawValue.Get<FVector2D>();

	//			return !RawVector.IsNearlyZero(0.1f);
	//		}
	//	}


		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			bool bW = PC->IsInputKeyDown(EKeys::W);
			bool bA = PC->IsInputKeyDown(EKeys::A);
			bool bS = PC->IsInputKeyDown(EKeys::S);
			bool bD = PC->IsInputKeyDown(EKeys::D);

			bool bKeyboardActive = (bW || bA || bS || bD);

			float GamepadX = 0.f;
			float GamepadY = 0.f;

			if (PC->PlayerInput)
			{
				GamepadX = PC->PlayerInput->GetKeyValue(EKeys::Gamepad_LeftX);
				GamepadY = PC->PlayerInput->GetKeyValue(EKeys::Gamepad_LeftY);
			}

			// 3. スティックに「遊び（デッドゾーン）」を作って判定
			FVector2D StickVector(GamepadX, GamepadY);
			bool bStickActive = !StickVector.IsNearlyZero(0.2f);
			// 4. キーボード、またはスティックのどちらかが動いていれば true を返す
			return (bKeyboardActive || bStickActive);
		}
	
	return false;

	//if (CurrentPlayerState == PlayerState::Move)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Move"));

	//	return true;
	//}
	//UE_LOG(LogTemp, Warning, TEXT("NotMove"));


	//return false;

}


void APlayer_2::StanseStart()
{
	//タグ作成
	FGameplayTag StanceFreezeTag = FGameplayTag::RequestGameplayTag(FName("StanceAbility.Freeze"));

	//タグを持っていたら
	if (AbilitySystemComponent->HasMatchingGameplayTag(StanceFreezeTag))
	{
		//よくわからないデータ
		FGameplayEventData Payload;
		Payload.Instigator = this;

		//状態をアイドルに
		//CurrentPlayerState = PlayerState::Idle;


		//アビりティ再開のタグ
		FGameplayTag StanceStartTag = FGameplayTag::RequestGameplayTag(FName("StanceAbility.Normal"));

		//アビリティにイベントの通知を送る
		AbilitySystemComponent->HandleGameplayEvent(StanceStartTag, &Payload);

	}

}

void APlayer_2::SetStateStance()
{
	CurrentPlayerState = Player2State::Stance;
}


//ステータスをアイドルにする(攻撃アニメーション終了時に呼び出される(GA_SlashAttack.cpp))
void APlayer_2::SetStateIdle()
{
	CurrentPlayerState = Player2State::Idle;
}


////ステータスを変える関数
//void APlayer_2::ChangeState(PlayerState State)
//{
//	CurrentPlayerState = State;
//}


//移動可能かどうか
void APlayer_2::DontMove(bool move)
{
	CanMove = move;
}


void APlayer_2::IsAttacking(bool IsAttacking)
{


	CanAttacking = IsAttacking;
}

void APlayer_2::IsDodge(bool dodge)
{
	Dodging = dodge;
}



