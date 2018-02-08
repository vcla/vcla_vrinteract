// Fill out your copyright notice in the Description page of Project Settings.

#include "VRInteractPlatform.h"
#include "SocketInterface.h"

// This function does not need to be modified.
USocketInterface::USocketInterface(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

// Add default functionality here for any ISocketInterface functions that are not pure virtual.
void ISocketInterface::SocketInit()
{
	FIPv4Endpoint Endpoint(FIPv4Address(192, 168, 1, 195), 10020);
    ListenerSocket = FTcpSocketBuilder(TEXT("TCPLISTENER"))
        .AsReusable()
        .BoundToEndpoint(Endpoint)
        .WithReceiveBufferSize(UB_TCP_BUFFER_SIZE)
        .WithSendBufferSize(UB_TCP_BUFFER_SIZE)
        .Listening(1);
}

void ISocketInterface::SocketDestroy()
{
	// Destroy the TCPListener.
    if (TcpListener != NULL) {
        delete TcpListener;
        TcpListener = NULL;
    }
    // Release the socket.
    if (ListenerSocket != NULL) {
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenerSocket);
        ListenerSocket = NULL;
    }
    // Destroy the Receiver.
    if (Receiver != NULL) {
        delete Receiver;
        Receiver = NULL;
    }
    // Release the socket of the connection.
    if (ConnectionSocket != NULL) {
        ConnectionSocket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ConnectionSocket);
        ConnectionSocket = NULL;
    }
}
