// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "KinectStructs.h"
#include "LeapController.h"
#include "LeapEnums.h"
#include "LeapEventInterface.h"
#include "VRPawn.generated.h"


USTRUCT()
struct FAvatarBoneInfo
{
	GENERATED_BODY()

		UPROPERTY(EditDefaultsOnly, Category = "AvatarBoneInfo")
		FName AvatarBoneName = FName();

	UPROPERTY(EditDefaultsOnly, Category = "AvatarBoneInfo")
		FRotator NeutralBoneRotation = FRotator();

	UPROPERTY(EditDefaultsOnly, Category = "AvatarBoneInfo")
		TEnumAsByte<EJoint::Type> KinectJointType = EJoint::JointType_SpineBase;
};

UCLASS()
class PLUGINPLAYGROUND_API AVRPawn : public APawn, public ILeapEventInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVRPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Sets Pawn's internal values based on the user's current real world position
	void CalibratePawn();

	FTransform GetConvertedRotation(FName BoneName);

	UPROPERTY(EditAnywhere, Category = "Avateering")
		TArray<FAvatarBoneInfo> BoneInfoArray;
	
	UPROPERTY(EditDefaultsOnly, Category = "VRPawn")
	USkeletalMeshComponent* BodyMesh;

	UPROPERTY(EditDefaultsOnly, Category = "VRPawn")
	USkeletalMeshComponent* HeadMesh;

	UPROPERTY(EditDefaultsOnly, Category = "VRPawn")
	USceneComponent* HeadOffset;

	UPROPERTY(EditDefaultsOnly, Category = "VRPawn")
	UCameraComponent* CameraView;

	UPROPERTY(EditDefaultsOnly, Category = "VRPawn")
	ULeapController* LeapController;

private:
	//Distance between Kinect and user's hips when in neutral calibration pose
	FTransform KinectNeutralOffset;

	TMap<FName, FAvatarBoneInfo> BoneInfoMap;
};
