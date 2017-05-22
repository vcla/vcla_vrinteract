// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "LeapController.h"
#include "LeapEnums.h"
#include "LeapEventInterface.h"
#include "HandObject.h"
#include "BasePawn.h"
#include "LeapPawn.generated.h"

/**
* Pawn with leap motion used for fine control of the hands
*/
UCLASS()
class VRINTERACTPLATFORM_API ALeapPawn : public ABasePawn, public ILeapEventInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties

	ALeapPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Sets Pawn's internal values based on the user's current real world position
	virtual void CalibratePawn() override;

	UPROPERTY(EditDefaultsOnly, Category = "VRPawn")
	ULeapController* LeapController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avateering")
		UHandObject* LeftHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avateering")
		UHandObject* RightHand;

	//skeleton dependent rotator, represents additional offset skeleton hand joint needed to align with Leap Motion's axis
	UPROPERTY(EditDefaultsOnly, Category = "Avateering")
		FRotator LeftHandNeutralOffset;

	UPROPERTY(EditDefaultsOnly, Category = "Avateering")
		FRotator RightHandNeutralOffset;

	//Value Leap grab value needs to cross in order to be considered a grab
	UPROPERTY(EditDefaultsOnly, Category = "Avateering")
		float GrabThreshold;

	//Interaction functions and properties

	void FireGrabEvents(UHandObject* Hand);

	void UpdateHandVariables();

};
