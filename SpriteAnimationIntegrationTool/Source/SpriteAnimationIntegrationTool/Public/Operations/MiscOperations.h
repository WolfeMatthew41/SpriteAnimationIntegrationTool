// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MiscOperations.generated.h"

/**
 * 
 */
UCLASS()
class UMiscOperations : public UObject
{
	GENERATED_BODY()

public:

	//UPluginWindowManager();
	
	FString OutputFolderSelect();

	FString GetDirection(int32 Index);

	FString GetState(int32 Index);

	bool AddState(FString StateName);

	void RemoveState(int32 Index);

	FString PadNumberString(int32 number, int32 totalDigits);

private:

	TMap<int32, FString> StateMap = {
		{0, "Idle"},
		{1, "Move"}
	};

	TMap<int32, FString> DirectionMap = {
		{0, "Left"},
		{1, "Right"},
		{2, "Up"},
		{3, "Down"},
		{4, "Up Left"},
		{5, "Up Right"},
		{6, "Down Left"},
		{7, "Down Right"}
	};
};
