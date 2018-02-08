// Fill out your copyright notice in the Description page of Project Settings.

#include "VRInteractPlatform.h"
#include "BaseRobot.h"


// Sets default values
ABaseRobot::ABaseRobot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseRobot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseRobot::Tick( float DeltaSeconds )
{
	Super::Tick( DeltaSeconds );

}

void ABaseRobot::LaunchAnimation()
{
	ParseRobotData();
}

void ABaseRobot::ParseRobotData()
{
	std::string str;
	if(!PendingData.IsEmpty())
	{
		PendingData.Dequeue(str);
		if(PendingData.IsEmpty())
			PendingData.Enqueue(str);
	}

	//parsing the current json object, override
}

void ABaseRobot::RecvRobotData(std::string RobotData)
{
	PendingData.Enqueue(RobotData);
}