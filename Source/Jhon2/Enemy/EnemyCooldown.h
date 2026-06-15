// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyCooldown.generated.h"

//前方宣言
class AEnemyManager;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JHON2_API UEnemyCooldown : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyCooldown();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//後隙状態に呼ばれる関数
	void Cooldown(AEnemyManager* EnemyCharacter);
};
