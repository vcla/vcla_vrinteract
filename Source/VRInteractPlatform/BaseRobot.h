// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include <string>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "BaseRobot.generated.h"

using namespace rapidjson;

UCLASS()
class VRINTERACTPLATFORM_API ABaseRobot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseRobot();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void LaunchAnimation();

	virtual void ParseRobotData();

	virtual void RecvRobotData(std::string RobotData);

protected:

	TQueue<std::string> PendingData;
};
