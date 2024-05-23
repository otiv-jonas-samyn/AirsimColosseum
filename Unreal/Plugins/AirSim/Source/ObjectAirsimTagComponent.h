// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "common/CommonEnums.hpp"
#include "ObjectAirsimTagComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIRSIM_API UObjectAirsimTagComponent : public UActorComponent
{
	GENERATED_BODY()

/*
--------------------------------------
	UNREAL ENGINE FUNCTIONS
--------------------------------------
*/
public:	
	UObjectAirsimTagComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

/*
--------------------------------------
	CUSTOM FUNCTIONS
--------------------------------------
*/
public:
	msr::airlib::EObjectAirsimTag GetObjectAirsimTag() const { return _ObjectAirsimTag; }

protected:

/*
--------------------------------------
	VARIABLES
--------------------------------------
*/
protected:
    UPROPERTY(EditAnywhere, Category = "ObjectAirsimTag")
    int _ObjectAirsimTagInt{ 0 }; // Check Airsim Tags to know the value of each tag

	msr::airlib::EObjectAirsimTag _ObjectAirsimTag{ msr::airlib::EObjectAirsimTag::OBJECT_AIRSIM_TAG_NONE };
};