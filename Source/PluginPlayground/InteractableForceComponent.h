// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableBaseComponent.h"
#include "InteractableForceComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PLUGINPLAYGROUND_API UInteractableForceComponent : public UInteractableBaseComponent
{
	GENERATED_BODY()

public:
		virtual void WhileGrabbed_Implementation(UHandObject* HandObject) override;

	
	
};
