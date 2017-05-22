// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
//#include "KinectStructs.h"
//#include "VRPawn.h"
#include "HandObject.h"
#include "VRAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class VRINTERACTPLATFORM_API UVRAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		float TrackLeft;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		float TrackRight;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FVector LeftHandLocation;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FVector RightHandLocation;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FVector LeftElbowLocation;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FVector RightElbowLocation;

#pragma region bonerotations
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator SpineBase;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator SpineMid;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator SpineTop;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator ShoulderLeft;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator ElbowLeft;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator WristLeft;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator HipLeft;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator KneeLeft;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator FootLeft;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator ShoulderRight;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator ElbowRight;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator WristRight;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator HipRight;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator KneeRight;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator FootRight;
#pragma region leftfingers
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator ThumbProxLeft;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator ThumbInterLeft;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator ThumbDistLeft;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator IndexProxLeft;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator IndexInterLeft;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator IndexDistLeft;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator MiddleProxLeft;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator MiddleInterLeft;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator MiddleDistLeft;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator RingProxLeft;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator RingInterLeft;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator RingDistLeft;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator PinkyProxLeft;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator PinkyInterLeft;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator PinkyDistLeft;
#pragma endregion
#pragma region rightfingers
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator ThumbProxRight;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator ThumbInterRight;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator ThumbDistRight;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator IndexProxRight;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator IndexInterRight;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator IndexDistRight;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator MiddleProxRight;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator MiddleInterRight;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator MiddleDistRight;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator RingProxRight;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator RingInterRight;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator RingDistRight;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator PinkyProxRight;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator PinkyInterRight;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator PinkyDistRight;
#pragma endregion
#pragma endregion
};
