#include "KinectVCLAPrivatePCH.h"


#define NumOfKinectBones 25

FKinectBone::FKinectBone() {}

FKinectBone::FKinectBone(const JointOrientation& jointOrientation, const Joint& joint)
{


	JointTypeStart = UKinectFunctionLibrary::BoneMap[(EJoint::Type)joint.JointType];
	JointTypeEnd = (EJoint::Type)joint.JointType;

	TrackingState = (ETrackingState::Type)joint.TrackingState;
	CameraSpacePoint = FVector(joint.Position.X, joint.Position.Y, joint.Position.Z);

	Orientation = FQuat(jointOrientation.Orientation.x,
		jointOrientation.Orientation.y,
		jointOrientation.Orientation.z,
		jointOrientation.Orientation.w);

	FVector UETranslation = FVector(-CameraSpacePoint.Z, CameraSpacePoint.X, CameraSpacePoint.Y);
	FQuat UERotation = FQuat(-Orientation.Z, Orientation.X, Orientation.Y, -Orientation.W);// *FRotator(0.f, 0.f, 0.f).Quaternion();

	JointTransform.SetRotation(UERotation);

	JointTransform.SetTranslation(UETranslation);


}


FKinectBone& FKinectBone::operator=(const FKinectBone& Other)
{
	this->JointTypeStart = Other.JointTypeStart;

	this->JointTypeEnd = Other.JointTypeEnd;

	this->TrackingState = Other.TrackingState;

	this->CameraSpacePoint = Other.CameraSpacePoint;

	this->Orientation = Other.Orientation;

	this->JointTransform = Other.JointTransform;

	this->Children = Other.Children;

	return *this;
}

FBody::FBody() :bIsTracked(false) {

	KinectBones.AddZeroed(NumOfKinectBones);


	TrackingId = 0;

}


FBody::FBody(IBody* pBody) {

	KinectBones.AddZeroed(NumOfKinectBones);

	if (!pBody)
		return;

	BOOLEAN IsTracked;
	HRESULT hr = pBody->get_IsTracked(&IsTracked);

	if (FAILED(hr) || !IsTracked) {

		bIsTracked = false;

		return;

	}

	TrackingId = 0;

	hr = pBody->get_TrackingId(&TrackingId);

	if (FAILED(hr)) {

		bIsTracked = false;

		return;

	}


	Joint nJoints[NumOfKinectBones];
	hr = pBody->GetJoints(NumOfKinectBones, nJoints);

	if (FAILED(hr)) {

		bIsTracked = false;
		return;
	}


	JointOrientation nJointOrientation[NumOfKinectBones];

	hr = pBody->GetJointOrientations(NumOfKinectBones, nJointOrientation);
	if (FAILED(hr)) {

		bIsTracked = false;
		return;
	}

	for (size_t i = 0; i < NumOfKinectBones; i++) //first pass build global skeleton relations 
	{
		KinectBones[i] = FKinectBone(nJointOrientation[i], nJoints[i]);

	}

	for (auto Bone : KinectBones)
	{

		if (Bone.JointTypeEnd != EJoint::JointType_SpineBase)
			KinectBones[Bone.JointTypeStart].Children.AddUnique(Bone.JointTypeEnd);

		if (Bone.JointTypeEnd == EJoint::JointType_SpineBase
			|| Bone.JointTypeEnd == EJoint::JointType_HandTipLeft
			|| Bone.JointTypeEnd == EJoint::JointType_HandTipRight
			|| Bone.JointTypeEnd == EJoint::JointType_Head
			|| Bone.JointTypeEnd == EJoint::JointType_FootLeft
			|| Bone.JointTypeEnd == EJoint::JointType_FootRight)
			continue;



		FRotationMatrix R(Bone.JointTransform.GetRotation().Rotator());
	}

	HandState nLeftHandState, nRightHandState;
	if (FAILED(pBody->get_HandLeftState(&nLeftHandState)) || FAILED(pBody->get_HandRightState(&nRightHandState))) {
		bIsTracked = false;
		return;
	}


	PointF tempLean;
	pBody->get_Lean(&tempLean);

	Lean = FVector(tempLean.X, tempLean.Y, 0);

	bIsTracked = true;

}

FBody& FBody::operator=(const FBody & OtherBody)
{

	if (this->KinectBones.Num() == 0) {

		KinectBones.AddZeroed(NumOfKinectBones);

	}
	this->bIsTracked = OtherBody.bIsTracked;

	this->TrackingId = OtherBody.TrackingId;

	this->Lean = FVector(OtherBody.Lean);

	for (size_t i = 0; i < OtherBody.KinectBones.Num(); i++)
	{

		this->KinectBones[i] = OtherBody.KinectBones[i];
	}

	return *this;
}
