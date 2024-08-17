// Copyright Matthew Wolfe All Rights Reserved.

#include "PluginWindowManager.h"
#include "SpriteAnimationPreview.h"
#include "MiscOperations.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

UPluginWindowManager::UPluginWindowManager()
{
	AnimationPreviewMap.SetNum(4);
	CustomAnimationNameList.SetNum(4);
	RemovedIndices.SetNum(4);

	twoDirSpriteSelectionContainer = SNew(SVerticalBox);
	fourDirSpriteSelectionContainer = SNew(SVerticalBox);
	eightDirSpriteSelectionContainer = SNew(SVerticalBox);
	flatSpriteSelectionContainer = SNew(SVerticalBox);

	twoDirCustomStatesContainer = SNew(SVerticalBox);
	fourDirCustomStatesContainer = SNew(SVerticalBox);
	eightDirCustomStatesContainer = SNew(SVerticalBox);
	flatCustomStatesContainer = SNew(SVerticalBox);
}

TSharedRef<SDockTab> UPluginWindowManager::MakeSpriteIntegrationWindow() 
{
	return SNew(SDockTab);
}

TSharedPtr<SVerticalBox> UPluginWindowManager::GetTwoDirContainer() const 
{
	return twoDirSpriteSelectionContainer;
}

TSharedPtr<SVerticalBox> UPluginWindowManager::GetFourDirContainer() const
{
	return fourDirSpriteSelectionContainer;
}

TSharedPtr<SVerticalBox> UPluginWindowManager::GetEightDirContainer() const
{
	return eightDirSpriteSelectionContainer;
}

TSharedPtr<SVerticalBox> UPluginWindowManager::GetFlatContainer() const
{
	return flatSpriteSelectionContainer;
}

TSharedPtr<SVerticalBox> UPluginWindowManager::GetTwoDirCustomContainer() const
{
	return twoDirCustomStatesContainer;
}

TSharedPtr<SVerticalBox> UPluginWindowManager::GetFourDirCustomContainer() const
{
	return fourDirCustomStatesContainer;
}

TSharedPtr<SVerticalBox> UPluginWindowManager::GetEightDirCustomContainer() const
{
	return eightDirCustomStatesContainer;
}

TSharedPtr<SVerticalBox> UPluginWindowManager::GetFlatCustomContainer() const
{
	return flatCustomStatesContainer;
}

FString UPluginWindowManager::GetCustomAnimationName(int32 Index, int32 WidgetIndex) const 
{
	if (CustomAnimationNameList.Num() <= Index)
	{
		return "";
	}
	return CustomAnimationNameList[WidgetIndex][Index]->GetText().ToString();
}

int32 UPluginWindowManager::GetCustomAnimationNameListSize(int32 Index) const 
{
	return CustomAnimationNameList[Index].Num();
}

TArray<FString> UPluginWindowManager::GetAnimationStatesList(int32 Index) const
{
	TArray<FString> StatesList;
	AnimationPreviewMap[Index].GetKeys(StatesList);
	return StatesList;
}

int32 UPluginWindowManager::GetAnimationStateNum(FString Key, int32 Index) const
{
	return AnimationPreviewMap[Index][Key].Num();
}

TSharedPtr<SSpriteAnimationPreview> UPluginWindowManager::GetAnimationPreview(FString Key, int32 Index, int32 WidgetIndex) const 
{
	return AnimationPreviewMap[WidgetIndex][Key][Index];
}

void UPluginWindowManager::FullReset() 
{
	twoDirCustomStatesContainer->ClearChildren();
	fourDirCustomStatesContainer->ClearChildren();
	eightDirCustomStatesContainer->ClearChildren();
	flatCustomStatesContainer->ClearChildren();
	CustomAnimationNameList.Empty();
	CustomAnimationNameList.SetNum(4);
	RemovedIndices.Empty();
	RemovedIndices.SetNum(4);


	AnimationPreviewMap.Empty();
	AnimationPreviewMap.SetNum(4);
	twoDirSpriteSelectionContainer->ClearChildren();
	fourDirSpriteSelectionContainer->ClearChildren();
	eightDirSpriteSelectionContainer->ClearChildren();
	flatSpriteSelectionContainer->ClearChildren();
}

