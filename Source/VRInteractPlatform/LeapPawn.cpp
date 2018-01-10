// Fill out your copyright notice in the Description page of Project Settings.

#include "VRInteractPlatform.h"
#include "LeapPawn.h"
#include "HeadMountedDisplay.h"

static Value VectorMaker(FVector vec, Document &doc)
{
	Value oj_vec(kObjectType);

	oj_vec.AddMember("X", vec.X, doc.GetAllocator());
	oj_vec.AddMember("Y", vec.Y, doc.GetAllocator());
	oj_vec.AddMember("Z", vec.Z, doc.GetAllocator());

	return oj_vec;
}

static Value RotatorMaker(FRotator rot, Document &doc)
{
	Value oj_rot(kObjectType);

	oj_rot.AddMember("Pitch", rot.Pitch, doc.GetAllocator());
	oj_rot.AddMember("Yaw", rot.Yaw, doc.GetAllocator());
	oj_rot.AddMember("Roll", rot.Roll, doc.GetAllocator());

	return oj_rot;
}

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

	UpdateInterval = 0.001;
	PoseRecord = false;
	PoseData = "";
}

// Called when the game starts or when spawned
void ALeapPawn::BeginPlay()
{
	Super::BeginPlay();
	

	LeftHand->NeutralBoneOrientation = LeftHandNeutralOffset;
	RightHand->NeutralBoneOrientation = RightHandNeutralOffset;

	CalibratePawn();

	PoseRecord = true;

	if(PoseRecord)
	{
		FString GameDir = FPaths::GameDir();
		FString FileName = GameDir+"HumanPoseRecord.csv";

		if(FPaths::FileExists(FileName))
	    {
	        FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*FileName);
	    }
	}

	FTimerHandle ReceiverHandler;
    GetWorldTimerManager().SetTimer(ReceiverHandler, this, &ALeapPawn::UpdateAnim, UpdateInterval, true);
}

void ALeapPawn::BeginDestroy()
{
	Super::BeginDestroy();

	if(PoseRecord)
	{
		FString GameDir = FPaths::GameDir();
		FString FileName = GameDir+"HumanPoseRecord.csv";

		FFileHelper::SaveStringToFile(PoseData, *FileName, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get());
	}
}

// Called every frame
void ALeapPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
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

void ALeapPawn::UpdateAnim()
{
	Super::UpdateAnim();

	LeftGrab = false; RightGrab = false;
	LeftRelease = false; RightRelease = false;

	if (LeftHand->Confidence > 0)
	{
		FireGrabEvents(LeftHand);
	}
	if (RightHand->Confidence > 0)
	{
		FireGrabEvents(RightHand);
	}

	UpdateHandVariables();

	// record pose data
	AnimRecord();
}

