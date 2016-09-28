// Fill out your copyright notice in the Description page of Project Settings.

#include "PluginPlayground.h"
#include "TestGameMode.h"

#include "KinectFunctionLibrary.h"
#include "VRPawn.h"

ATestGameMode::ATestGameMode()
{
	DefaultPawnClass = AVRPawn::StaticClass();
}

void ATestGameMode::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;

	UE_LOG(LogTemp, Warning, TEXT("Start play about to start sensor"));
	UKinectFunctionLibrary::StartSensor();

}

void ATestGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	UKinectFunctionLibrary::UpdateBody();

	//UKinectFunctionLibrary::TestKinect();
}

void ATestGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	UE_LOG(LogTemp, Warning, TEXT("End play about to stop sensor"));
	UKinectFunctionLibrary::StopSensor();
}
