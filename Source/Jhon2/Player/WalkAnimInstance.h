// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WalkAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class JHON2_API UWalkAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

	//補正の値
	float CorrectionVector;

public:

	UWalkAnimInstance();

	//アニメーション更新処理をオーバーライド
	virtual void NativeUpdateAnimation(float Seconds) override;

	


protected:
	//スティックの入力値
	UPROPERTY(BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float WalkAnimationSpeed;

	//スティックの入力値
	UPROPERTY(BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float DashAnimationSpeed;

private:

	//コンポーネントの情報を取得する関数
	bool GetCharacterMovementComponent();

	//stickの値を取得する関数
	void GetStickInput();

	//stickの値を受け取り補正をかける関数
	void CorrectStickInput();

	//補正した値を代入する関数
	void AssignmentMagnitude();

	UPROPERTY()
	UCharacterMovementComponent* MoveComponent;

	UPROPERTY()
	FVector StickVector;

	UPROPERTY()
	float StickMagnitude;


};
