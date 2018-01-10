// Fill out your copyright notice in the Description page of Project Settings.

#include "VRInteractPlatform.h"
#include "CustomGrabInterface.h"
#include "AnimateActor.h"

static Document ParsedDoc(const Value &obj)
{
	Document d;
	StringBuffer buf;
	Writer<StringBuffer> writer(buf);
	
	obj.Accept(writer);
	std::string objStr = buf.GetString();
	d.Parse(objStr.c_str());

	return d;
}

static FRotator RotatorMaker(const Value &rotator)
{
	Document d = ParsedDoc(rotator);
	double pitch = d["Pitch"].GetDouble();
	double yaw = d["Yaw"].GetDouble();
	double roll = d["Roll"].GetDouble();

	return FRotator(pitch, yaw, roll);
}

static FVector VectorMaker(const Value &vector)
{
	Document d = ParsedDoc(vector);
	double x = d["X"].GetDouble();
	double y = d["Y"].GetDouble();
	double z = d["Z"].GetDouble();

	return FVector(x, y, z);
}

// Sets default values
AAnimateActor::AAnimateActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));

	HeadOffset = CreateDefaultSubobject<USceneComponent>(TEXT("HeadOffset"));
	CameraView = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	BodyMesh->SetupAttachment(RootComponent);

	HeadOffset->SetupAttachment(BodyMesh);
	CameraView->SetupAttachment(HeadOffset);
	HeadMesh->SetupAttachment(CameraView);

	RecordApplied = 0;
	UpdateInterval = .001;

	RecordTrigger = false;
	RecordSeg = "";
}

// Called when the game starts or when spawned
void AAnimateActor::BeginPlay()
{
	Super::BeginPlay();
	
	FString GameDir = FPaths::GameDir();
    FString FileName = GameDir+"HumanPoseRecord.csv";

    if(FPaths::FileExists(FileName))
    {
    	FString FileData = "";
    	FFileHelper::LoadFileToString(FileData, *FileName);

    	int32 FieldCount = FileData.ParseIntoArray(ApplyPoseArray, TEXT("\n"), true);
    	UE_LOG(LogTemp, Warning, TEXT("record count: %d"), FieldCount);
    }

    if(BodyMesh)
    	AnimInstance = Cast<UVRAnimInstance>(BodyMesh->GetAnimInstance());

    FTimerHandle ReceiverHandler;
    GetWorldTimerManager().SetTimer(ReceiverHandler, this, &AAnimateActor::UpdateAnim, UpdateInterval, true);
}

void AAnimateActor::BeginDestroy()
{
	Super::BeginDestroy();

	FString GameDir = FPaths::GameDir();
    FString FileName = GameDir+"HumanPoseRecordSeg.csv";

    FFileHelper::SaveStringToFile(RecordSeg, *FileName, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get());
}

// Called every frame
void AAnimateActor::Tick( float DeltaTime )
{	
	Super::Tick( DeltaTime );	
}

void AAnimateActor::Grab(bool IsLeft)
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
				// LeftHandCustomGrab.Add(Hit.GetActor());
				// CustomGrabActor->OnGrab(this, IsLeft);
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
				// RightHandCustomGrab.Add(Hit.GetActor());
				// CustomGrabActor->OnGrab(this, IsLeft);
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

void AAnimateActor::Release(bool IsLeft)
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
				// CustomInterfaceRef->OnRelease(this, IsLeft);
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
				// CustomInterfaceRef->OnRelease(this, IsLeft);
			}
		}
		RightHandCustomGrab.Empty();
	}

	// UE_LOG(LogTemp, Warning, TEXT("Releasing"));
}

void AAnimateActor::UpdateAnim()
{
	if(RecordApplied < ApplyPoseArray.Num())
	{
		FString record = ApplyPoseArray[RecordApplied];
       	std::string json_data(TCHAR_TO_UTF8(*record));
       	Document doc;
        doc.Parse(json_data.c_str());
        UpdateActor(doc);

        if(AnimInstance)
        {
        	UpdateBody(doc);
        	UpdateHand(doc);
        }

        if(RecordTrigger)
        	RecordSeg += record+"\n";

        RecordApplied++;
	}
}

void AAnimateActor::UpdateActor(Document &doc)
{
	// Actor Pose;

	Document d = ParsedDoc(doc["ActorPose"].GetObject());
	
	SetActorRotation(RotatorMaker(d["ActorRot"].GetObject()));
	SetActorLocation(VectorMaker(d["ActorLoc"].GetObject()));
}

void AAnimateActor::UpdateBody(Document &doc)
{
	// Body Animation

	Document d = ParsedDoc(doc["BodyPose"].GetObject());

	AnimInstance->SpineBase = RotatorMaker(d["SpineBaseRot"].GetObject());
	AnimInstance->SpineMid = RotatorMaker(d["SpineMidRot"].GetObject());
	AnimInstance->SpineTop = RotatorMaker(d["SpineTopRot"].GetObject());
	AnimInstance->ShoulderLeft = RotatorMaker(d["ShoulderLeftRot"].GetObject());
	AnimInstance->ShoulderRight = RotatorMaker(d["ShoulderRightRot"].GetObject());
	AnimInstance->HipLeft = RotatorMaker(d["HipLeftRot"].GetObject());
	AnimInstance->KneeLeft = RotatorMaker(d["KneeLeftRot"].GetObject());
	AnimInstance->FootLeft = RotatorMaker(d["FootLeftRot"].GetObject());
	AnimInstance->HipRight = RotatorMaker(d["HipRightRot"].GetObject());
	AnimInstance->KneeRight = RotatorMaker(d["KneeRightRot"].GetObject());
	AnimInstance->FootRight = RotatorMaker(d["FootRightRot"].GetObject());
	AnimInstance->LeftElbowLocation = VectorMaker(d["LeftElbowLoc"].GetObject());
	AnimInstance->RightElbowLocation = VectorMaker(d["RightElbowLoc"].GetObject());
	AnimInstance->HipLocationOffset = VectorMaker(d["HipLocationOffset"].GetObject());
}

