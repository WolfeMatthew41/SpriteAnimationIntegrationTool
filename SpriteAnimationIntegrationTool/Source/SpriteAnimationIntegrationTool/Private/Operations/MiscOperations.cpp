// Copyright Matthew Wolfe All Rights Reserved.

#include "MiscOperations.h"
#include "IDesktopPlatform.h"
#include "DesktopPlatformModule.h"

FString UMiscOperations::OutputFolderSelect() 
{
	IDesktopPlatform* desktopPlatform = FDesktopPlatformModule::Get();

	if (desktopPlatform)
	{
		FString folderPath;
		const void* parentWindow = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);

		if (desktopPlatform->OpenDirectoryDialog(parentWindow, TEXT("Choose a Directory in the projects Contents"), TEXT(""), folderPath))
		{
			return folderPath;
		}
	}
	return "";
}

FString UMiscOperations::GetState(int32 Index)
{
	if (StateMap.Find(Index))
	{
		return StateMap[Index];
	}
	return "";
}

bool UMiscOperations::AddState(FString StateName) 
{
	int32 mapSize = StateMap.Num() - 1;
	StateMap.Add(mapSize, StateName);

	if (StateMap.Find(mapSize))
	{
		return true;
	}
	return false;
}

void UMiscOperations::RemoveState(int32 Index) 
{
	StateMap.Remove(Index);
}

FString UMiscOperations::GetDirection(int32 Index) 
{
	if (DirectionMap.Find(Index))
	{
		return DirectionMap[Index];
	}
	return "";
}


FString UMiscOperations::PadNumberString(int32 number, int32 totalDigits)
{
	FString numberString = FString::FromInt(number);
	while (numberString.Len() < totalDigits)
	{
		numberString = "0" + numberString;
	}
	return numberString;
}