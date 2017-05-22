// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "LeapEnums.h"
#include "LeapHand.h"
#include "HandObject.generated.h"

USTRUCT(BlueprintType)
struct VRINTERACTPLATFORM_API FBoneStruct
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avateering")
		FRotator BoneOrientation;


	FBoneStruct()
	{
		BoneOrientation = FRotator(5, 0, 0);
	}
};

USTRUCT(BlueprintType)
struct VRINTERACTPLATFORM_API FFingerStruct
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avateering")
	FBoneStruct Proximal;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avateering")
	FBoneStruct Intermediate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avateering")
	FBoneStruct Distal;

	void UpdateBoneOrientation(FRotator NewOrientation, bool UpdateChildren = true)
	{
		//Proximal.NeutralBoneOrientation = NewOrientation;
	}

	FFingerStruct()
	{
		Proximal = FBoneStruct();
	}
};

/**
 * 
 */
UCLASS(Blueprintable)
class VRINTERACTPLATFORM_API UHandObject : public UObject
{
	GENERATED_BODY()
	
public:
	UHandObject();



	/**The additional rotation required by the avatar in order for the bone to be in a neutral pose */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Avateering")
		FRotator NeutralBoneOrientation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avateering")
	FFingerStruct Thumb;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avateering")
	FFingerStruct Index;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avateering")
	FFingerStruct Middle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avateering")
	FFingerStruct Ring;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avateering")
	FFingerStruct Pinky;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avateering")
	FBoneStruct Arm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avateering")
	FBoneStruct Palm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avateering")
		FVector HandLocation;

	UFUNCTION(BlueprintCallable, Category = "Avateering")
		void UpdateFromLeapHand(ULeapHand* Hand);
	
	void CopyHand(UHandObject* OtherHand);

	void UpdateFingerFromLeap(FFingerStruct& FingerStruct, ULeapFinger* LeapFinger, LeapHandType HandType);

	FRotator ConvertRotator(FRotator OtherRotator);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Avateering")
	float Confidence;

	bool IsLeft;

	float PreviousGrabStrength;
	float CurrentGrabStrength;

	FVector GrabCenter;

};
