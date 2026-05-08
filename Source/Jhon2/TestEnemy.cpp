// Fill out your copyright notice in the Description page of Project Settings.


#include "TestEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values
ATestEnemy::ATestEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (PlayerPawn) {
		//プレイヤー座標を取得
		FVector PlayerLocation = PlayerPawn->GetActorLocation();
		UE_LOG(LogTemp, Warning, TEXT("PlayerLocation: %s"), *PlayerLocation.ToString());
	}
}

// Called when the game starts or when spawned
void ATestEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

