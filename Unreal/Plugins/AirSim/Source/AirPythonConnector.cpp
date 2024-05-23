// Fill out your copyright notice in the Description page of Project Settings.
#include "AirPythonConnector.h"
#include "Networking/Public/Networking.h"

#include "Engine/World.h"
#include "Engine.h"

//	*******************************************************
//  Helper function to convert FString to an array of bytes
//	*******************************************************
void StringToBytes(const FString& InString, TArray<uint8>& OutBytes, int32 Count)
{
	OutBytes.Empty(Count);
	const ANSICHAR* AnsiData = StringCast<ANSICHAR>(*InString).Get();

	// Append the ANSICHAR data to the output array
	for (int32 i = 0; i < Count; ++i)
	{
		OutBytes.Add(static_cast<uint8>(AnsiData[i]));
	}
}

UAirPythonConnector::UAirPythonConnector()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAirPythonConnector::BeginPlay()
{
	Super::BeginPlay();	

	FIPv4Endpoint endPoint(FIPv4Address(127, 0, 0, 1), _UDPListenPort);

	//Create a UDP socket
	_pListenSocket = FUdpSocketBuilder(TEXT("PyListenSocket"))
		.AsReusable()
		.AsNonBlocking()
		.BoundToEndpoint(endPoint);

	_pListenSocket->SetRecvErr();
}

void UAirPythonConnector::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (_pListenSocket)
	{
		_pListenSocket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(_pListenSocket);
	}
}

void UAirPythonConnector::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (_bRepeat == false)
	{
		return;
	}

	_ElapsedTime += DeltaTime;
	if(_ElapsedTime >= _RepeatDelay)
	{
		SendUDPMessage(_SuccesMessage, _IPString);
		_ElapsedTime = 0.0f;
	}
}

char UAirPythonConnector::GetInputOverUDP()
{
	if (!_pListenSocket) 
	{
		if (_bDebug)
		{
			UE_LOG(LogTemp, Error, TEXT("No socket available"));
		}
		return '0';
	}

	uint8 ReceiveBuffer[1];  // Buffer size of 1 byte
	int32 BytesRead;
	TSharedRef<FInternetAddr> SenderAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	// Handle received UDP packets
	while (_pListenSocket->RecvFrom(ReceiveBuffer, sizeof(ReceiveBuffer), BytesRead, *SenderAddress))
	{
		if (BytesRead > 0)
		{
			return ReceiveBuffer[0]; // Directly return the received character
		}
	}

	return '0';
}

void UAirPythonConnector::SendUDPMessage(const FString& Message, const FString& IP)
{
	if (!_pListenSocket) return;

	int32 BytesSent;
	TSharedRef<FInternetAddr> RemoteAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	bool bIsValid{};
	RemoteAddr->SetIp(*IP, bIsValid);
	RemoteAddr->SetPort(_UDPSendPort);

	if (bIsValid)
	{
		TArray<uint8> SendBuffer;
		StringToBytes(Message, SendBuffer, StringCast<ANSICHAR>(*Message).Length());
		_pListenSocket->SendTo(SendBuffer.GetData(), SendBuffer.Num(), BytesSent, *RemoteAddr);
	}
}