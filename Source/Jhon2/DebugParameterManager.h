// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/NoExportTypes.h"
#include "DebugParameterManager.generated.h"

//前方宣言
class USlider;
class UTextBlock;

/**
 * 
 */

//配列の中身になる構造体
USTRUCT(BlueprintType)
struct FParameterSelector
{
	GENERATED_BODY()

	//クラス選択をするプルダウン
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Parameter")
	TSubclassOf<UObject> TargetClass;

	//選択したクラスの変数を一覧表示するプルダウン
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta = (GetOptions = "GetVariableOptions"),Category = "Parameter")
	FString TargetVariable;

};

UCLASS(Blueprintable,BlueprintType)
class JHON2_API UDebugParameterManager : public UUserWidget 
{
	GENERATED_BODY()

public:
	//
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Parameter")
	TArray<FParameterSelector> ParameterList;

	//プルダウンの中身を自動的に生成する関数
	UFUNCTION()
	TArray<FString> GetVariableOptions() const;
protected:
	//画面の初期化時に呼び出される
	virtual bool Initialize() override;


private:
	//スライダーが動いた時に呼び出すカスタム関数
	UFUNCTION()
	void OnSliderValueChanged(float NewValue);
};