//===========================================================================================================================================
void UPluginWindowManager::SpriteSelectionConstruct(int32 DirectionCount) 
{
	UMiscOperations* operation = NewObject<UMiscOperations>();

	//Check if already constructed
	if (AnimationPreviewMap[0].Num() > 0 || AnimationPreviewMap[1].Num() > 0
		|| AnimationPreviewMap[2].Num() > 0 || AnimationPreviewMap[3].Num() > 0
		|| twoDirSpriteSelectionContainer->NumSlots() > 0 || fourDirSpriteSelectionContainer->NumSlots() > 0
		|| eightDirSpriteSelectionContainer->NumSlots() > 0 || flatSpriteSelectionContainer->NumSlots() > 0)
	{
		return;
	}

	//Base states are Idle and Move, so only 2
	for (int i = 0; i < 2; i++)
	{
		FString state = operation->GetState(i);
		TArray<TSharedPtr<SSpriteAnimationPreview>> AnimationPreviewList;

		AnimationPreviewMap[0].FindOrAdd(state);
		AnimationPreviewMap[1].FindOrAdd(state);
		AnimationPreviewMap[2].FindOrAdd(state);
		
		for (int j = 0; j < DirectionCount; j++)
		{
			//First we define which of the states is getting placed
			FString direction = operation->GetDirection(j);


			//We create the animation preview section
			TSharedPtr<SSpriteAnimationPreview> tempAnimationPreview = SNew(SSpriteAnimationPreview);

			if (!tempAnimationPreview.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("Somehow did not work! The Animation Preview Section did not work!"));
				return;
			}

			//AnimationPreviewList.Add(
			//	SAssignNew(tempAnimationPreview, SSpriteAnimationPreview)
			//);

			//if (AnimationPreviewList.Num() <= j)
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("Failed to add into the animation list properly!"));
			//	return;
			//}

			if (j < 2)
			{
				AnimationPreviewMap[0][state].Add(SAssignNew(tempAnimationPreview, SSpriteAnimationPreview));
			}
			if (j < 4)
			{
				AnimationPreviewMap[1][state].Add(SAssignNew(tempAnimationPreview, SSpriteAnimationPreview));
			}
			AnimationPreviewMap[2][state].Add(SAssignNew(tempAnimationPreview, SSpriteAnimationPreview));
		}

		//AnimationPreviewMap.Add(state, AnimationPreviewList);
	}

	//Now we add the animation previews to the containers
	TArray<FString> states;
	AnimationPreviewMap[0].GetKeys(states);

	for (int i = 0; i < states.Num(); i++)
	{
		FString state = states[i];
		//TArray<TSharedPtr<SSpriteAnimationPreview>> AnimationPreviewList = AnimationPreviewMap[state];

		for (int j = 0; j < AnimationPreviewMap[2][state].Num(); j++)
		{
			FString direction = operation->GetDirection(j);
			//We now can construct the vertical box with what is needed
			//Will need to check for the index to see what gets added in for the direction count
			if (j < 2)
			{
				//UE_LOG(LogTemp, Warning, TEXT("%s"), *direction);
				twoDirSpriteSelectionContainer->AddSlot()
					.AutoHeight()
					[
						SNew(STextBlock)
							.Text(FText::FromString(state + " " + direction))
					];
				twoDirSpriteSelectionContainer->AddSlot()
					.AutoHeight()
					[
						AnimationPreviewMap[0][state][j].ToSharedRef()
					];
			}

			if (j < 4)
			{
				fourDirSpriteSelectionContainer->AddSlot()
					.AutoHeight()
					[
						SNew(STextBlock)
							.Text(FText::FromString(state + " " + direction))
					];
				fourDirSpriteSelectionContainer->AddSlot()
					.AutoHeight()
					[
						AnimationPreviewMap[1][state][j].ToSharedRef()
					];
			}


			eightDirSpriteSelectionContainer->AddSlot()
				.AutoHeight()
				[
					SNew(STextBlock)
						.Text(FText::FromString(state + " " + direction))
				];
			eightDirSpriteSelectionContainer->AddSlot()
				.AutoHeight()
				[
					AnimationPreviewMap[2][state][j].ToSharedRef()
				];
		}

	}

	//Creating the set for the flat animation
	//We create the animation preview section
	//TArray<TSharedPtr<SSpriteAnimationPreview>> AnimationPreviewList;
	TSharedPtr<SSpriteAnimationPreview> tempAnimationPreview = SNew(SSpriteAnimationPreview);

	if (!tempAnimationPreview.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Somehow did not work! The Animation Preview Section did not work!"));
		return;
	}

	//AnimationPreviewList.Add(
	//	SAssignNew(tempAnimationPreview, SSpriteAnimationPreview)
	//);

	//if (AnimationPreviewList.Num() <= 0)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Failed to add into the animation list properly!"));
	//	return;
	//}
	FString FlatState = "Flat_Animation";
	AnimationPreviewMap[3].Add(FlatState);
	AnimationPreviewMap[3][FlatState].Add(SAssignNew(tempAnimationPreview, SSpriteAnimationPreview));

		flatSpriteSelectionContainer->AddSlot()
			.AutoHeight()
			[
				SNew(STextBlock)
					.Text(FText::FromString("Flat Animation"))
			];
		flatSpriteSelectionContainer->AddSlot()
			.AutoHeight()
			[
				AnimationPreviewMap[3][FlatState][0].ToSharedRef()
			];

		//AnimationPreviewMap.Add("Flat Animation", AnimationPreviewList);
}

