// Fill out your copyright notice in the Description page of Project Settings.

#include "VRInteractPlatform.h"
#include "BaxterRobot.h"

#ifdef UB_SCALE_CONSTANT
#undef UB_SCALE_CONSTANT
#endif
#define UB_SCALE_CONSTANT 1.0f//100.0f

static FVector VectorSwitchY(FVector ru)	//vector right hand to left hand 
{
	return FVector(ru.X, -(ru.Y), ru.Z);
}

static FQuat QuaternionSwitchY(FQuat rq)	//quaternion right hand to left hand
{
	return FQuat(rq.X, -(rq.Y), rq.Z, -(rq.W));
}

static void LoadInMesh(UStaticMeshComponent** c, const TCHAR* path) {
    UStaticMesh* pMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, path));
    if (pMesh == NULL) {
        UE_LOG(LogTemp, Warning, TEXT("Can't find the mesh"));
    }
    if (!(*c)->SetStaticMesh(pMesh)) {
        UE_LOG(LogTemp, Warning, TEXT("Fail setting the mesh"));
    }
}

void ABaxterRobot::InitializeTransMap(UStaticMeshComponent** c, const TCHAR* name, const TCHAR* mesh_path)
{
	UE_LOG(LogTemp, Warning, TEXT("Start initializing frame for %s"), name);
    // Load in the mesh;
    LoadInMesh(c, mesh_path);

    NameToMeshMap.Emplace(name, *c);
}

void ABaxterRobot::BaxterAnimation(const Value& SegArr)
{
	for(SizeType i=0; i<SegArr.Size(); i++)
	{
		Document doc = ParsedDoc(SegArr[i]);
		std::string MeshName = doc["MeshName"].GetString();
		FString FMeshName(MeshName.c_str());

		Document d = ParsedDoc(doc["MeshPose"].GetObject());
		FQuat rot = QuatMaker(d["Rot"].GetObject());
		FVector loc = VectorMaker(d["Loc"].GetObject());

		UStaticMeshComponent* mesh = NameToMeshMap[FMeshName];
		mesh->SetRelativeLocation(VectorSwitchY(loc*UB_SCALE_CONSTANT));
		mesh->SetRelativeRotation(QuaternionSwitchY(rot));
	}
}

ABaxterRobot::ABaxterRobot()
{
	PrimaryActorTick.bCanEverTick = true;

	InitializeComponent();
	RootComponent = SM_Base;
}

