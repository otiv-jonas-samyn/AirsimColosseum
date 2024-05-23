// Fill out your copyright notice in the Description page of Project Settings.
#include "ObjectAirsimTagComponent.h"
#include "SimMode/SimModeBase.h"

UObjectAirsimTagComponent::UObjectAirsimTagComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UObjectAirsimTagComponent::BeginPlay()
{
	Super::BeginPlay();

	int smallestTag{ static_cast<int>(msr::airlib::EObjectAirsimTag::OBJECT_AIRSIM_TAG_NONE) };
    int biggestTag{ static_cast<int>(msr::airlib::EObjectAirsimTag::OBJECT_AIRSIM_TAG_LAST_EXCLUDE) };

    if (_ObjectAirsimTagInt < smallestTag) 
    {
        _ObjectAirsimTagInt = smallestTag;
    }
    else if (_ObjectAirsimTagInt >= biggestTag) 
    {
        _ObjectAirsimTagInt = biggestTag - 1;
    }

    _ObjectAirsimTag = static_cast<msr::airlib::EObjectAirsimTag>(_ObjectAirsimTagInt);
    ASimModeBase::getSimMode()->AddObjectToInstance(GetOwner());
}

void UObjectAirsimTagComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

