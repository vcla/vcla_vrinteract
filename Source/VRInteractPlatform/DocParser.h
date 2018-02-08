// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "DocParser.generated.h"

using namespace rapidjson;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDocParser : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class VRINTERACTPLATFORM_API IDocParser
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	Document ParsedDoc(const Value &obj);
	FQuat QuatMaker(const Value &quat);
	FVector VectorMaker(const Value &vector);
};
