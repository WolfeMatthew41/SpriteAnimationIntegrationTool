// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Widgets/Docking/SDockTab.h"
#include "SpriteAnimationPreview.h"
#include "PluginWindowManager.generated.h"

/**
 * 
 */
UCLASS()
class UPluginWindowManager : public UObject
{
	GENERATED_BODY()

public:

	UPluginWindowManager();

	TSharedRef<SDockTab> MakeSpriteIntegrationWindow();

	void SpriteSelectionConstruct(int32 DirectionCount);

	void AddCustomState(int32 DirectionCount);

	void SetWidgetIndex(int32 Index);

	TSharedPtr<SVerticalBox> CustomStateConstruct(TArray<TSharedPtr<SSpriteAnimationPreview>>& list, int32 directionCount);

	TSharedPtr<SVerticalBox> GetTwoDirContainer() const;
	TSharedPtr<SVerticalBox> GetFourDirContainer() const;
	TSharedPtr<SVerticalBox> GetEightDirContainer() const;
	TSharedPtr<SVerticalBox> GetFlatContainer() const;

	TSharedPtr<SVerticalBox> GetTwoDirCustomContainer() const;
	TSharedPtr<SVerticalBox> GetFourDirCustomContainer() const;
	TSharedPtr<SVerticalBox> GetEightDirCustomContainer() const;
	TSharedPtr<SVerticalBox> GetFlatCustomContainer() const;

	void FullReset();

	FString GetCustomAnimationName(int32 Index, int32 WidgetIndex) const;
	int32 GetCustomAnimationNameListSize(int32 Index) const;

	TArray<FString> GetAnimationStatesList(int32 Index) const;
	int32 GetAnimationStateNum(FString Key, int32 Index) const;
	TSharedPtr<SSpriteAnimationPreview> GetAnimationPreview(FString Key, int32 Index, int32 WidgetIndex) const;

private:

	TArray<TMap<FString, TArray<TSharedPtr<SSpriteAnimationPreview>>>> AnimationPreviewMap;

	TSharedPtr<SVerticalBox> twoDirSpriteSelectionContainer;
	TSharedPtr<SVerticalBox> fourDirSpriteSelectionContainer;
	TSharedPtr<SVerticalBox> eightDirSpriteSelectionContainer;
	TSharedPtr<SVerticalBox> flatSpriteSelectionContainer;

	TSharedPtr<SVerticalBox> twoDirCustomStatesContainer;
	TSharedPtr<SVerticalBox> fourDirCustomStatesContainer;
	TSharedPtr<SVerticalBox> eightDirCustomStatesContainer;
	TSharedPtr<SVerticalBox> flatCustomStatesContainer;

	TArray<TArray<TSharedPtr<SEditableTextBox>>> CustomAnimationNameList;
	TArray<TArray<int32>> RemovedIndices;

	int32 DirectionSwitcherIndex = 0;
	
};
