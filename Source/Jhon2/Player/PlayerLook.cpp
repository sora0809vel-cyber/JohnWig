// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerLook.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"       // ACharacter を使うために必要
#include "GameFramework/PlayerController.h" // APlayerController を使うために必要

// Sets default values for this component's properties
UPlayerLook::UPlayerLook()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void UPlayerLook::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UPlayerLook::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

//プレイヤーから入力値を受け取る関数
void UPlayerLook::AddLookInput(FVector2D LookAxisVector, TObjectPtr<AController> PlayerController)
{
	//nullチェック
	if (!PlayerController) { return; }

	//引数をAPlayerControllerにキャスト
	if (APlayerController* PC = Cast<APlayerController>(PlayerController)) 
	{

		//コントローラーが操作しているPawnを取得
		if (APawn* ControlledPawn = PC->GetPawn())
		{
			//マウス、Padの入力値を反映
			ControlledPawn->AddControllerYawInput(LookAxisVector.X);
			ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);

		}
	}
}

