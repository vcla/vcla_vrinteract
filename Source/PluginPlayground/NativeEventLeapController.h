// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LeapController.h"
#include "NativeEventLeapController.generated.h"

/**
 * Hack to provide hooks for C++ events without having to implement Blueprint or modify the original plugin
 */
UCLASS()
class PLUGINPLAYGROUND_API UNativeEventLeapController : public ULeapController
{
	GENERATED_BODY()
	
	
	
	
};
