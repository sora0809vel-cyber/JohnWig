// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyIdle.generated.h"

//前方宣言
class AEnemyManager;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JHON2_API UEnemyIdle : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyIdle();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	//待機関数
	void Idle(AEnemyManager* EnemyCharacter);
		
};
