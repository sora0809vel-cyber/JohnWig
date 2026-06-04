// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerLook.generated.h"

//クラスの前方宣言
class USpringArmComponent;
class UCameraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JHON2_API UPlayerLook : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerLook();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//カメラをプレイヤーの後ろに固定するスプリングアーム
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//キャラクターから入力値を受け取る関数
	void AddLookInput(FVector2D LookAxisVector, TObjectPtr<AController> PlayerController);
};
