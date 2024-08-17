// Copyright Matthew Wolfe All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "DesktopPlatformModule.h"

/**  */
class SSpriteAnimationPreview : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSpriteAnimationPreview) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	FString GetFolderPath() const;
	float GetFrameRate() const;
	TArray<TSharedPtr<FString>> GetImageList() const;

private:
	TArray<TSharedPtr<FString>> ImageList;

	TSharedPtr<SEditableTextBox> FolderPathList;
	TSharedPtr<SEditableTextBox> FrameRateBox;
	float FrameRate;
	int32 CurrentFrame;
	TSharedPtr<SImage> ImageWidget;

	FReply OnInputFolderSelectionButton();

	FReply OnGenerateAnimationSpriteButton();

	void GetImageFiles(const FString& folderPath);

	void StartAnimation();
	FReply OnPlayButton();
	FReply OnStopButton();
	void UpdateAnimation();

	FVector2D GetImageSize(UTexture2D* texture);

	FTimerHandle AnimationTimerHandle;
};