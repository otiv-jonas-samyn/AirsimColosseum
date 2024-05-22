// Fill out your copyright notice in the Description page of Project Settings.
#include "Vehicles/DataCollector/DataCollectorPawn.h"

ADataCollectorPawn::ADataCollectorPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADataCollectorPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADataCollectorPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADataCollectorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


bool ADataCollectorPawn::SetAndAttachToActor(AActor* pActor)
{
	if (pActor == nullptr)
	{
		_bIsAttached = false;
		return false;
	}

	_pActorToConnectTo = pActor;

	//Check if the actor has a camera component to attach to
	UCameraComponent* pCamera = _pActorToConnectTo->FindComponentByClass<UCameraComponent>();
	if (pCamera == nullptr)
	{
		if (_bDebug)
		{
			UE_LOG(LogTemp, Error, TEXT("ADataCollectorPawn::SetAndAttachToActor() -- ATTACHING TO ACTOR"));
		}
		_bIsAttached = AttachToActor(_pActorToConnectTo, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
	else
	{
		if (_bDebug)
		{
			UE_LOG(LogTemp, Error, TEXT("ADataCollectorPawn::SetAndAttachToActor() -- ATTACHING TO ACTORS CAMERA"));
		}
		_bIsAttached = AttachToComponent(pCamera, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
	return _bIsAttached;
}

