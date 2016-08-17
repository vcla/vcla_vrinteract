#pragma once

#include "Core.h"
#include "CoreUObject.h"
#include "Engine.h"
#include "Engine/EngineTypes.h"
#include "AllowWindowsPlatformTypes.h"
#include "Kinect.h"
#include "HideWindowsPlatformTypes.h"
#include "KinectStructs.generated.h"

/**************************************************************************************************
* Kinect functions exposed for use in UE4
* Assorted functions and structs taken from Kinect 4 Windows 2.0 plugin by Leon Rosengarten
* https://forums.unrealengine.com/showthread.php?389-Kinect-4-Windows-v2-0-Plugin
* 
**************************************************************************************************/

#pragma region Kinect Types

UENUM(BlueprintType)
namespace EJoint {
	enum Type
	{
		JointType_SpineBase = 0 UMETA(DisplayName = "JointType_SpineBase"),
		JointType_SpineMid = 1 UMETA(DisplayName = "JointType_SpineMid"),
		JointType_Neck = 2 UMETA(DisplayName = "JointType_Neck"),
		JointType_Head = 3 UMETA(DisplayName = "JointType_Head"),
		JointType_ShoulderLeft = 4 UMETA(DisplayName = "JointType_ShoulderLeft"),
		JointType_ElbowLeft = 5 UMETA(DisplayName = "JointType_ElbowLeft"),
		JointType_WristLeft = 6 UMETA(DisplayName = "JointType_WristLeft"),
		JointType_HandLeft = 7 UMETA(DisplayName = "JointType_HandLeft"),
		JointType_ShoulderRight = 8 UMETA(DisplayName = "JointType_ShoulderRight"),
		JointType_ElbowRight = 9 UMETA(DisplayName = "JointType_ElbowRight"),
		JointType_WristRight = 10 UMETA(DisplayName = "JointType_WristRight"),
		JointType_HandRight = 11 UMETA(DisplayName = "JointType_HandRight"),
		JointType_HipLeft = 12 UMETA(DisplayName = "JointType_HipLeft"),
		JointType_KneeLeft = 13 UMETA(DisplayName = "JointType_KneeLeft"),
		JointType_AnkleLeft = 14 UMETA(DisplayName = "JointType_AnkleLeft"),
		JointType_FootLeft = 15 UMETA(DisplayName = "JointType_FootLeft"),
		JointType_HipRight = 16 UMETA(DisplayName = "JointType_HipRight"),
		JointType_KneeRight = 17 UMETA(DisplayName = "JointType_KneeRight"),
		JointType_AnkleRight = 18 UMETA(DisplayName = "JointType_AnkleRight"),
		JointType_FootRight = 19 UMETA(DisplayName = "JointType_FootRight"),
		JointType_SpineShoulder = 20 UMETA(DisplayName = "JointType_SpineShoulder"),
		JointType_HandTipLeft = 21 UMETA(DisplayName = "JointType_HandTipLeft"),
		JointType_ThumbLeft = 22 UMETA(DisplayName = "JointType_ThumbLeft"),
		JointType_HandTipRight = 23 UMETA(DisplayName = "JointType_HandTipRight"),
		JointType_ThumbRight = 24 UMETA(DisplayName = "JointType_ThumbRight"),
		JointType_Count = 25 UMETA(DisplayName = "JointType_Count", Hidden)
	};
}

UENUM(BlueprintType)
namespace ETrackingState {

	enum Type {
		TrackingState_NotTracked = 0 UMETA(DisplayName = "TrackingState_NotTracked"),
		TrackingState_Inferred = 1 UMETA(DisplayName = "TrackingState_Inferred"),
		TrackingState_Tracked = 2 UMETA(DisplayName = "TrackingState_Tracked")
	};

}

USTRUCT(BlueprintType)
struct KINECTVCLA_API FKinectBone
{
	GENERATED_BODY()

public:

	/**************************************************************************************************
	* Default constructor.
	*
	* @author	Leon Rosengarten
	**************************************************************************************************/

	FKinectBone();

	FKinectBone(const JointOrientation& jointOrientation, const Joint& joint);

	FKinectBone& operator = (const FKinectBone& Other);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinect")
		TEnumAsByte<EJoint::Type> JointTypeStart;   ///< The joint type start
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinect")
		TEnumAsByte<EJoint::Type> JointTypeEnd; ///< The joint type end

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinect")
		FQuat Orientation;  ///< The orientation in kinect camera space relative to parent joint
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinect")
		TEnumAsByte<ETrackingState::Type> TrackingState;	///< State of the tracking

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinect")
		FTransform JointTransform;  ///< The joint transform in ue4

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinect")
		TArray<TEnumAsByte<EJoint::Type>> Children;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinect")
		FVector CameraSpacePoint;   ///< The translation in kinect camera space relative to camera
};



USTRUCT(BlueprintType)
struct KINECTVCLA_API FBody
{
	GENERATED_BODY()

public:



	FBody();

	/**************************************************************************************************
	* Constructor.
	*
	* @author	Leon Rosengarten
	* @param [in,out]	pBody	If non-null, the body.
	**************************************************************************************************/

	FBody(IBody* pBody);

	/**************************************************************************************************
	* Assignment operator.
	*
	* @author	Leon Rosengarten
	* @param	OtherBody	The other body.
	*
	* @return	A shallow copy of this object.
	**************************************************************************************************/

	FBody& operator = (const FBody & OtherBody);

		TArray<FKinectBone> KinectBones;	///< The kinect bones

		FVector Lean;

		uint64 TrackingId;  ///< Identifier for the tracking

		bool bIsTracked;	///< true if this object is tracked
};
