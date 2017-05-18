// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CustomGrabInterface.h"
#include "TestForceActor.generated.h"

UCLASS()
class PLUGINPLAYGROUND_API ATestForceActor : public AActor, public ICustomGrabInterface
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditDefaultsOnly, Category = "TestForce")
		UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category = "TestForce")
		float ForceMagnitude;

	// Sets default values for this actor's properties
	ATestForceActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void OnGrab(ABasePawn* PawnRef, bool IsLeft) override;

	virtual void OnRelease(ABasePawn* PawnRef, bool IsLeft) override;
	
	ABasePawn* GrabbingPawn;
	FName HandName;
	FVector RelativeGrabPoint;
};
