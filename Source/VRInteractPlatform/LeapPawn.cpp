// Fill out your copyright notice in the Description page of Project Settings.

#include "VRInteractPlatform.h"
#include "LeapPawn.h"
#include "HeadMountedDisplay.h"


// Sets default values
ALeapPawn::ALeapPawn()
{

	LeapController = CreateDefaultSubobject<ULeapController>(TEXT("LeapController"));
	LeftHand = CreateDefaultSubobject<UHandObject>(TEXT("LeftHand"));
	RightHand = CreateDefaultSubobject<UHandObject>(TEXT("RightHand"));

	AddOwnedComponent(LeapController);

	LeftHandNeutralOffset = FRotator(1, 1, 1);
	RightHandNeutralOffset = FRotator(1, 1, 1);

	LeftHand->IsLeft = true;
	RightHand->IsLeft = false;

	GrabThreshold = 0.4;
}

// Called when the game starts or when spawned
void ALeapPawn::BeginPlay()
{
	Super::BeginPlay();
	

	LeftHand->NeutralBoneOrientation = LeftHandNeutralOffset;
	RightHand->NeutralBoneOrientation = RightHandNeutralOffset;

	CalibratePawn();
}

// Called every frame
void ALeapPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	
	if (LeftHand->Confidence > 0)
	{
		FireGrabEvents(LeftHand);
	}
	if (RightHand->Confidence > 0)
	{
		FireGrabEvents(RightHand);
	}

	UpdateHandVariables();
}

void ALeapPawn::CalibratePawn()
{
	
	Super::CalibratePawn();

	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		LeapController->OptimizeForHMD(true, true, true);
		LeapController->SetLeapMountToHMDOffset(FVector(9.f, 0, 0));
	}

}

void ALeapPawn::FireGrabEvents(UHandObject * Hand)
{
	UWorld* TheWorld = this->GetWorld();
	if (Hand->CurrentGrabStrength > GrabThreshold)
	{
		if (Hand->PreviousGrabStrength <= GrabThreshold)
		{
			TArray<UPrimitiveComponent*> ComponentArray = TArray<UPrimitiveComponent*>();
			//spherecast yaaay
			FVector HandLocation = BodyMesh->GetSocketLocation(Hand->IsLeft ? LeftHandAttachPoint : RightHandAttachPoint);
			DrawDebugSphere(TheWorld, HandLocation, 5.f, 8, FColor(255, 0, 0), true);

			TArray<FHitResult> OutResults;
			FCollisionShape GrabSphere = FCollisionShape::MakeSphere(5.f);
			FCollisionObjectQueryParams ObjectParams;
			FCollisionQueryParams CollisionParams;
			ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
			ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);
			CollisionParams.bFindInitialOverlaps = true;
			TheWorld->SweepMultiByObjectType(OutResults, HandLocation, HandLocation + FVector(.1, 0, 0), FQuat(), ObjectParams, GrabSphere, CollisionParams);

			Grab(Hand->IsLeft, OutResults);
		}
	}
	else
	{
		if (Hand->PreviousGrabStrength > GrabThreshold)
		{
			Release(Hand->IsLeft);
		}
	}
}

void ALeapPawn::UpdateHandVariables()
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

	if (LeftHand->Confidence > 0.1)
	{
		MyInstance->TrackLeft = 1.0f;
		MyInstance->LeftHandLocation = LeftHand->HandLocation + HeadOffset->RelativeLocation;

		MyInstance->ElbowLeft = LeftHand->Arm.BoneOrientation;
		MyInstance->WristLeft = LeftHand->Palm.BoneOrientation;

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
	else
	{
		MyInstance->TrackLeft = 0;
	}

	if (RightHand->Confidence > 0.1)
	{
		MyInstance->TrackRight = 1.0f;

		MyInstance->RightHandLocation = RightHand->HandLocation + HeadOffset->RelativeLocation;

		MyInstance->ElbowRight = RightHand->Arm.BoneOrientation;
		MyInstance->WristRight = RightHand->Palm.BoneOrientation;

		//Set fingers from Leap only
		MyInstance->ThumbProxRight = RightHand->Thumb.Proximal.BoneOrientation;
		MyInstance->ThumbInterRight = RightHand->Thumb.Intermediate.BoneOrientation;
		MyInstance->ThumbDistRight = RightHand->Thumb.Distal.BoneOrientation;
		MyInstance->IndexProxRight = RightHand->Index.Proximal.BoneOrientation;
		MyInstance->IndexInterRight = RightHand->Index.Intermediate.BoneOrientation;
		MyInstance->IndexDistRight = RightHand->Index.Distal.BoneOrientation;
		MyInstance->MiddleProxRight = RightHand->Middle.Proximal.BoneOrientation;
		MyInstance->MiddleInterRight = RightHand->Middle.Intermediate.BoneOrientation;
		MyInstance->MiddleDistRight = RightHand->Middle.Distal.BoneOrientation;
		MyInstance->RingProxRight = RightHand->Ring.Proximal.BoneOrientation;
		MyInstance->RingInterRight = RightHand->Ring.Intermediate.BoneOrientation;
		MyInstance->RingDistRight = RightHand->Ring.Distal.BoneOrientation;
		MyInstance->PinkyProxRight = RightHand->Pinky.Proximal.BoneOrientation;
		MyInstance->PinkyInterRight = RightHand->Pinky.Intermediate.BoneOrientation;
		MyInstance->PinkyDistRight = RightHand->Pinky.Distal.BoneOrientation;
	}
	else
	{
		MyInstance->TrackRight = 0;
	}
	
}
