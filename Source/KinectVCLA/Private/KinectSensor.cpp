#include "KinectVCLAPrivatePCH.h"
#include "KinectSensor.h"

#include "AllowWindowsPlatformTypes.h"

#define BODY_WAIT_OBJECT WAIT_OBJECT_0




FKinectSensor::FKinectSensor()
{
	bRun = false;
	Thread = nullptr;

}


FKinectSensor::~FKinectSensor()
{
	
}

bool FKinectSensor::Init()
{
	HRESULT hr;
	

	hr = GetDefaultKinectSensor(&pKinectSensor);

	if (FAILED(hr))
	{

		return false;
	}
	pKinectSensor->Open();

	IBodyFrameSource* pBodyFrameSource = nullptr;
	hr = pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);

	if (FAILED(hr))
	{

		return false;
	}

	hr = pBodyFrameSource->OpenReader(&pBodyFrameReader);
	SAFE_RELEASE(pBodyFrameSource);

	if (FAILED(hr))
	{

		return false;
	}
		

	hr = pBodyFrameReader->SubscribeFrameArrived(&BodyFrameHandle);

	if (FAILED(hr))
	{

		return false;
	}

	//catch (HRESULT hr)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("%d"), (uint32) hr);
	//	return false;	
	//}

	bRun = true;
	return true;
}

uint32 FKinectSensor::Run()
{
	while (bRun)
	{
		HANDLE WaitForBodyHandle = reinterpret_cast<HANDLE>(BodyFrameHandle);
		DWORD WaitResult = WaitForSingleObject(WaitForBodyHandle, 35);
		switch (WaitResult)
		{
		case BODY_WAIT_OBJECT:
		{
			IBodyFrameArrivedEventArgs* pArgs = nullptr;
			HRESULT hr = pBodyFrameReader->GetFrameArrivedEventData(BodyFrameHandle, &pArgs);
			if (SUCCEEDED(hr))
			{
				ProcessBodyFrame(pArgs);
			}
			SAFE_RELEASE(pArgs);
			break;
		}
		case WAIT_TIMEOUT:
		{
			//UE_LOG(LogTemp, Warning, TEXT("wait timed out"));
			break;
		}
		case WAIT_FAILED:
		{
			//UE_LOG(LogTemp, Warning, TEXT("wait failed: %d"), (uint32)GetLastError());
			break;
		}
		default:
		{
			//UE_LOG(LogTemp, Warning, TEXT("well something weird happened: %d"), (uint32)WaitResult);
			break;
		}
		}
	}
	return 0;
}

void FKinectSensor::Stop()
{
	bRun = false;
}

void FKinectSensor::Exit()
{
	pBodyFrameReader->UnsubscribeFrameArrived(BodyFrameHandle);
	SAFE_RELEASE(pBodyFrameReader);

	if (pKinectSensor)
	{
		pKinectSensor->Close();
		SAFE_RELEASE(pKinectSensor);
	}
}

void FKinectSensor::StartKinect()
{
	if (!Thread)
	{
		Thread = FRunnableThread::Create(this, TEXT("KinectSensor"));
	}
}

void FKinectSensor::StopKinect()
{
	if (Thread)
	{
		Thread->Kill(true);
		delete Thread;
		Thread = nullptr;
	}
	bNewBodyFrame = false;
}

//void FKinectSensor::SafeCall(HRESULT HrResult)
//{
//	if (FAILED(HrResult))
//	{
//		throw HrResult;
//	}
//}

void FKinectSensor::ProcessBodyFrame(IBodyFrameArrivedEventArgs* pArgs)
{
	IBodyFrameReference* pBodyFrameReference;
	HRESULT hr;

	hr = pArgs->get_FrameReference(&pBodyFrameReference);
	if (SUCCEEDED(hr))
	{
		IBodyFrame* pBodyFrame;

		hr = pBodyFrameReference->AcquireFrame(&pBodyFrame);
		if (SUCCEEDED(hr))
		{
			IBody* bodies[BODY_COUNT] = { 0 };
			FScopeLock lock(&BodyCriticalSection);
			hr = pBodyFrame->GetAndRefreshBodyData(_countof(bodies), bodies);
			BOOLEAN bIsTracked;
			for (int i = 0; i < _countof(bodies); i++)
			{
				bodies[i]->get_IsTracked(&bIsTracked);
				if (bIsTracked)
				{
					ActiveBody = FBody(bodies[i]);
					bNewBodyFrame = true;
					//UE_LOG(LogTemp, Warning, TEXT("body processed"));
					break;
				}
			}

			for (int i = 0; i <_countof(bodies); i++)
			{
				SAFE_RELEASE(bodies[i]);
			}
		}

		SAFE_RELEASE(pBodyFrame)
	}

	SAFE_RELEASE(pBodyFrameReference);
}

bool FKinectSensor::GetBodyInfo(FBody& OutBody)
{
	FScopeLock lock(&BodyCriticalSection);
	if (bNewBodyFrame)
	{
		OutBody = ActiveBody;
		bNewBodyFrame = false;
		return true;
	}
	return false;
}

#include "HideWindowsPlatformTypes.h"
