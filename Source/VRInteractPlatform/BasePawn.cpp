// Fill out your copyright notice in the Description page of Project Settings.

#include "VRInteractPlatform.h"
#include "KinectFunctionLibrary.h"
#include "CustomGrabInterface.h"
#include "BasePawn.h"


// Sets default values
ABasePawn::ABasePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));

	HeadOffset = CreateDefaultSubobject<USceneComponent>(TEXT("HeadOffset"));
	CameraView = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	BodyMesh->SetupAttachment(RootComponent);

	HeadOffset->SetupAttachment(BodyMesh);
	CameraView->SetupAttachment(HeadOffset);
	HeadMesh->SetupAttachment(CameraView);

	MovementSpeed = 150;
	TurnSpeed = .1f;

	UpdateInterval = .01f;
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

void ABasePawn::BeginDestroy()
{
	Super::BeginDestroy();

}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABasePawn::UpdateAnim()
{
	UpdateMoveAnim();
	//Update Avatar animation
	//BodyMesh->SetRelativeLocation(HipsTranslationOffset);
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

	InputComponent->BindAction("Calibrate", EInputEvent::IE_Pressed, this, &ABasePawn::CalibratePawn);
	InputComponent->BindAxis("MoveForward", this, &ABasePawn::ProcessForward);
	InputComponent->BindAxis("MoveRight", this, &ABasePawn::ProcessRight);
	InputComponent->BindAxis("RotateBody", this, &ABasePawn::ProcessRotate);
}

void ABasePawn::ProcessForward(float AxisValue)
{
	MovementInput.X = AxisValue;
}

void ABasePawn::ProcessRight(float AxisValue)
{
	MovementInput.Y = AxisValue;
}

void ABasePawn::ProcessRotate(float AxisValue)
{
	MovementInput.Z = AxisValue;
}

void ABasePawn::UpdateMoveAnim()
{
	//Consume movement input
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += TurnSpeed * MovementInput.Z * UpdateInterval; //DeltaTime;
	SetActorRotation(NewRotation);
	FVector DisplacementVector = FVector(0, 0, 0);
	DisplacementVector = GetActorForwardVector() * MovementInput.X + GetActorRightVector() * MovementInput.Y;
	DisplacementVector = DisplacementVector.GetSafeNormal();
	DisplacementVector = MovementSpeed * DisplacementVector * UpdateInterval; //DeltaTime;
	FVector NewLocation = GetActorLocation() + DisplacementVector;
	SetActorLocation(NewLocation);
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
		UE_LOG(LogTemp, Warning, TEXT("no bodymesh"));
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
	// AnimInstance->LeftHandLocation = GetConvertedTransform(FName("hand_l")).GetLocation();
	// AnimInstance->RightHandLocation = GetConvertedTransform(FName("hand_r")).GetLocation();

	// AnimInstance->ElbowLeft = GetConvertedTransform(FName("lowerarm_l")).Rotator();
	// AnimInstance->WristLeft = GetConvertedTransform(FName("hand_l")).Rotator();
	// AnimInstance->ElbowRight = GetConvertedTransform(FName("lowerarm_r")).Rotator();
	// AnimInstance->WristRight = GetConvertedTransform(FName("hand_r")).Rotator();

	AnimInstance->HipLocationOffset = UKinectFunctionLibrary::GetWorldJointTransform(EJoint::JointType_SpineBase).GetTranslation() - KinectNeutralOffset.GetTranslation();
}

void ABasePawn::Grab(bool IsLeft)
{
	//Spherecast to find all primitive components in grab range
	UWorld* TheWorld = this->GetWorld();

	FVector HandLocation = BodyMesh->GetSocketLocation(IsLeft ? LeftHandAttachPoint : RightHandAttachPoint);
	// DrawDebugSphere(TheWorld, HandLocation, 5.f, 8, FColor(255, 0, 0), true);

	TArray<FHitResult> OutResults;
	FCollisionShape GrabSphere = FCollisionShape::MakeSphere(5.f);
	FCollisionObjectQueryParams ObjectParams;
	FCollisionQueryParams CollisionParams;
	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);
	CollisionParams.bFindInitialOverlaps = true;
	TheWorld->SweepMultiByObjectType(OutResults, HandLocation, HandLocation + FVector(.1, 0, 0), FQuat(), ObjectParams, GrabSphere, CollisionParams);

	//Calling grab logic on all components in range
	FAttachmentTransformRules GrabRules = FAttachmentTransformRules::KeepWorldTransform;
	GrabRules.bWeldSimulatedBodies = true;
	if (IsLeft)
	{
		for (auto& Hit : OutResults)
		{
			ICustomGrabInterface* CustomGrabActor = Cast<ICustomGrabInterface>(Hit.GetActor());
			//special logic for components attached to special actors 
			//TODO:worry about multiple hits per actor?
			if (CustomGrabActor) 
			{
				LeftHandCustomGrab.Add(Hit.GetActor());
				CustomGrabActor->OnGrab(this, IsLeft);
			}
			//regular attach logic for other components
			else
			{
				UPrimitiveComponent* Comp = Hit.GetComponent();
				Comp->SetSimulatePhysics(false);

				Comp->AttachToComponent(BodyMesh, GrabRules, LeftHandAttachPoint);
				LeftHandGrabbedComponents.Add(Comp);
			}
		}
	}
	else
	{
		for (auto& Hit : OutResults)
		{
			ICustomGrabInterface* CustomGrabActor = Cast<ICustomGrabInterface>(Hit.GetActor());
			if (CustomGrabActor)
			{
				RightHandCustomGrab.Add(Hit.GetActor());
				CustomGrabActor->OnGrab(this, IsLeft);
			}
			else
			{
				UPrimitiveComponent* Comp = Hit.GetComponent();
				Comp->SetSimulatePhysics(false);

				Comp->AttachToComponent(BodyMesh, GrabRules, RightHandAttachPoint);
				RightHandGrabbedComponents.Add(Comp);
			}
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("Grabbing"));
}

void ABasePawn::Release(bool IsLeft)
{
	if (IsLeft)
	{
		for (auto& Comp : LeftHandGrabbedComponents)
		{
			Comp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			Comp->SetSimulatePhysics(true);
		}
		LeftHandGrabbedComponents.Empty();

		for (auto& CustomActor : LeftHandCustomGrab)
		{
			ICustomGrabInterface* CustomInterfaceRef = Cast<ICustomGrabInterface>(CustomActor);
			if (CustomInterfaceRef)
			{
				CustomInterfaceRef->OnRelease(this, IsLeft);
			}
		}
		LeftHandCustomGrab.Empty();
	}
	else
	{
		for (auto& Comp : RightHandGrabbedComponents)
		{
			Comp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			Comp->SetSimulatePhysics(true);
		}
		RightHandGrabbedComponents.Empty();

		for (auto& CustomActor : RightHandCustomGrab)
		{
			ICustomGrabInterface* CustomInterfaceRef = Cast<ICustomGrabInterface>(CustomActor);
			if (CustomInterfaceRef)
			{
				CustomInterfaceRef->OnRelease(this, IsLeft);
			}
		}
		RightHandCustomGrab.Empty();
	}

	//UE_LOG(LogTemp, Warning, TEXT("Releasing"));
}
