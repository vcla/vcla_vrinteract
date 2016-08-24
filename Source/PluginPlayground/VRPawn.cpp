// Fill out your copyright notice in the Description page of Project Settings.

#include "PluginPlayground.h"
#include "VRPawn.h"
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

	BodyMesh->SetupAttachment(RootComponent);

	HeadOffset->SetupAttachment(RootComponent);
	CameraView->SetupAttachment(HeadOffset);
	HeadMesh->SetupAttachment(CameraView);
}

// Called when the game starts or when spawned
void AVRPawn::BeginPlay()
{
	Super::BeginPlay();
	

	for (auto BoneInfo : BoneInfoArray)
	{
		BoneInfoMap.Add(BoneInfo.AvatarBoneName, BoneInfo);
	}


	CalibratePawn();
}

// Called every frame
void AVRPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	FVector HipsTranslationOffset = UKinectFunctionLibrary::GetWorldJointTransform(EJoint::JointType_SpineBase).GetTranslation() - KinectNeutralOffset.GetTranslation();
	BodyMesh->SetRelativeLocation(HipsTranslationOffset);
}

// Called to bind functionality to input
void AVRPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Calibrate", EInputEvent::IE_Pressed, this, &AVRPawn::CalibratePawn);
}

void AVRPawn::CalibratePawn()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();

	KinectNeutralOffset = UKinectFunctionLibrary::GetWorldJointTransform(EJoint::JointType_SpineBase);
	BodyMesh->SetRelativeLocation(FVector(0, 0, 0));

	//HeadOffset->SetRelativeRotation(UKinectFunctionLibrary::GetWorldJointTransform(EJoint::JointType_Neck).GetRotation());
}

FTransform AVRPawn::GetConvertedRotation(FName BoneName)
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
