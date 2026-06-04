// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerAttack.h"
#include "PlayerLook.h"
#include "PlayerMove.h"
#include <Kismet/GameplayStatics.h>
#include "Components/SkeletalMeshComponent.h"
#include "AbilitySystemComponent.h"

// Sets default values
APlayerManager::APlayerManager()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//参照用変数のコンポーネント生成
	PLook = CreateDefaultSubobject<UPlayerLook>(TEXT("LookComponent"));
	PMove = CreateDefaultSubobject<UPlayerMove>(TEXT("MoveComponent"));
	PAttack = CreateDefaultSubobject<UPlayerAttack>(TEXT("AttackComponent"));

	// コンストラクタでASCを生成してアタッチ
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

//IAbilitySystemInterfaceの必須関数
UAbilitySystemComponent* APlayerManager::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void APlayerManager::BeginPlay()
{
	Super::BeginPlay();

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
void APlayerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//アクションのバインド
	if (UEnhancedInputComponent* EnhancedInputComponent =
		CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerManager::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerManager::Look);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APlayerManager::Attack);
	}
}

//プレイヤーの移動モジュールへ入力値を渡す
void APlayerManager::Move(const FInputActionValue& Value)
{
	//入力値の代入
	MovementVector = Value.Get<FVector2D>();

	if (Controller) 
	{
		//MoveモジュールへX,Y軸の入力とキャラクターのコントローラーを渡す
		PMove->AddMovementInput(MovementVector, Controller);
	}
}

//プレイヤーの視点移動モジュールへ入力値を渡す
void APlayerManager::Look(const FInputActionValue& Value)
{
	//マウス、Padの入力値を取得
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	//コントローラーがあるなら通す
	if (Controller) 
	{
		//LookモジュールへX,Y軸の入力とキャラクターのコントローラーを渡す
		PLook->AddLookInput(LookAxisVector, Controller);
	}
}

//プレイヤーの攻撃モジュールへ入力値を渡す
void APlayerManager::Attack()
{
	if (Controller) {
		//Attackモジュールの攻撃関数を呼び出す
		PAttack->AddAttackInput();
	}
}
