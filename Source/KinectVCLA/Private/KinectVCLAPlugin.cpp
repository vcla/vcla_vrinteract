// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "KinectVCLAPrivatePCH.h"
#include "KinectSensor.h"


class FKinectVCLAPlugin : public IKinectVCLAModule
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual void StartSensor() override;
	virtual void StopSensor() override;

	virtual bool GetBody(FBody& OutBody) override;
	
private:
	TSharedPtr<FKinectSensor> KinectSensor;
};

IMPLEMENT_GAME_MODULE( FKinectVCLAPlugin, KinectVCLA)



void FKinectVCLAPlugin::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
	KinectSensor = TSharedPtr<FKinectSensor>(new FKinectSensor());
}


void FKinectVCLAPlugin::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	KinectSensor.Reset();
}

void FKinectVCLAPlugin::StartSensor()
{
	KinectSensor->StartKinect();
}

void FKinectVCLAPlugin::StopSensor()
{
	KinectSensor->StopKinect();
}

bool FKinectVCLAPlugin::GetBody(FBody& OutBody)
{
	return KinectSensor->GetBodyInfo(OutBody);
}