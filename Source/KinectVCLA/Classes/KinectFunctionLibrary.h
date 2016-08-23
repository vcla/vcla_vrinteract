#pragma once

#include "KinectStructs.h"
#include "KinectFunctionLibrary.generated.h"


#pragma endregion
UCLASS()
class KINECTVCLA_API UKinectFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:
	static void StartSensor();
	static void StopSensor();

	static TMap<TEnumAsByte<EJoint::Type>, TEnumAsByte<EJoint::Type>> BoneMap;
	static bool UpdateBody();

	//static void UpdateGlobalBoneOrientation(TEnumAsByte<EJoint::Type> BoneParentJoint, FQuat ParentOrientation);

	static FBody MyBody;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kinect")
		static FTransform GetWorldJointTransform(EJoint::Type JointType);

private:
	//static TSharedPtr<FKinectSensor> KinectSensor;
	
};

