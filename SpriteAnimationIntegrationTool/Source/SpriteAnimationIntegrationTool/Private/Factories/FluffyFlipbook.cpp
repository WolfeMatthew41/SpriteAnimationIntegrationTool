// Copyright Matthew Wolfe All Rights Reserved.

#include "FluffyFlipbook.h"


void UFluffyFlipbook::AddKeyFrame(UPaperSprite* Sprite, int32 FrameRun) 
{
	FPaperFlipbookKeyFrame KeyFrame;
	KeyFrame.Sprite = Sprite;
	KeyFrame.FrameRun = FrameRun;

	KeyFrames.Add(KeyFrame);
}