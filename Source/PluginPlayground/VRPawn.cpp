// Fill out your copyright notice in the Description page of Project Settings.

#include "PluginPlayground.h"
#include "VRPawn.h"
#include "VRAnimInstance.h"
#include "HeadMountedDisplay.h"
#include "KinectFunctionLibrary.h"


// Sets default values
AVRPawn::AVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));

	HeadOffset = CreateDefaultSubobject<USceneComponent>(TEXT("HeadOffset"));
	CameraView = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	LeapController = CreateDefaultSubobject<ULeapController>(TEXT("LeapController"));
	LeftHand = CreateDefaultSubobject<UHandObject>(TEXT("LeftHand"));
	RightHand = CreateDefaultSubobject<UHandObject>(TEXT("RightHand"));

	BodyMesh->SetupAttachment(RootComponent);

	HeadOffset->SetupAttachment(RootComponent);
	CameraView->SetupAttachment(HeadOffset);
	HeadMesh->SetupAttachment(CameraView);

	AddOwnedComponent(LeapController);

	LeftHandNeutralOffset = FRotator(1, 1, 1);
	RightHandNeutralOffset = FRotator(1, 1, 1);

	GrabThreshold = 0.4;
}

// Called when the game starts or when spawned
void AVRPawn::BeginPlay()
{
	Super::BeginPlay();
	

	for (auto BoneInfo : BoneInfoArray)
	{
		BoneInfoMap.Add(BoneInfo.AvatarBoneName, BoneInfo);
	}

	LeftHand->NeutralBoneOrientation = LeftHandNeutralOffset;
	RightHand->NeutralBoneOrientation = RightHandNeutralOffset;

	CalibratePawn();
}

// Called every frame
void AVRPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	FVector HipsTranslationOffset = UKinectFunctionLibrary::GetWorldJointTransform(EJoint::JointType_SpineBase).GetTranslation() - KinectNeutralOffset.GetTranslation();
	BodyMesh->SetRelativeLocation(HipsTranslationOffset);

	FireGrabEvents(LeftHand);
	FireGrabEvents(RightHand);

	UpdateAnimVariables();
}

// Called to bind functionality to input
void AVRPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Calibrate", EInputEvent::IE_Pressed, this, &AVRPawn::CalibratePawn);
}
//
//void AVRPawn::LeapHandMoved_Implementation(ULeapHand* hand)
//{
//	
//	ILeapEventInterface* einterface = Cast<ILeapEventInterface>(this);
//	if (einterface)
//	{
//		//FString LogString = einterface->ToString();
//		//UE_LOG(LogTemp, Warning, TEXT("%s"), *LogString);
//	}
//	
//}

void AVRPawn::CalibratePawn()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();

	KinectNeutralOffset = UKinectFunctionLibrary::GetWorldJointTransform(EJoint::JointType_SpineBase);
	BodyMesh->SetRelativeLocation(FVector(0, 0, 0));

	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		LeapController->OptimizeForHMD(true, true, true);
		LeapController->SetLeapMountToHMDOffset(FVector(9.f, 0, 0));
	}

	//HeadOffset->SetRelativeRotation(UKinectFunctionLibrary::GetWorldJointTransform(EJoint::JointType_Neck).GetRotation());
}

FTransform AVRPawn::GetConvertedTransform(FName BoneName)
{
	FAvatarBoneInfo* Info = BoneInfoMap.Find(BoneName);
	FTransform Result = FTransform();
	if (Info)
	{
		Result = UKinectFunctionLibrary::MyBody.KinectBones[Info->KinectJointType].JointTransform;
		Result.ConcatenateRotation(Info->NeutralBoneRotation.Quaternion());
	}
	return Result;
}

void AVRPawn::Grab(UHandObject* GrabbingHand)
{
	//setup grab trace parameters
	FCollisionObjectQueryParams ObjectParams(CAN_GRAB_OBJECT);
	FCollisionShape GrabRegion = FCollisionShape::MakeSphere(5.f);
	GetWorld()->DebugDrawTraceTag = FName("GrabTag");
	FCollisionQueryParams GrabCollisionParams;
	GrabCollisionParams.TraceTag = FName("GrabTag");

	TArray<FOverlapResult> OverlapResults;
	FVector WorldGrabCenter = BodyMesh->GetComponentTransform().TransformVector(GrabbingHand->GrabCenter + HeadOffset->RelativeLocation);
	GetWorld()->OverlapMultiByObjectType(OverlapResults, WorldGrabCenter, FQuat(0, 0, 0, 1), ObjectParams, GrabRegion, GrabCollisionParams);

	UE_LOG(LogTemp, Warning, TEXT("BodyMesh Location: %s"), *BodyMesh->GetComponentLocation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("GrabCenter: %s"), *WorldGrabCenter.ToString());

	UE_LOG(LogTemp, Warning, TEXT("Grabbing"));
}

void AVRPawn::Release(UHandObject* ReleasingHand)
{
	UE_LOG(LogTemp, Warning, TEXT("Releasing"));
}

void AVRPawn::FireGrabEvents(UHandObject * Hand)
{
	if (Hand->CurrentGrabStrength > GrabThreshold)
	{
		if (Hand->PreviousGrabStrength <= GrabThreshold)
		{
			Grab(Hand);
		}
	}
	else
	{
		if (Hand->PreviousGrabStrength > GrabThreshold)
		{
			Release(Hand);
		}
	}
}

void AVRPawn::UpdateAnimVariables()
{
	if (!BodyMesh)
	{
		return;
	}

	UVRAnimInstance* MyInstance = Cast<UVRAnimInstance>(BodyMesh->GetAnimInstance());

	if (!MyInstance)
	{
		return;
	}

	MyInstance->ThumbProxLeft = LeftHand->Thumb.Proximal.BoneOrientation;
	MyInstance->ThumbInterLeft = LeftHand->Thumb.Intermediate.BoneOrientation;
	MyInstance->ThumbDistLeft = LeftHand->Thumb.Distal.BoneOrientation;
	MyInstance->IndexProxLeft = LeftHand->Index.Proximal.BoneOrientation;
	MyInstance->IndexInterLeft = LeftHand->Index.Intermediate.BoneOrientation;
	MyInstance->IndexDistLeft = LeftHand->Index.Distal.BoneOrientation;
	MyInstance->MiddleProxLeft = LeftHand->Middle.Proximal.BoneOrientation;
	MyInstance->MiddleInterLeft = LeftHand->Middle.Intermediate.BoneOrientation;
	MyInstance->MiddleDistLeft = LeftHand->Middle.Distal.BoneOrientation;
	MyInstance->RingProxLeft = LeftHand->Ring.Proximal.BoneOrientation;
	MyInstance->RingInterLeft = LeftHand->Ring.Intermediate.BoneOrientation;
	MyInstance->RingDistLeft = LeftHand->Ring.Distal.BoneOrientation;
	MyInstance->PinkyProxLeft = LeftHand->Pinky.Proximal.BoneOrientation;
	MyInstance->PinkyInterLeft = LeftHand->Pinky.Intermediate.BoneOrientation;
	MyInstance->PinkyDistLeft = LeftHand->Pinky.Distal.BoneOrientation;
}