void AAnimateActor::UpdateHand(Document &doc)
{
	Document d;

	// Left Hand Grab
	d = ParsedDoc(doc["LeftHandGrab"].GetObject());
	if(d["LeftGrab"].GetBool())
		Grab(true);
	if(d["LeftRelease"].GetBool())
		Release(true);

	// Left Hand Animation
	d = ParsedDoc(doc["LeftHandPose"].GetObject());
	// if(d["TrackLeft"].GetDouble() > 0.0)
	{
		AnimInstance->TrackLeft = d["TrackLeft"].GetDouble();
		AnimInstance->LeftHandLocation = VectorMaker(d["LeftHandLoc"].GetObject());
		AnimInstance->ElbowLeft = RotatorMaker(d["LeftElbowRot"].GetObject());
		AnimInstance->WristLeft = RotatorMaker(d["LeftWristRot"].GetObject());
		AnimInstance->ThumbProxLeft = RotatorMaker(d["LeftThumbProxRot"].GetObject());
		AnimInstance->ThumbInterLeft = RotatorMaker(d["LeftThumbInterRot"].GetObject());
		AnimInstance->ThumbDistLeft = RotatorMaker(d["LeftThumbDistRot"].GetObject());
		AnimInstance->IndexProxLeft = RotatorMaker(d["LeftIndexProxRot"].GetObject());
		AnimInstance->IndexInterLeft = RotatorMaker(d["LeftIndexInterRot"].GetObject());
		AnimInstance->IndexDistLeft = RotatorMaker(d["LeftIndexDistRot"].GetObject());
		AnimInstance->MiddleProxLeft = RotatorMaker(d["LeftMiddleProxRot"].GetObject());
		AnimInstance->MiddleInterLeft = RotatorMaker(d["LeftMiddleInterRot"].GetObject());
		AnimInstance->MiddleDistLeft = RotatorMaker(d["LeftMiddleDistRot"].GetObject());
		AnimInstance->RingProxLeft = RotatorMaker(d["LeftRingProxRot"].GetObject());
		AnimInstance->RingInterLeft = RotatorMaker(d["LeftRingInterRot"].GetObject());
		AnimInstance->RingDistLeft = RotatorMaker(d["LeftRingDistRot"].GetObject());
		AnimInstance->PinkyProxLeft = RotatorMaker(d["LeftPinkyProxRot"].GetObject());
		AnimInstance->PinkyInterLeft = RotatorMaker(d["LeftPinkyInterRot"].GetObject());
		AnimInstance->PinkyDistLeft = RotatorMaker(d["LeftPinkyDistRot"].GetObject());
	}

	// Right Hand Grab
	d = ParsedDoc(doc["RightHandGrab"].GetObject());
	if(d["RightGrab"].GetBool())
		Grab(false);
	if(d["RightRelease"].GetBool())
		Release(false);

	// Right Hand Animation
	d = ParsedDoc(doc["RightHandPose"].GetObject());
	// if(d["TrackRight"].GetDouble() > 0.0)
	{
		AnimInstance->TrackRight = d["TrackRight"].GetDouble();
		AnimInstance->RightHandLocation = VectorMaker(d["RightHandLoc"].GetObject());
		AnimInstance->ElbowRight = RotatorMaker(d["RightElbowRot"].GetObject());
		AnimInstance->WristRight = RotatorMaker(d["RightWristRot"].GetObject());
		AnimInstance->ThumbProxRight = RotatorMaker(d["RightThumbProxRot"].GetObject());
		AnimInstance->ThumbInterRight = RotatorMaker(d["RightThumbInterRot"].GetObject());
		AnimInstance->ThumbDistRight = RotatorMaker(d["RightThumbDistRot"].GetObject());
		AnimInstance->IndexProxRight = RotatorMaker(d["RightIndexProxRot"].GetObject());
		AnimInstance->IndexInterRight = RotatorMaker(d["RightIndexInterRot"].GetObject());
		AnimInstance->IndexDistRight = RotatorMaker(d["RightIndexDistRot"].GetObject());
		AnimInstance->MiddleProxRight = RotatorMaker(d["RightMiddleProxRot"].GetObject());
		AnimInstance->MiddleInterRight = RotatorMaker(d["RightMiddleInterRot"].GetObject());
		AnimInstance->MiddleDistRight = RotatorMaker(d["RightMiddleDistRot"].GetObject());
		AnimInstance->RingProxRight = RotatorMaker(d["RightRingProxRot"].GetObject());
		AnimInstance->RingInterRight = RotatorMaker(d["RightRingInterRot"].GetObject());
		AnimInstance->RingDistRight = RotatorMaker(d["RightRingDistRot"].GetObject());
		AnimInstance->PinkyProxRight = RotatorMaker(d["RightPinkyProxRot"].GetObject());
		AnimInstance->PinkyInterRight = RotatorMaker(d["RightPinkyInterRot"].GetObject());
		AnimInstance->PinkyDistRight = RotatorMaker(d["RightPinkyDistRot"].GetObject());
	}
}

void AAnimateActor::RecordTriggerFunc()
{
	RecordTrigger = !RecordTrigger;
}