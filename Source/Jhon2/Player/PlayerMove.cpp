// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMove.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UPlayerMove::UPlayerMove()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UPlayerMove::BeginPlay()
{
	Super::BeginPlay();


}


// Called every frame
void UPlayerMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//プレイヤーの移動処理
void UPlayerMove::AddMovementInput(FVector2D MovementVector, TObjectPtr<AController> PlayerController)
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	//nullチェック
	if (!OwnerCharacter && !PlayerController) return;

	if (PlayerController)
	{
		//ベクトルの正規化
		FVector2D NomalizeMovementVector = MovementVector.GetClampedToMaxSize(1.0f);

		//コントローラーの向きから移動方向を計算
		const FRotator Rotation = PlayerController->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//前方向と右方向ベクトル
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);


		//キャラクターの関数を呼び出す
		OwnerCharacter->AddMovementInput(ForwardDirection, NomalizeMovementVector.Y);
		OwnerCharacter->AddMovementInput(RightDirection, NomalizeMovementVector.X);

	}
}

