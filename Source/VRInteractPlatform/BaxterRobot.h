// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseRobot.h"
#include "BaxterRobot.generated.h"

/**
 * 
 */
UCLASS()
class VRINTERACTPLATFORM_API ABaxterRobot : public ABaseRobot
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaxterRobot();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SM Assets")
        USceneComponent* SceneComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SM Assets")
        UStaticMeshComponent* SM_Base;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SM Assets")
        UStaticMeshComponent* SM_Torso;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SM Assets")
        UStaticMeshComponent* SM_Head;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SM Assets")
        UStaticMeshComponent* SM_Screen;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SM Assets")
        UStaticMeshComponent* SM_LArmMount;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SM Assets")
        UStaticMeshComponent* SM_LUShoulder;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SM Assets")
        UStaticMeshComponent* SM_LLShoulder;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SM Assets")
        UStaticMeshComponent* SM_LUElbow;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SM Assets")
        UStaticMeshComponent* SM_LLElbow;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SM Assets")
        UStaticMeshComponent* SM_LUForearm;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SM Assets")
        UStaticMeshComponent* SM_LLForearm;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SM Assets")
        UStaticMeshComponent* SM_LWrist;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SM Assets")
        UStaticMeshComponent* SM_RArmMount;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SM Assets")
        UStaticMeshComponent* SM_RUShoulder;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SM Assets")
        UStaticMeshComponent* SM_RLShoulder;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SM Assets")
        UStaticMeshComponent* SM_RUElbow;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SM Assets")
        UStaticMeshComponent* SM_RLElbow;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SM Assets")
        UStaticMeshComponent* SM_RUForearm;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SM Assets")
        UStaticMeshComponent* SM_RLForearm;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SM Assets")
        UStaticMeshComponent* SM_RWrist;

    // UPROPERTY(EditAnywhere, Category = "SM Assets")
	//     UStaticMeshComponent* SM_LGripperBase;

	// UPROPERTY(EditAnywhere, Category = "SM Assets")
	//     UStaticMeshComponent* SM_LGripperTipL;

	// UPROPERTY(EditAnywhere, Category = "SM Assets")
	//     UStaticMeshComponent* SM_LGripperTipR;

	// UPROPERTY(EditAnywhere, Category = "SM Assets")
	//     UStaticMeshComponent* SM_RGripperBase;

	// UPROPERTY(EditAnywhere, Category = "SM Assets")
	//     UStaticMeshComponent* SM_RGripperTipL;

	// UPROPERTY(EditAnywhere, Category = "SM Assets")
	//     UStaticMeshComponent* SM_RGripperTipR;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void LaunchAnimation() override;

	virtual void ParseRobotData() override;

	void BaxterAnimation(const Value& SegArr);

private:

	TMap<FString, UStaticMeshComponent*> NameToMeshMap;
	

	void InitializeComponent();

	void InitializeTransMap(UStaticMeshComponent** c,
        	const TCHAR* name,
        	const TCHAR* mesh_path);
};
