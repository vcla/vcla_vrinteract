// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SocketInterface.h"
#include "BaseRobot.h"

#include <string>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

#include "CentralCommunicator.generated.h"

UCLASS()
class VRINTERACTPLATFORM_API ACentralCommunicator : public AActor, public ISocketInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACentralCommunicator();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Handle the TCP connection with provided socket interface
    bool OnTcpListenerConnectionAccepted(FSocket* Socket, const FIPv4Endpoint& Endpoint);

    int32 OnTcpReceived(const uint8* Data, int32 BytesReceived);
	
    void BuildListenerConnection();

private:

	TMap<FString, ABaseRobot*> Name2RobotMap;
};
