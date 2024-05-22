// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../Car/CarPawn.h"
#include "DataCollectorPawn.generated.h"

UCLASS()
class AIRSIM_API ADataCollectorPawn : public ACarPawn
{
	GENERATED_BODY()

public:
	ADataCollectorPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "AirSim")
	bool SetAndAttachToActor(AActor* pActorToConnectTo);

	UFUNCTION(BlueprintCallable, Category = "AirSim")
	bool GetIsAttached() const { return _bIsAttached; }

	UFUNCTION(BlueprintCallable, Category = "AirSim")
	void SetDebug(bool bDebug) { _bDebug = bDebug; }

protected:
	UPROPERTY(EditAnywhere, Category = "AirSim")
	AActor* _pActorToConnectTo{ nullptr };

	bool _bIsAttached{ false };

	UPROPERTY(EditAnywhere, Category = "AirSim")
	bool _bDebug{ false };


};
