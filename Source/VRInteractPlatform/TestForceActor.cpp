// Fill out your copyright notice in the Description page of Project Settings.

#include "VRInteractPlatform.h"
#include "TestForceActor.h"


// Sets default values
ATestForceActor::ATestForceActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GrabbingPawn = NULL;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshObject"));
	MeshComponent->SetSimulatePhysics(true);
	ForceMagnitude = 15.f;
}

// Called when the game starts or when spawned
void ATestForceActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestForceActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (GrabbingPawn)
	{
		FVector CurrentWorldLocation = MeshComponent->GetComponentLocation() + RelativeGrabPoint;
		FVector ForceVector = GrabbingPawn->BodyMesh->GetBoneLocation(HandName, EBoneSpaces::WorldSpace) - CurrentWorldLocation;
		MeshComponent->AddForceAtLocation(ForceMagnitude * ForceVector,  CurrentWorldLocation);
	}

}

void ATestForceActor::OnGrab(ABasePawn* PawnRef, bool IsLeft)
{
	GrabbingPawn = PawnRef;
	HandName = IsLeft ? PawnRef->LeftHandAttachPoint : PawnRef->RightHandAttachPoint;
	RelativeGrabPoint = GrabbingPawn->BodyMesh->GetBoneLocation(HandName, EBoneSpaces::WorldSpace)-MeshComponent->GetComponentLocation();
	//UE_LOG(LogTemp, Warning, TEXT("CUSTOM GRAB IS HAPPENING???"));
}

void ATestForceActor::OnRelease(ABasePawn* PawnRef, bool IsLeft)
{
	GrabbingPawn = NULL;
	//UE_LOG(LogTemp, Warning, TEXT("CUSTOM RELEASING???"));
}