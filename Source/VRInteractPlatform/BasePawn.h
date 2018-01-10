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
class VRINTERACTPLATFORM_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

	//Avateering functions and properties

	//Gets the orientation of a bone from the kinect adjusted based on the skeletal mesh's bone orientations
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

	UPROPERTY(EditDefaultsOnly, Category = "VRPawn")
		float MovementSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "VRPawn")
		float TurnSpeed;

	//Name of the joint where objects should be attached
	UPROPERTY(EditDefaultsOnly, Category = "VRPawn")
		FName LeftHandAttachPoint;

	UPROPERTY(EditDefaultsOnly, Category = "VRPawn")
		FName RightHandAttachPoint;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void BeginDestroy() override;

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void CalibratePawn();
	
	void Grab(bool IsLeft);
	void Release(bool IsLeft);

	//Update the body's animinstance, assumed to be VRAnimInstance
	virtual void UpdateAnim();

	virtual void UpdateMoveAnim();
	virtual void UpdateBodyAnim();

	//Pawn movement
	FVector MovementInput;

	void ProcessForward(float AxisValue);
	void ProcessRight(float AxisValue);
	void ProcessRotate(float AxisValue);

protected:
	float UpdateInterval;

	TArray<UPrimitiveComponent*> LeftHandGrabbedComponents;
	TArray<UPrimitiveComponent*> RightHandGrabbedComponents;

	//AActors since direct array of interfaces is a pain. should never be not interfaces, but check for it anyways
	TArray<AActor*> LeftHandCustomGrab; 
	TArray<AActor*> RightHandCustomGrab;

private:
	//Distance between Kinect and user's hips when in neutral calibration pose
	FTransform KinectNeutralOffset;

	//Default is attaching components instead of the whole actor to deal with actors w/ more than one mesh to grab
	TMap<FName, FAvatarBoneInfo> BoneInfoMap;
	
};