void ALeapPawn::FireGrabEvents(UHandObject * Hand)
{
	if (Hand->CurrentGrabStrength > GrabThreshold)
	{
		if (Hand->PreviousGrabStrength <= GrabThreshold)
		{
			Grab(Hand->IsLeft);

			LeftGrab = Hand->IsLeft ? true : LeftGrab;
			LeftRelease = Hand->IsLeft ? false : LeftRelease;
			RightGrab = Hand->IsLeft ? RightGrab : true;
			RightRelease = Hand->IsLeft ? RightRelease : false;
		}
	}
	else
	{
		if (Hand->PreviousGrabStrength > GrabThreshold)
		{
			Release(Hand->IsLeft);

			LeftRelease = Hand->IsLeft ? true : LeftRelease;
			LeftGrab = Hand->IsLeft ? false : LeftGrab;
			RightRelease = Hand->IsLeft ? RightRelease : true;
			RightGrab = Hand->IsLeft ? RightGrab : false;
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

void ALeapPawn::AnimRecord()
{
	Value OJ(kObjectType);

	// Actor Pose
	Value OJActor(kObjectType);
	FVector Loc = GetActorLocation();
	FRotator Rot = GetActorRotation();
	OJActor.AddMember("ActorLoc", VectorMaker(Loc, doc), doc.GetAllocator());
	OJActor.AddMember("ActorRot", RotatorMaker(Rot, doc), doc.GetAllocator());
	OJ.AddMember("ActorPose", OJActor, doc.GetAllocator());

	UVRAnimInstance* MyInstance = Cast<UVRAnimInstance>(BodyMesh->GetAnimInstance());

	if(MyInstance)
	{
		// Body Pose
		Value OJBody(kObjectType);
		OJBody.AddMember("SpineBaseRot", RotatorMaker(MyInstance->SpineBase, doc), doc.GetAllocator());
		OJBody.AddMember("SpineMidRot", RotatorMaker(MyInstance->SpineMid, doc), doc.GetAllocator());
		OJBody.AddMember("SpineTopRot", RotatorMaker(MyInstance->SpineTop, doc), doc.GetAllocator());
		OJBody.AddMember("ShoulderLeftRot", RotatorMaker(MyInstance->ShoulderLeft, doc), doc.GetAllocator());
		OJBody.AddMember("ShoulderRightRot", RotatorMaker(MyInstance->ShoulderRight, doc), doc.GetAllocator());
		OJBody.AddMember("HipLeftRot", RotatorMaker(MyInstance->HipLeft, doc), doc.GetAllocator());
		OJBody.AddMember("KneeLeftRot", RotatorMaker(MyInstance->KneeLeft, doc), doc.GetAllocator());
		OJBody.AddMember("FootLeftRot", RotatorMaker(MyInstance->FootLeft, doc), doc.GetAllocator());
		OJBody.AddMember("HipRightRot", RotatorMaker(MyInstance->HipRight, doc), doc.GetAllocator());
		OJBody.AddMember("KneeRightRot", RotatorMaker(MyInstance->KneeRight, doc), doc.GetAllocator());
		OJBody.AddMember("FootRightRot", RotatorMaker(MyInstance->FootRight, doc), doc.GetAllocator());
		OJBody.AddMember("LeftElbowLoc", VectorMaker(MyInstance->LeftElbowLocation, doc), doc.GetAllocator());
		OJBody.AddMember("RightElbowLoc", VectorMaker(MyInstance->RightElbowLocation, doc), doc.GetAllocator());
		OJBody.AddMember("HipLocationOffset", VectorMaker(MyInstance->HipLocationOffset, doc), doc.GetAllocator());
		OJ.AddMember("BodyPose", OJBody, doc.GetAllocator());

		// Left Hand Pose
		Value OJLeftHand(kObjectType);
		OJLeftHand.AddMember("TrackLeft", (double)MyInstance->TrackLeft, doc.GetAllocator());
		OJLeftHand.AddMember("LeftHandLoc", VectorMaker(MyInstance->LeftHandLocation, doc), doc.GetAllocator());
		OJLeftHand.AddMember("LeftElbowRot", RotatorMaker(MyInstance->ElbowLeft, doc), doc.GetAllocator());
		OJLeftHand.AddMember("LeftWristRot", RotatorMaker(MyInstance->WristLeft, doc), doc.GetAllocator());
		OJLeftHand.AddMember("LeftThumbProxRot", RotatorMaker(MyInstance->ThumbProxLeft, doc), doc.GetAllocator());
		OJLeftHand.AddMember("LeftThumbInterRot", RotatorMaker(MyInstance->ThumbInterLeft, doc), doc.GetAllocator());
		OJLeftHand.AddMember("LeftThumbDistRot", RotatorMaker(MyInstance->ThumbDistLeft, doc), doc.GetAllocator());
		OJLeftHand.AddMember("LeftIndexProxRot", RotatorMaker(MyInstance->IndexProxLeft, doc), doc.GetAllocator());
		OJLeftHand.AddMember("LeftIndexInterRot", RotatorMaker(MyInstance->IndexInterLeft, doc), doc.GetAllocator());
		OJLeftHand.AddMember("LeftIndexDistRot", RotatorMaker(MyInstance->IndexDistLeft, doc), doc.GetAllocator());
		OJLeftHand.AddMember("LeftMiddleProxRot", RotatorMaker(MyInstance->MiddleProxLeft, doc), doc.GetAllocator());
		OJLeftHand.AddMember("LeftMiddleInterRot", RotatorMaker(MyInstance->MiddleInterLeft, doc), doc.GetAllocator());
		OJLeftHand.AddMember("LeftMiddleDistRot", RotatorMaker(MyInstance->MiddleDistLeft, doc), doc.GetAllocator());
		OJLeftHand.AddMember("LeftRingProxRot", RotatorMaker(MyInstance->RingProxLeft, doc), doc.GetAllocator());
		OJLeftHand.AddMember("LeftRingInterRot", RotatorMaker(MyInstance->RingInterLeft, doc), doc.GetAllocator());
		OJLeftHand.AddMember("LeftRingDistRot", RotatorMaker(MyInstance->RingDistLeft, doc), doc.GetAllocator());
		OJLeftHand.AddMember("LeftPinkyProxRot", RotatorMaker(MyInstance->PinkyProxLeft, doc), doc.GetAllocator());
		OJLeftHand.AddMember("LeftPinkyInterRot", RotatorMaker(MyInstance->PinkyInterLeft, doc), doc.GetAllocator());
		OJLeftHand.AddMember("LeftPinkyDistRot", RotatorMaker(MyInstance->PinkyDistLeft, doc), doc.GetAllocator());
		Value OJLeftGrab(kObjectType);
		OJLeftGrab.AddMember("LeftGrab", LeftGrab, doc.GetAllocator());
		OJLeftGrab.AddMember("LeftRelease", LeftRelease, doc.GetAllocator());
		OJ.AddMember("LeftHandPose", OJLeftHand, doc.GetAllocator());
		OJ.AddMember("LeftHandGrab", OJLeftGrab, doc.GetAllocator());


		// Right Hand Pose
		Value OJRightHand(kObjectType);
		OJRightHand.AddMember("TrackRight", (double)MyInstance->TrackRight, doc.GetAllocator());
		OJRightHand.AddMember("RightHandLoc", VectorMaker(MyInstance->RightHandLocation, doc), doc.GetAllocator());
		OJRightHand.AddMember("RightElbowRot", RotatorMaker(MyInstance->ElbowRight, doc), doc.GetAllocator());
		OJRightHand.AddMember("RightWristRot", RotatorMaker(MyInstance->WristRight, doc), doc.GetAllocator());
		OJRightHand.AddMember("RightThumbProxRot", RotatorMaker(MyInstance->ThumbProxRight, doc), doc.GetAllocator());
		OJRightHand.AddMember("RightThumbInterRot", RotatorMaker(MyInstance->ThumbInterRight, doc), doc.GetAllocator());
		OJRightHand.AddMember("RightThumbDistRot", RotatorMaker(MyInstance->ThumbDistRight, doc), doc.GetAllocator());
		OJRightHand.AddMember("RightIndexProxRot", RotatorMaker(MyInstance->IndexProxRight, doc), doc.GetAllocator());
		OJRightHand.AddMember("RightIndexInterRot", RotatorMaker(MyInstance->IndexInterRight, doc), doc.GetAllocator());
		OJRightHand.AddMember("RightIndexDistRot", RotatorMaker(MyInstance->IndexDistRight, doc), doc.GetAllocator());
		OJRightHand.AddMember("RightMiddleProxRot", RotatorMaker(MyInstance->MiddleProxRight, doc), doc.GetAllocator());
		OJRightHand.AddMember("RightMiddleInterRot", RotatorMaker(MyInstance->MiddleInterRight, doc), doc.GetAllocator());
		OJRightHand.AddMember("RightMiddleDistRot", RotatorMaker(MyInstance->MiddleDistRight, doc), doc.GetAllocator());
		OJRightHand.AddMember("RightRingProxRot", RotatorMaker(MyInstance->RingProxRight, doc), doc.GetAllocator());
		OJRightHand.AddMember("RightRingInterRot", RotatorMaker(MyInstance->RingInterRight, doc), doc.GetAllocator());
		OJRightHand.AddMember("RightRingDistRot", RotatorMaker(MyInstance->RingDistRight, doc), doc.GetAllocator());
		OJRightHand.AddMember("RightPinkyProxRot", RotatorMaker(MyInstance->PinkyProxRight, doc), doc.GetAllocator());
		OJRightHand.AddMember("RightPinkyInterRot", RotatorMaker(MyInstance->PinkyInterRight, doc), doc.GetAllocator());
		OJRightHand.AddMember("RightPinkyDistRot", RotatorMaker(MyInstance->PinkyDistRight, doc), doc.GetAllocator());
		Value OJRightGrab(kObjectType);
		OJRightGrab.AddMember("RightGrab", RightGrab, doc.GetAllocator());
		OJRightGrab.AddMember("RightRelease", RightRelease, doc.GetAllocator());
		OJ.AddMember("RightHandPose", OJRightHand, doc.GetAllocator());
		OJ.AddMember("RightHandGrab", OJRightGrab, doc.GetAllocator());
	}	

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	OJ.Accept(writer);
	std::string str = buffer.GetString();
	FString data(str.c_str());

	PoseData += data+"\n";
}