void ABaxterRobot::InitializeComponent()
{
	SM_Base = CreateDefaultSubobject<UStaticMeshComponent>("base");
    InitializeTransMap(&SM_Base,
        TEXT("base"),
        TEXT("StaticMesh'/Game/Assets/baxter/PEDESTAL.PEDESTAL'")
        );
	// Attach base to SceneComponent.
    SM_Base->SetupAttachment(SceneComponent);
    SM_Base->SetWorldScale3D(FVector(1.0f)*100.0f);

    SM_Torso = CreateDefaultSubobject<UStaticMeshComponent>("torso");
    InitializeTransMap(&SM_Torso,
        TEXT("torso"),
        TEXT("StaticMesh'/Game/Assets/baxter/base_link.base_link'")
        );
    // Set the attachment mode.
    SM_Torso->SetupAttachment(SM_Base);

    // Now for head.
    SM_Head = CreateDefaultSubobject<UStaticMeshComponent>("head");
    InitializeTransMap(&SM_Head,
        TEXT("head"),
        TEXT("StaticMesh'/Game/Assets/baxter/H0.H0'")
        );
    // Set the attachment mode.
    SM_Head->SetupAttachment(SM_Torso);

    // Now for screen.
    SM_Screen = CreateDefaultSubobject<UStaticMeshComponent>("screen");
    InitializeTransMap(&SM_Screen,
        TEXT("screen"),
        TEXT("StaticMesh'/Game/Assets/baxter/H1.H1'")
        );
    // Set the attachment mode.
    SM_Screen->SetupAttachment(SM_Head);

    // Now for left arm mount.
    SM_LArmMount = CreateDefaultSubobject<UStaticMeshComponent>("left_arm_mount");
    InitializeTransMap(&SM_LArmMount,
        TEXT("left_arm_mount"),
        TEXT("")
        );
    // Set the attachment mode.
    SM_LArmMount->SetupAttachment(SM_Torso);

    // Now for left upper shoulder.
    SM_LUShoulder = CreateDefaultSubobject<UStaticMeshComponent>("left_upper_shoulder");
    InitializeTransMap(&SM_LUShoulder,
        TEXT("left_upper_shoulder"),
        TEXT("StaticMesh'/Game/Assets/baxter/S0.S0'")
        );
    // Set the attachment mode.
    SM_LUShoulder->SetupAttachment(SM_LArmMount);

    // Now for left lower shoulder.
    SM_LLShoulder = CreateDefaultSubobject<UStaticMeshComponent>("left_lower_shoulder");
    InitializeTransMap(&SM_LLShoulder,
        TEXT("left_lower_shoulder"),
        TEXT("StaticMesh'/Game/Assets/baxter/S1.S1'")
        );
    // Set the attachment mode.
    SM_LLShoulder->SetupAttachment(SM_LUShoulder);

    // Now for left upper elbow.
    SM_LUElbow = CreateDefaultSubobject<UStaticMeshComponent>("left_upper_elbow");
    InitializeTransMap(&SM_LUElbow,
        TEXT("left_upper_elbow"),
        TEXT("StaticMesh'/Game/Assets/baxter/E0.E0'")
        );
    // Set the attachment mode.
    SM_LUElbow->SetupAttachment(SM_LLShoulder);

    // Now for left lower elbow.
    SM_LLElbow = CreateDefaultSubobject<UStaticMeshComponent>("left_lower_elbow");
    InitializeTransMap(&SM_LLElbow,
        TEXT("left_lower_elbow"),
        TEXT("StaticMesh'/Game/Assets/baxter/E1.E1'")
        );
    // Set the attachment mode.
    SM_LLElbow->SetupAttachment(SM_LUElbow);

    // Now for left upper forearm.
    SM_LUForearm = CreateDefaultSubobject<UStaticMeshComponent>("left_upper_forearm");
    InitializeTransMap(&SM_LUForearm,
        TEXT("left_upper_forearm"),
        TEXT("StaticMesh'/Game/Assets/baxter/W0.W0'")
        );
    // Set the attachment mode.
    SM_LUForearm->SetupAttachment(SM_LLElbow);

    // Now for left lower forearm.
    SM_LLForearm = CreateDefaultSubobject<UStaticMeshComponent>("left_lower_forearm");
    InitializeTransMap(&SM_LLForearm,
        TEXT("left_lower_forearm"),
        TEXT("StaticMesh'/Game/Assets/baxter/W1.W1'")
        );
    // Set the attachment mode.
    SM_LLForearm->SetupAttachment(SM_LUForearm);

    // Now for left wrist.
    SM_LWrist = CreateDefaultSubobject<UStaticMeshComponent>("left_wrist");
    InitializeTransMap(&SM_LWrist,
        TEXT("left_wrist"),
        TEXT("StaticMesh'/Game/Assets/baxter/W2.W2'")
        );
    // Set the attachment mode.
    SM_LWrist->SetupAttachment(SM_LLForearm);

    // // Now for left gripper base.
    // SM_LGripperBase = CreateDefaultSubobject<UStaticMeshComponent>("left_gripper_base");
    // InitializeTransMap(&SM_LGripperBase,
    // 	TEXT("left_gripper_base"),
    // 	TEXT("StaticMesh'/Game/gripper/electric_gripper_base.electric_gripper_base'")
    // 	); 
    // // Set the attachment mode.
    // SM_LGripperBase->SetupAttachment(SM_LWrist);
    // SM_LGripperBase->SetRelativeRotation(FRotator(0, 180, 90));
    // SM_LGripperBase->SetRelativeLocation(FVector(-0.01, 0, 0.14));

    // // Now for left gripper left tip.
    // SM_LGripperTipL = CreateDefaultSubobject<UStaticMeshComponent>("left_gripper_left_tip");
    // InitializeTransMap(&SM_LGripperTipL,
    // 	TEXT("left_gripper_left_tip"),
    // 	TEXT("StaticMesh'/Game/gripper/standard_narrow.standard_narrow'")
    // 	);
    // // Set the attachment mode.
    // SM_LGripperTipL->SetupAttachment(SM_LGripperBase);
    // SM_LGripperTipL->SetRelativeRotation(FRotator(0, 0, -90));
    // SM_LGripperTipL->SetRelativeLocation(FVector(0, -0.02, 0.05));

    // // Now for left gripper right tip.
    // SM_LGripperTipR = CreateDefaultSubobject<UStaticMeshComponent>("left_gripper_right_tip");
    // InitializeTransMap(&SM_LGripperTipR,
    // 	TEXT("left_gripper_right_tip"),
    // 	TEXT("StaticMesh'/Game/gripper/standard_narrow.standard_narrow'")
    // 	);
    // // Set the attachment mode.
    // SM_LGripperTipR->SetupAttachment(SM_LGripperBase);
    // SM_LGripperTipR->SetRelativeRotation(FRotator(180, 0, -90));
    // SM_LGripperTipR->SetRelativeLocation(FVector(0, -0.02, -0.05));

    /*************************************************************************************/
    SM_RArmMount = CreateDefaultSubobject<UStaticMeshComponent>("right_arm_mount");
    InitializeTransMap(&SM_RArmMount,
        TEXT("right_arm_mount"),
        TEXT("")
        );
    SM_RArmMount->SetupAttachment(SM_Torso);

    SM_RUShoulder = CreateDefaultSubobject<UStaticMeshComponent>("right_upper_shoulder");
    InitializeTransMap(&SM_RUShoulder,
        TEXT("right_upper_shoulder"),
        TEXT("StaticMesh'/Game/Assets/baxter/S0.S0'")
        );
    SM_RUShoulder->SetupAttachment(SM_RArmMount);

    SM_RLShoulder = CreateDefaultSubobject<UStaticMeshComponent>("right_lower_shoulder");
    InitializeTransMap(&SM_RLShoulder,
        TEXT("right_lower_shoulder"),
        TEXT("StaticMesh'/Game/Assets/baxter/S1.S1'")
        );
    SM_RLShoulder->SetupAttachment(SM_RUShoulder);

    SM_RUElbow = CreateDefaultSubobject<UStaticMeshComponent>("right_upper_elbow");
    InitializeTransMap(&SM_RUElbow,
        TEXT("right_upper_elbow"),
        TEXT("StaticMesh'/Game/Assets/baxter/E0.E0'")
        );
    SM_RUElbow->SetupAttachment(SM_RLShoulder);

    SM_RLElbow = CreateDefaultSubobject<UStaticMeshComponent>("right_lower_elbow");
    InitializeTransMap(&SM_RLElbow,
        TEXT("right_lower_elbow"),
        TEXT("StaticMesh'/Game/Assets/baxter/E1.E1'")
        );
    SM_RLElbow->SetupAttachment(SM_RUElbow);

    SM_RUForearm = CreateDefaultSubobject<UStaticMeshComponent>("right_upper_forearm");
    InitializeTransMap(&SM_RUForearm,
        TEXT("right_upper_forearm"),
        TEXT("StaticMesh'/Game/Assets/baxter/W0.W0'")
        );
    SM_RUForearm->SetupAttachment(SM_RLElbow);

    SM_RLForearm = CreateDefaultSubobject<UStaticMeshComponent>("right_lower_forearm");
    InitializeTransMap(&SM_RLForearm,
        TEXT("right_lower_forearm"),
        TEXT("StaticMesh'/Game/Assets/baxter/W1.W1'")
        );
    SM_RLForearm->SetupAttachment(SM_RUForearm);

    SM_RWrist = CreateDefaultSubobject<UStaticMeshComponent>("right_wrist");
    InitializeTransMap(&SM_RWrist,
        TEXT("right_wrist"),
        TEXT("StaticMesh'/Game/Assets/baxter/W2.W2'")
        );
    SM_RWrist->SetupAttachment(SM_RLForearm);

    // // Now for right gripper base.
    // SM_RGripperBase = CreateDefaultSubobject<UStaticMeshComponent>("right_gripper_base");
    // InitializeTransMap(&SM_RGripperBase,
    // 	TEXT("right_gripper_base"),
    // 	TEXT("StaticMesh'/Game/gripper/electric_gripper_base.electric_gripper_base'")
    // 	); 
    // SM_RGripperBase->SetupAttachment(SM_RWrist);
    // SM_RGripperBase->SetRelativeRotation(FRotator(0, 180, 90));
    // SM_RGripperBase->SetRelativeLocation(FVector(-0.01, 0, 0.14));

    // // Now for right gripper left tip.
    // SM_RGripperTipL = CreateDefaultSubobject<UStaticMeshComponent>("right_gripper_left_tip");
    // InitializeTransMap(&SM_RGripperTipL,
    // 	TEXT("right_gripper_left_tip"),
    // 	TEXT("StaticMesh'/Game/gripper/standard_narrow.standard_narrow'")
    // 	);
    // SM_RGripperTipL->SetupAttachment(SM_RGripperBase);
    // SM_RGripperTipL->SetRelativeRotation(FRotator(0, 0, -90));
    // SM_RGripperTipL->SetRelativeLocation(FVector(0, -0.02, 0.05));

    // // Now for right gripper right tip.
    // SM_RGripperTipR = CreateDefaultSubobject<UStaticMeshComponent>("right_gripper_right_tip");
    // InitializeTransMap(&SM_RGripperTipR,
    // 	TEXT("right_gripper_right_tip"),
    // 	TEXT("StaticMesh'/Game/gripper/standard_narrow.standard_narrow'")
    // 	);
    // SM_RGripperTipR->SetupAttachment(SM_RGripperBase);
    // SM_RGripperTipR->SetRelativeRotation(FRotator(180, 0, -90));
    // SM_RGripperTipR->SetRelativeLocation(FVector(0, -0.02, -0.05));
}

void ABaxterRobot::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle ReceiverHandler;
    GetWorldTimerManager().SetTimer(ReceiverHandler, this, &ABaxterRobot::LaunchAnimation, 0.005f, true);

    UE_LOG(LogTemp, Warning, TEXT("%s"), *GetActorLocation().ToString());
    UE_LOG(LogTemp, Warning, TEXT("%s"), *(SM_Base->GetComponentLocation()).ToString());
}

void ABaxterRobot::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABaxterRobot::LaunchAnimation()
{
	ParseRobotData();
}

void ABaxterRobot::ParseRobotData()
{
	std::string data;
	if(!PendingData.IsEmpty())
	{
		PendingData.Dequeue(data);
		if(PendingData.IsEmpty())
			PendingData.Enqueue(data);
	}
	else return;

	// Parse the current json object, override

	// UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(data.c_str()));
	// Animation
	Document doc;
	doc.Parse(data.c_str());
	BaxterAnimation(doc["Anim"].GetArray());
}