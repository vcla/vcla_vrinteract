// Fill out your copyright notice in the Description page of Project Settings.

#include "VRInteractPlatform.h"
#include "HeadMountedDisplay.h"
#include "HandObject.h"
#include "LeapBone.h"


UHandObject::UHandObject()
{
	NeutralBoneOrientation = FRotator(0, 0, 0);
	HandLocation = FVector(0, 0, 0);
	//MyHandType = LeapHandType::HAND_UNKNOWN;

	Thumb = FFingerStruct();
	Index = FFingerStruct();
	Middle = FFingerStruct();
	Ring = FFingerStruct();
	Pinky = FFingerStruct();

	Arm = FBoneStruct();
	Palm = FBoneStruct();

	Confidence = 0;
	PreviousGrabStrength = 0;
	CurrentGrabStrength = 0;
}

void UHandObject::CopyHand(UHandObject* OtherHand)
{
	NeutralBoneOrientation = OtherHand->NeutralBoneOrientation;

	Thumb = OtherHand->Thumb;
	Index = OtherHand->Index;
	Middle = OtherHand->Middle;
	Ring = OtherHand->Ring;
	Pinky = OtherHand->Pinky;

	Arm = OtherHand->Arm;
	Palm = OtherHand->Palm;
}

void UHandObject::UpdateFromLeapHand(ULeapHand* LeapHand)
{
	LeapHandType MyHandType = LeapHand->HandType;
	Confidence = LeapHand->Confidence;

	PreviousGrabStrength = CurrentGrabStrength;
	CurrentGrabStrength = LeapHand->GrabStrength;

	Arm.BoneOrientation = ConvertRotator(LeapHand->Arm->GetOrientation(MyHandType));
	Palm.BoneOrientation = ConvertRotator(LeapHand->PalmOrientation);

	FRotator HeadRotation;
	FVector HeadLocation;
	
	UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(HeadRotation, HeadLocation);
	//FTransform HeadTransform = FTransform(HeadRotation, HeadLocation + HeadRotation.RotateVector(FVector(1, 0, 0)), FVector(1, 1, 1));

	//HandLocation = HeadTransform.InverseTransformVector(LeapHand->WristPosition*0.1);
	HandLocation = (LeapHand->WristPosition*0.1) + HeadLocation + HeadRotation.RotateVector(FVector(8, 0, 0)); //Hack to account for leap plugin not adding position offset for wrist

	//Fingers
	ULeapFingerList* Fingers = LeapHand->Fingers();

	GrabCenter = LeapHand->SphereCenter;


	for (int i = 0; i < Fingers->Count; i++)
	{
		ULeapFinger* Finger = Fingers->GetPointableById(i);
		switch (Finger->Type)
		{
		case FINGER_TYPE_THUMB:
			UpdateFingerFromLeap(Thumb, Finger, MyHandType);
			break;
		case FINGER_TYPE_INDEX:
			UpdateFingerFromLeap(Index, Finger, MyHandType);
			break;
		case FINGER_TYPE_MIDDLE:
			UpdateFingerFromLeap(Middle, Finger, MyHandType);
			break;
		case FINGER_TYPE_RING:
			UpdateFingerFromLeap(Ring, Finger, MyHandType);
			break;
		case FINGER_TYPE_PINKY:
			UpdateFingerFromLeap(Pinky, Finger, MyHandType);
			break;
		default:
			break;
		}
	}
}

void UHandObject::UpdateFingerFromLeap(FFingerStruct& FingerStruct, ULeapFinger* LeapFinger, LeapHandType HandType)
{
	FingerStruct.Proximal.BoneOrientation = ConvertRotator(LeapFinger->Proximal->GetOrientation(HandType));
	FingerStruct.Intermediate.BoneOrientation = ConvertRotator(LeapFinger->Intermediate->GetOrientation(HandType));
	FingerStruct.Distal.BoneOrientation = ConvertRotator(LeapFinger->Distal->GetOrientation(HandType));
}

FRotator UHandObject::ConvertRotator(FRotator RotatorToConvert)
{
	return FRotator(RotatorToConvert.Quaternion()*NeutralBoneOrientation.Quaternion());
}
