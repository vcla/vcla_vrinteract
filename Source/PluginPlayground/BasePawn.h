// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "VRAnimInstance.h"
#include "KinectStructs.h"
#include "BasePawn.generated.h"



USTRUCT()
struct FAvatarBoneInfo
{
	GENERATED_BODY()

		UPROPERTY(EditDefaultsOnly, Category = "AvatarBoneInfo")
		FName AvatarBoneName = FName();

	//Specific to a particular skeletal mesh, must be updated
	UPROPERTY(EditDefaultsOnly, Category = "AvatarBoneInfo")
		FRotator NeutralBoneRotation = FRotator();

	UPROPERTY(EditDefaultsOnly, Category = "AvatarBoneInfo")
		TEnumAsByte<EJoint::Type> KinectJointType = EJoint::JointType_SpineBase;
};


//Base class that the various VRPawns should implement off of
//Assumption is that they'd all use the kinect for the main body, so that functionality is here
//Though in case someone comes up with an alternative for getting body info those functions are virtual
UCLASS()
class PLUGINPLAYGROUND_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

	//Avateering functions and properties
	FTransform GetConvertedTransform(FName BoneName);

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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void CalibratePawn();
	
	virtual void Grab();
	virtual void Release();

	virtual void UpdateBodyAnim();

private:
	//Distance between Kinect and user's hips when in neutral calibration pose
	FTransform KinectNeutralOffset;

	TMap<FName, FAvatarBoneInfo> BoneInfoMap;
};
