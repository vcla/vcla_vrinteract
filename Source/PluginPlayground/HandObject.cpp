// Fill out your copyright notice in the Description page of Project Settings.

#include "PluginPlayground.h"
#include "HandObject.h"
#include "LeapBone.h"


UHandObject::UHandObject()
{
	NeutralBoneOrientation = FRotator(0, 0, 0);
	//MyHandType = LeapHandType::HAND_UNKNOWN;

	Thumb = FFingerStruct();
	Index = FFingerStruct();
	Middle = FFingerStruct();
	Ring = FFingerStruct();
	Pinky = FFingerStruct();

	Arm = FBoneStruct();
	Palm = FBoneStruct();
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

	Arm.BoneOrientation = ConvertRotator(LeapHand->Arm->GetOrientation(MyHandType));
	Palm.BoneOrientation = ConvertRotator(LeapHand->PalmOrientation);

	//Fingers
	ULeapFingerList* Fingers = LeapHand->Fingers();


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
