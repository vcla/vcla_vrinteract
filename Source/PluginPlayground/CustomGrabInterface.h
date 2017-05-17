// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasePawn.h"
#include "CustomGrabInterface.generated.h"

/**
 * Interface for when after being grabbed an object needs to do something other than get attached to the hand
 */
UINTERFACE(BlueprintType)
class PLUGINPLAYGROUND_API UCustomGrabInterface : public UInterface
{
	GENERATED_BODY()
	
	
	
	
};

class PLUGINPLAYGROUND_API ICustomGrabInterface
{
	GENERATED_BODY()

public:

	UFUNCTION()
		virtual void OnGrab(ABasePawn* PawnRef, bool IsLeft)=0;

	UFUNCTION()
		virtual void OnRelease(ABasePawn* PawnRef, bool IsLeft)=0;

};