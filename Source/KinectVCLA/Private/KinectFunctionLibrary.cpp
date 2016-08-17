#include "KinectVCLAPrivatePCH.h"

//TSharedPtr<FKinectSensor> UKinectFunctionLibrary::KinectSensor = TSharedPtr<FKinectSensor>(new FKinectSensor());
TMap<TEnumAsByte<EJoint::Type>, TEnumAsByte<EJoint::Type>> UKinectFunctionLibrary::BoneMap;
TMap<TEnumAsByte<EJoint::Type>, FTransform> UKinectFunctionLibrary::GlobalBoneTransformMap;
FBody UKinectFunctionLibrary::MyBody;

UKinectFunctionLibrary::UKinectFunctionLibrary(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	BoneMap.Add(EJoint::JointType_SpineBase, EJoint::JointType_SpineBase);
	BoneMap.Add(EJoint::JointType_SpineMid, EJoint::JointType_SpineBase);
	BoneMap.Add(EJoint::JointType_Neck, EJoint::JointType_SpineMid);
	BoneMap.Add(EJoint::JointType_Head, EJoint::JointType_Neck);
	BoneMap.Add(EJoint::JointType_ShoulderLeft, EJoint::JointType_SpineShoulder);//
	BoneMap.Add(EJoint::JointType_ElbowLeft, EJoint::JointType_ShoulderLeft);
	BoneMap.Add(EJoint::JointType_WristLeft, EJoint::JointType_ElbowLeft);
	BoneMap.Add(EJoint::JointType_HandLeft, EJoint::JointType_WristLeft);
	BoneMap.Add(EJoint::JointType_ShoulderRight, EJoint::JointType_SpineShoulder);//
	BoneMap.Add(EJoint::JointType_ElbowRight, EJoint::JointType_ShoulderRight);
	BoneMap.Add(EJoint::JointType_WristRight, EJoint::JointType_ElbowRight);
	BoneMap.Add(EJoint::JointType_HandRight, EJoint::JointType_WristRight);
	BoneMap.Add(EJoint::JointType_HipLeft, EJoint::JointType_SpineBase);
	BoneMap.Add(EJoint::JointType_KneeLeft, EJoint::JointType_HipLeft);
	BoneMap.Add(EJoint::JointType_AnkleLeft, EJoint::JointType_KneeLeft);
	BoneMap.Add(EJoint::JointType_FootLeft, EJoint::JointType_AnkleLeft);
	BoneMap.Add(EJoint::JointType_HipRight, EJoint::JointType_SpineBase);
	BoneMap.Add(EJoint::JointType_KneeRight, EJoint::JointType_HipRight);
	BoneMap.Add(EJoint::JointType_AnkleRight, EJoint::JointType_KneeRight);
	BoneMap.Add(EJoint::JointType_FootRight, EJoint::JointType_AnkleRight);
	BoneMap.Add(EJoint::JointType_SpineShoulder, EJoint::JointType_SpineMid);
	BoneMap.Add(EJoint::JointType_HandTipLeft, EJoint::JointType_HandLeft);
	BoneMap.Add(EJoint::JointType_ThumbLeft, EJoint::JointType_HandLeft);
	BoneMap.Add(EJoint::JointType_HandTipRight, EJoint::JointType_HandRight);
	BoneMap.Add(EJoint::JointType_ThumbRight, EJoint::JointType_HandRight);

	TArray<TEnumAsByte<EJoint::Type>> BoneArray;
	BoneMap.GenerateKeyArray(BoneArray);

	for (auto bone : BoneArray)
	{
		GlobalBoneTransformMap.Add(bone, FTransform());
	}
}

void UKinectFunctionLibrary::StartSensor()
{
	if (IKinectVCLAModule::IsAvailable())
	{
		IKinectVCLAModule::Get().StartSensor();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Kinect Module not loaded"));
	}
}

void UKinectFunctionLibrary::StopSensor()
{
	if (IKinectVCLAModule::IsAvailable())
	{
		IKinectVCLAModule::Get().StopSensor();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Kinect Module not loaded"));
	}
}

bool UKinectFunctionLibrary::UpdateBody()
{
	if (IKinectVCLAModule::IsAvailable())
	{
		if (IKinectVCLAModule::Get().GetBody(MyBody))
		{

			return true;
		}
		//return Result.KinectBones[0].JointTransform;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Kinect Module not loaded"));
		//return FTransform();
	}
	return false;
}

void UKinectFunctionLibrary::UpdateGlobalBoneOrientation(TEnumAsByte<EJoint::Type> BoneParentJoint, FQuat ParentOrientation)
{

	FKinectBone Bone = MyBody.KinectBones[BoneParentJoint];
	if (GlobalBoneTransformMap.Contains(Bone.JointTypeEnd))
	{
		return;
	}
	else
	{
		FTransform Result;
		Result.SetRotation(Bone.JointTransform.GetRotation() * ParentOrientation);
		Result.SetTranslation(Bone.JointTransform.GetTranslation());
		GlobalBoneTransformMap.Add(Bone.JointTypeEnd, Result);
		for (auto ChildBone : Bone.Children)
		{
			UpdateGlobalBoneOrientation(ChildBone, Result.GetRotation());
		}
	}
}

FTransform UKinectFunctionLibrary::GetLocalJointTransform(EJoint::Type JointType)
{
	if (MyBody.KinectBones.Num() > 0)
	{
		return MyBody.KinectBones[JointType].JointTransform;
	}
	return FTransform();
}
