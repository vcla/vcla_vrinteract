// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "VRAnimInstance.h"

#include <string>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "AnimateActor.generated.h"

using namespace rapidjson;

UCLASS()
class VRINTERACTPLATFORM_API AAnimateActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAnimateActor();

	UPROPERTY(EditDefaultsOnly, Category = "VRActor")
		USkeletalMeshComponent* BodyMesh;

	UPROPERTY(EditDefaultsOnly, Category = "VRActor")
		USkeletalMeshComponent* HeadMesh;

	UPROPERTY(EditDefaultsOnly, Category = "VRActor")
		USceneComponent* HeadOffset;

	UPROPERTY(EditDefaultsOnly, Category = "VRActor")
		UCameraComponent* CameraView;

	//Name of the joint where objects should be attached
	UPROPERTY(EditDefaultsOnly, Category = "VRActor")
		FName LeftHandAttachPoint;

	UPROPERTY(EditDefaultsOnly, Category = "VRActor")
		FName RightHandAttachPoint;

	UFUNCTION(BlueprintCallable, Category = "VRActor")
		void RecordTriggerFunc();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void BeginDestroy() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void Grab(bool IsLeft);
	void Release(bool IsLeft);
	
	void UpdateAnim();
	void UpdateActor(Document &doc);
	void UpdateBody(Document &doc);
	void UpdateHand(Document &doc);

private:

	float UpdateInterval;

	uint64 RecordApplied;
	TArray<FString> ApplyPoseArray;

	bool RecordTrigger;
	FString RecordSeg;

	UVRAnimInstance* AnimInstance;

	TArray<UPrimitiveComponent*> LeftHandGrabbedComponents;
	TArray<UPrimitiveComponent*> RightHandGrabbedComponents;

	TArray<AActor*> LeftHandCustomGrab; 
	TArray<AActor*> RightHandCustomGrab;
};