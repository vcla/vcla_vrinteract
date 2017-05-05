// Fill out your copyright notice in the Description page of Project Settings.

#include "PluginPlayground.h"
#include "KinectFunctionLibrary.h"
#include "BasePawn.h"


// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));

	HeadOffset = CreateDefaultSubobject<USceneComponent>(TEXT("HeadOffset"));
	CameraView = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	BodyMesh->SetupAttachment(RootComponent);

	HeadOffset->SetupAttachment(RootComponent);
	CameraView->SetupAttachment(HeadOffset);
	HeadMesh->SetupAttachment(CameraView);
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();

	for (auto BoneInfo : BoneInfoArray)
	{
		BoneInfoMap.Add(BoneInfo.AvatarBoneName, BoneInfo);
	}
}

// Called every frame
void ABasePawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	FVector HipsTranslationOffset = UKinectFunctionLibrary::GetWorldJointTransform(EJoint::JointType_SpineBase).GetTranslation() - KinectNeutralOffset.GetTranslation();
	BodyMesh->SetRelativeLocation(HipsTranslationOffset);
	UpdateBodyAnim();
}

void ABasePawn::CalibratePawn()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();

	KinectNeutralOffset = UKinectFunctionLibrary::GetWorldJointTransform(EJoint::JointType_SpineBase);
	BodyMesh->SetRelativeLocation(FVector(0, 0, 0));
}

FTransform ABasePawn::GetConvertedTransform(FName BoneName)
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

// Called to bind functionality to input
void ABasePawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void ABasePawn::UpdateBodyAnim()
{
	if (!BodyMesh)
	{
		return;
	}

	UVRAnimInstance* AnimInstance = Cast<UVRAnimInstance>(BodyMesh->GetAnimInstance());

	if (!AnimInstance)
	{
		return;
	}

	AnimInstance->SpineBase = GetConvertedTransform(FName("pelvis")).Rotator();
	AnimInstance->SpineMid = GetConvertedTransform(FName("spine_02")).Rotator();
	AnimInstance->SpineTop = GetConvertedTransform(FName("spine_03")).Rotator();
	AnimInstance->ShoulderLeft = GetConvertedTransform(FName("upperarm_l")).Rotator();
	AnimInstance->ShoulderRight = GetConvertedTransform(FName("upperarm_r")).Rotator();
	AnimInstance->HipLeft = GetConvertedTransform(FName("thigh_l")).Rotator();
	AnimInstance->KneeLeft = GetConvertedTransform(FName("calf_l")).Rotator();
	AnimInstance->FootLeft = GetConvertedTransform(FName("foot_l")).Rotator();
	AnimInstance->HipRight = GetConvertedTransform(FName("thigh_r")).Rotator();
	AnimInstance->KneeRight = GetConvertedTransform(FName("calf_r")).Rotator();
	AnimInstance->FootRight = GetConvertedTransform(FName("foot_r")).Rotator();

	AnimInstance->LeftElbowLocation = GetConvertedTransform(FName("lowerarm_l")).GetLocation();
	AnimInstance->RightElbowLocation = GetConvertedTransform(FName("lowerarm_r")).GetLocation();

	AnimInstance->ElbowLeft = GetConvertedTransform(FName("lowerarm_l")).Rotator();
	AnimInstance->WristLeft = GetConvertedTransform(FName("hand_l")).Rotator();
	AnimInstance->ElbowRight = GetConvertedTransform(FName("lowerarm_r")).Rotator();
	AnimInstance->WristRight = GetConvertedTransform(FName("hand_r")).Rotator();
}

void ABasePawn::Grab()
{

	UE_LOG(LogTemp, Warning, TEXT("Grabbing"));
}

void ABasePawn::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Releasing"));
}
