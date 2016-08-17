#pragma once

#include "Core.h"
#include "KinectStructs.h"
#include "AllowWindowsPlatformTypes.h"
#include "Kinect.h"

class KINECTVCLA_API FKinectSensor : public FRunnable
{
public:
	FKinectSensor();
	~FKinectSensor();

	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();

	virtual void Exit();

	void StartKinect();
	void StopKinect();

	bool GetBodyInfo(FBody& OutBody);

private:
	bool bRun;

	IKinectSensor* pKinectSensor;
	IBodyFrameReader* pBodyFrameReader;

	FCriticalSection BodyCriticalSection;

	//IBody* bodies[BODY_COUNT];
	FBody ActiveBody;
	bool bNewBodyFrame;

	WAITABLE_HANDLE BodyFrameHandle;

	FRunnableThread* Thread;

	//Helper to make sure our Kinect functions arent failing
	//void SafeCall(HRESULT HrResult);

	void ProcessBodyFrame(IBodyFrameArrivedEventArgs* pArgs);

};

#include "HideWindowsPlatformTypes.h"