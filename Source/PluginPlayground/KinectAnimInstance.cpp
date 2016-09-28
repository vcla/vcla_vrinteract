// Fill out your copyright notice in the Description page of Project Settings.

#include "PluginPlayground.h"
#include "KinectFunctionLibrary.h"
#include "KinectAnimInstance.h"
//
//UKinectAnimInstance::UKinectAnimInstance(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
//{
//	LeftHandAnim = NewObject<UHandObject>();
//}

void UKinectAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningPawn = Cast<AVRPawn>(TryGetPawnOwner());
	LeftHandAnim = NewObject<UHandObject>();
	RightHandAnim = NewObject<UHandObject>();

	//TArray<TEnumAsByte<EJoint::Type>> BoneArray;
	//UKinectFunctionLibrary::BoneMap.GenerateKeyArray(BoneArray);
	//for (auto bone : BoneArray)
	//{
	//	//BoneTransformMap.Add()
	//}
}

void UKinectAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	Super::NativeUpdateAnimation(DeltaTimeX);

	if (OwningPawn)
	{
		LeftHandAnim->CopyHand(OwningPawn->LeftHand);
		RightHandAnim->CopyHand(OwningPawn->RightHand);
	}
	
		//UE_LOG(LogTemp, Warning, TEXT("Updating Global Bone Transform"));
		//UKinectFunctionLibrary::GlobalBoneTransformMap.Empty();
		//UKinectFunctionLibrary::UpdateGlobalBoneOrientation(EJoint::JointType_SpineBase, HipsSkeletalComponentOffset.Quaternion());
	
}

FTransform UKinectAnimInstance::GetJointOrientation(EJoint::Type JointType)
{
	//FTransform Result = FTransform();
	//Result.SetRotation(Result.GetRotation() * HipsSkeletalComponentOffset.Quaternion());
	//return Result;
	return UKinectFunctionLibrary::MyBody.KinectBones[JointType].JointTransform;
	//if (UKinectFunctionLibrary::GlobalBoneTransformMap.Contains(JointType))
	//{
	//	return UKinectFunctionLibrary::GlobalBoneTransformMap[JointType];
	//}
	//return FTransform();
	//FTransform Result = UKinectFunctionLibrary::MyBody.KinectBones[JointType].JointTransform;
	//Result.SetRotation(UKinectFunctionLibrary::GlobalBoneTransformMap[JointType]);
	//for (auto bone : AnimBody.KinectBones)
	//{
	//	if (bone.JointTypeEnd == JointType)
	//	{
	//		
	//		Result = bone.JointTransform;
	//		Result.SetRotation(FRotator(90, 90, -90).Quaternion() * Result.GetRotation());
	//	}
	//}
	//return Result;
}

FTransform UKinectAnimInstance::GetConvertedRotation(FName BoneName)
{
	if (OwningPawn)
	{
		return OwningPawn->GetConvertedRotation(BoneName);
	}
	return FTransform();
}
