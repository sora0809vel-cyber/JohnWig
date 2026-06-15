// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugParameterManager.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"

TArray<FString> UDebugParameterManager::GetVariableOptions() const
{
	TArray<FString> Options;

	for (const FParameterSelector& ParamSlot : ParameterList)
	{
		if (ParamSlot.TargetClass)
		{
			//クラスの持つメンバ変数を走査
			for (TFieldIterator<FProperty> PropIt(ParamSlot.TargetClass); PropIt; ++PropIt)
			{
				FProperty* Property = *PropIt;
				if (Property)
				{
					Options.AddUnique(Property->GetName());
				}
			}
		}
	}

	//何も選ばれていなければ警告文を出す
	if (Options.Num() == 0)
	{
		Options.Add(TEXT("Please select TargetClass first"));
	}

	return Options;
}

bool UDebugParameterManager::Initialize()
{
	if (!Super::Initialize()) 
	{
		return false;
	}
	

	return true;
}

void UDebugParameterManager::OnSliderValueChanged(float NewValue)
{
}
