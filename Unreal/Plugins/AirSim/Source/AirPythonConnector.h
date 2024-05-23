// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "AirPythonConnector.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIRSIM_API UAirPythonConnector : public UActorComponent
{
	GENERATED_BODY()

/*
	-------------------------------------
		Unreal Engine Functions
	-------------------------------------
*/
public:	
	UAirPythonConnector();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	virtual void BeginPlay() override;


/*
	-------------------------------------
		Custom Functions
	-------------------------------------
*/
public:
	// Send a message via UDP
	void SendUDPMessage(const FString& message, const FString& ip);

	// Checks for input from Python and returns a char representing the pressed key
	char GetInputOverUDP();

protected:

/*
	-------------------------------------
		Variables
	-------------------------------------
*/
protected:
	UPROPERTY(EditAnywhere, Category = "Settings|Networking")
	uint16 _UDPListenPort{ 9999 };

	UPROPERTY(EditAnywhere, Category = "Settings|Networking")
	int32  _UDPSendPort{ 9988 };

	FSocket* _pListenSocket{ nullptr };

	UPROPERTY(EditAnywhere, Category = "Settings|Networking")
	FString _SuccesMessage{ "LOAD_SUCCESS" };

	UPROPERTY(EditAnywhere, Category = "Settings|Networking")
	FString _IPString{ "127.0.0.1" };

	UPROPERTY(EditAnywhere, Category = "Settings")
	bool _bRepeat{ true };

	UPROPERTY(EditAnywhere, Category = "Settings")
	float _RepeatDelay{ 10.f };

	float _ElapsedTime{ 0.f };

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool _bDebug{ false };		
};
