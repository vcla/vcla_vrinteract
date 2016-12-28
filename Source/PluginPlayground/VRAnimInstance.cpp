// Fill out your copyright notice in the Description page of Project Settings.

#include "PluginPlayground.h"
#include "KinectFunctionLibrary.h"
#include "VRAnimInstance.h"

void UVRAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningPawn = Cast<AVRPawn>(TryGetPawnOwner());
}

void UVRAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	Super::NativeUpdateAnimation(DeltaTimeX);

	if (OwningPawn)
	{
		//Set most of main body from Kinect
		SpineBase = OwningPawn->GetConvertedRotation(FName("pelvis")).Rotator();// UKinectFunctionLibrary::GetWorldJointTransform(EJoint::JointType_SpineBase).Rotator();
		SpineMid = OwningPawn->GetConvertedRotation(FName("spine_02")).Rotator();//UKinectFunctionLibrary::GetWorldJointTransform(EJoint::JointType_SpineMid).Rotator();
		SpineTop = OwningPawn->GetConvertedRotation(FName("spine_03")).Rotator();//UKinectFunctionLibrary::GetWorldJointTransform(EJoint::JointType_SpineShoulder).Rotator();
		ShoulderLeft = OwningPawn->GetConvertedRotation(FName("upperarm_l")).Rotator();//UKinectFunctionLibrary::GetWorldJointTransform(EJoint::JointType_ShoulderLeft).Rotator();
		ShoulderRight = OwningPawn->GetConvertedRotation(FName("upperarm_r")).Rotator();
		HipLeft = OwningPawn->GetConvertedRotation(FName("thigh_l")).Rotator();
		KneeLeft = OwningPawn->GetConvertedRotation(FName("calf_l")).Rotator();
		FootLeft = OwningPawn->GetConvertedRotation(FName("foot_l")).Rotator();
		HipRight = OwningPawn->GetConvertedRotation(FName("thigh_r")).Rotator();
		KneeRight = OwningPawn->GetConvertedRotation(FName("calf_r")).Rotator();
		FootRight = OwningPawn->GetConvertedRotation(FName("foot_r")).Rotator();
		//Elbow and wrist set from Leap or Kinect depending on tracking status
		if (OwningPawn->LeftHand->Confidence > 0.1)
		{
			TrackLeft = 1.0f;

			LeftHandLocation = OwningPawn->LeftHand->HandLocation + OwningPawn->HeadOffset->RelativeLocation;

			ElbowLeft = OwningPawn->LeftHand->Arm.BoneOrientation;
			WristLeft = OwningPawn->LeftHand->Palm.BoneOrientation;

			//Set fingers from Leap only
			ThumbProxLeft = OwningPawn->LeftHand->Thumb.Proximal.BoneOrientation;
			ThumbInterLeft = OwningPawn->LeftHand->Thumb.Intermediate.BoneOrientation;
			ThumbDistLeft = OwningPawn->LeftHand->Thumb.Distal.BoneOrientation;
			IndexProxLeft = OwningPawn->LeftHand->Index.Proximal.BoneOrientation;
			IndexInterLeft = OwningPawn->LeftHand->Index.Intermediate.BoneOrientation;
			IndexDistLeft = OwningPawn->LeftHand->Index.Distal.BoneOrientation;
			MiddleProxLeft = OwningPawn->LeftHand->Middle.Proximal.BoneOrientation;
			MiddleInterLeft = OwningPawn->LeftHand->Middle.Intermediate.BoneOrientation;
			MiddleDistLeft = OwningPawn->LeftHand->Middle.Distal.BoneOrientation;
			RingProxLeft = OwningPawn->LeftHand->Ring.Proximal.BoneOrientation;
			RingInterLeft = OwningPawn->LeftHand->Ring.Intermediate.BoneOrientation;
			RingDistLeft = OwningPawn->LeftHand->Ring.Distal.BoneOrientation;
			PinkyProxLeft = OwningPawn->LeftHand->Pinky.Proximal.BoneOrientation;
			PinkyInterLeft = OwningPawn->LeftHand->Pinky.Intermediate.BoneOrientation;
			PinkyDistLeft = OwningPawn->LeftHand->Pinky.Distal.BoneOrientation;
		}
		else
		{
			TrackLeft = 0;

			ElbowLeft = OwningPawn->GetConvertedRotation(FName("lowerarm_l")).Rotator();//UKinectFunctionLibrary::GetWorldJointTransform(EJoint::JointType_WristRight).Rotator();
			WristLeft = OwningPawn->GetConvertedRotation(FName("hand_l")).Rotator();//UKinectFunctionLibrary::GetWorldJointTransform(EJoint::JointType_HandRight).Rotator();
		}

		if (OwningPawn->RightHand->Confidence > 0.1)
		{
			TrackRight = 1.0f;

			RightHandLocation = OwningPawn->RightHand->HandLocation + OwningPawn->HeadOffset->RelativeLocation;

			ElbowRight = OwningPawn->RightHand->Arm.BoneOrientation;
			WristRight = OwningPawn->RightHand->Palm.BoneOrientation;

			//Set fingers from Leap only
			ThumbProxRight = OwningPawn->RightHand->Thumb.Proximal.BoneOrientation;
			ThumbInterRight = OwningPawn->RightHand->Thumb.Intermediate.BoneOrientation;
			ThumbDistRight = OwningPawn->RightHand->Thumb.Distal.BoneOrientation;
			IndexProxRight = OwningPawn->RightHand->Index.Proximal.BoneOrientation;
			IndexInterRight = OwningPawn->RightHand->Index.Intermediate.BoneOrientation;
			IndexDistRight = OwningPawn->RightHand->Index.Distal.BoneOrientation;
			MiddleProxRight = OwningPawn->RightHand->Middle.Proximal.BoneOrientation;
			MiddleInterRight = OwningPawn->RightHand->Middle.Intermediate.BoneOrientation;
			MiddleDistRight = OwningPawn->RightHand->Middle.Distal.BoneOrientation;
			RingProxRight = OwningPawn->RightHand->Ring.Proximal.BoneOrientation;
			RingInterRight = OwningPawn->RightHand->Ring.Intermediate.BoneOrientation;
			RingDistRight = OwningPawn->RightHand->Ring.Distal.BoneOrientation;
			PinkyProxRight = OwningPawn->RightHand->Pinky.Proximal.BoneOrientation;
			PinkyInterRight = OwningPawn->RightHand->Pinky.Intermediate.BoneOrientation;
			PinkyDistRight = OwningPawn->RightHand->Pinky.Distal.BoneOrientation;
		}
		else
		{
			TrackRight = 0;

			ElbowRight = OwningPawn->GetConvertedRotation(FName("lowerarm_r")).Rotator();//UKinectFunctionLibrary::GetWorldJointTransform(EJoint::JointType_WristRight).Rotator();
			WristRight = OwningPawn->GetConvertedRotation(FName("hand_r")).Rotator();//UKinectFunctionLibrary::GetWorldJointTransform(EJoint::JointType_HandRight).Rotator();
		}
		
	}
}