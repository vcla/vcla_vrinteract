// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "KinectStructs.h"
#include "VRPawn.h"
#include "HandObject.h"
#include "KinectAnimInstance.generated.h"



/**
 * 
 */
UCLASS()
class PLUGINPLAYGROUND_API UKinectAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Kinect)
	//TMap<TEnumAsByte<EJoint::Type>, FTransform> BoneTransformMap;
	//UKinectAnimInstance(const FObjectInitializer& ObjectInitializer);

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

	AVRPawn* OwningPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinect")
		FRotator HipsSkeletalComponentOffset;


	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kinect")
		FTransform GetJointOrientation(EJoint::Type JointType);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Avateering")
		FTransform GetConvertedRotation(FName BoneName);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avateering")
		UHandObject* LeftHandAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avateering")
		UHandObject* RightHandAnim;
};
