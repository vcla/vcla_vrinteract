// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "LeapController.h"
#include "LeapEnums.h"
#include "LeapEventInterface.h"
#include "HandObject.h"
#include "BasePawn.h"
#include "VRPawn.generated.h"


UCLASS()
class PLUGINPLAYGROUND_API AVRPawn : public ABasePawn, public ILeapEventInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVRPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	//virtual void LeapHandMoved_Implementation(ULeapHand* hand) override;

	// Sets Pawn's internal values based on the user's current real world position
	virtual void CalibratePawn() override;

	////Avateering functions and properties
	//FTransform GetConvertedTransform(FName BoneName);

	//UPROPERTY(EditAnywhere, Category = "Avateering")
	//	TArray<FAvatarBoneInfo> BoneInfoArray;
	//
	//UPROPERTY(EditDefaultsOnly, Category = "VRPawn")
	//USkeletalMeshComponent* BodyMesh;

	//UPROPERTY(EditDefaultsOnly, Category = "VRPawn")
	//USkeletalMeshComponent* HeadMesh;

	//UPROPERTY(EditDefaultsOnly, Category = "VRPawn")
	//USceneComponent* HeadOffset;

	//UPROPERTY(EditDefaultsOnly, Category = "VRPawn")
	//UCameraComponent* CameraView;

	UPROPERTY(EditDefaultsOnly, Category = "VRPawn")
	ULeapController* LeapController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avateering")
		UHandObject* LeftHand;

	UPROPERTY(EditDefaultsOnly, Category = "Avateering")
		FRotator LeftHandNeutralOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avateering")
		UHandObject* RightHand;

	UPROPERTY(EditDefaultsOnly, Category = "Avateering")
		FRotator RightHandNeutralOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Avateering")
		float GrabThreshold;

	//Interaction functions and properties

	UInteractableBaseComponent* RightHandObject;
	UInteractableBaseComponent* LeftHandObject;

	//void Grab(UHandObject* GrabbingHand);
	//void Release(UHandObject* ReleasingHand);

	void FireGrabEvents(UHandObject* Hand);

	void UpdateHandVariables();

};