//===========================================================================================================================================
void UPluginWindowManager::SetWidgetIndex(int32 Index) 
{
	DirectionSwitcherIndex = Index;
}

void UPluginWindowManager::AddCustomState(int32 DirectionCount) 
{
	int32 WidgetIndex = DirectionSwitcherIndex;


	TSharedPtr<SVerticalBox> tempBoxContainer = SNew(SVerticalBox);

	TSharedPtr<SEditableTextBox> tempEditBox = SNew(SEditableTextBox);



	if (!tempEditBox.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Was not able to add due to failure on the edit box name!"));
		return;
	}


	CustomAnimationNameList[WidgetIndex].Add(
		SAssignNew(tempEditBox, SEditableTextBox)
		.Text(FText::FromString("Custom State"))
	);

	if (CustomAnimationNameList[WidgetIndex].Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Was not able to add that custom animation!"));
		return;
	}

	int32 customAnimationIndex = CustomAnimationNameList[WidgetIndex].Num() - 1;

	for (int32 RemovedIndex : RemovedIndices[WidgetIndex])
	{
		if (customAnimationIndex >= RemovedIndex)
		{
			customAnimationIndex++;
		}
	}

	tempBoxContainer->AddSlot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					//SNew(SBox)
					CustomAnimationNameList[WidgetIndex][CustomAnimationNameList[WidgetIndex].Num() - 1].ToSharedRef()
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
						.Text(FText::FromString("Remove"))
						.OnClicked_Lambda([this, tempBoxContainer, customAnimationIndex, WidgetIndex]()
							{
								if (tempBoxContainer.IsValid())
								{
									int index = customAnimationIndex;

									for (int32 i = RemovedIndices[WidgetIndex].Num() - 1; i >= 0; i--)
									{
										int32 RemovedIndex = RemovedIndices[WidgetIndex][i];
										if (index > RemovedIndex && index != 0)
										{
											index--;
										}
									}

									if (index >= CustomAnimationNameList[WidgetIndex].Num() || index < 0)
									{
	/*									UE_LOG(LogTemp, Warning, TEXT("Couldn't Remove!"));

										UE_LOG(LogTemp, Warning, TEXT("Removed Indices Total %d"), RemovedIndices.Num());
										for (int32 i = 0; i < RemovedIndices.Num(); i++)
										{
											UE_LOG(LogTemp, Warning, TEXT("Removed Indices pos %d value %d"), i, RemovedIndices[i]);
										}
										UE_LOG(LogTemp, Warning, TEXT("Attempted removal index %d"), index);
										UE_LOG(LogTemp, Warning, TEXT("Supplied creation index %d"), customAnimationIndex);*/

										return FReply::Unhandled();
									}
									switch (WidgetIndex)
									{
									case 0:
										twoDirCustomStatesContainer->RemoveSlot(tempBoxContainer.ToSharedRef());
										break;
									case 1:
										fourDirCustomStatesContainer->RemoveSlot(tempBoxContainer.ToSharedRef());
										break;
									case 2:
										eightDirCustomStatesContainer->RemoveSlot(tempBoxContainer.ToSharedRef());
										break;
									case 3:
										flatCustomStatesContainer->RemoveSlot(tempBoxContainer.ToSharedRef());
									}
									
									CustomAnimationNameList[WidgetIndex].RemoveAt(index);

									AnimationPreviewMap[WidgetIndex].Remove(FString::FromInt(index));


									RemovedIndices[WidgetIndex].Add(customAnimationIndex);
									RemovedIndices[WidgetIndex].Sort();

									if (CustomAnimationNameList[WidgetIndex].Num() == 0)
									{
										index = -1;
										RemovedIndices[WidgetIndex].Empty();
									}
								}

								return FReply::Handled();
							})
				]

		];

	UMiscOperations* operation = NewObject<UMiscOperations>();

	AnimationPreviewMap[WidgetIndex].FindOrAdd(FString::FromInt(customAnimationIndex));

	int DirCount = 0;

	switch (WidgetIndex)
	{
	case 0:
		DirCount = 2;
		break;
	case 1:
		DirCount = 4;
		break;
	case 2:
		DirCount = 8;
		break;
	case 3:
		DirCount = 1;
	}


	for (int i = 0; i < DirCount; i++)
	{
		TSharedPtr<SSpriteAnimationPreview> tempAnimationPreview = SNew(SSpriteAnimationPreview);

		if (!tempAnimationPreview.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Somehow did not work! The Animation Preview Section did not work!"));
			return;
		}
		AnimationPreviewMap[WidgetIndex][FString::FromInt(customAnimationIndex)].Add(SAssignNew(tempAnimationPreview, SSpriteAnimationPreview));
	}

	if (DirCount > 1)
	{
		//Non-Flat Animation
		tempBoxContainer->AddSlot()
			[
				CustomStateConstruct(AnimationPreviewMap[WidgetIndex][FString::FromInt(customAnimationIndex)], DirCount).ToSharedRef()
			];

		switch (WidgetIndex)
		{
		case 0:
			twoDirCustomStatesContainer->AddSlot()
				.AutoHeight()
				[
					tempBoxContainer.ToSharedRef()
				];
			break;
		case 1:
			fourDirCustomStatesContainer->AddSlot()
				.AutoHeight()
				[
					tempBoxContainer.ToSharedRef()
				];
			break;
		case 2:
			eightDirCustomStatesContainer->AddSlot()
				.AutoHeight()
				[
					tempBoxContainer.ToSharedRef()
				];

		}
	}
	else
	{

		tempBoxContainer->AddSlot()
			[
				AnimationPreviewMap[3][FString::FromInt(customAnimationIndex)][0].ToSharedRef()
			];

		flatCustomStatesContainer->AddSlot()
			.AutoHeight()
			[
				tempBoxContainer.ToSharedRef()
			];
	}

}

//===========================================================================================================================================
TSharedPtr<SVerticalBox> UPluginWindowManager::CustomStateConstruct(TArray<TSharedPtr<SSpriteAnimationPreview>> & list, int32 directionCount)
{
	TSharedPtr<SVerticalBox> tempBox = SNew(SVerticalBox);
	UMiscOperations* operation = NewObject<UMiscOperations>();

	for (int i = 0; i < directionCount; i++)
	{
		FString direction = operation->GetDirection(i);

		tempBox->AddSlot()
			.AutoHeight()
			[
				SNew(STextBlock)
					.Text(FText::FromString(direction))
			];

		tempBox->AddSlot()
			.AutoHeight()
			[
				list[i].ToSharedRef()
			];
	}
	return tempBox;
}