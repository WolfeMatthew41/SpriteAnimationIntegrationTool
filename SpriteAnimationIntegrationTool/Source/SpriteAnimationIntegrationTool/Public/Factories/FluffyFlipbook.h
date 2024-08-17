// Copyright Matthew Wolfe All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "FluffyFlipbook.generated.h"

/**  */
UCLASS()
class UFluffyFlipbook : public UPaperFlipbook
{
	GENERATED_BODY()

public:
	void AddKeyFrame(UPaperSprite* Sprite, int32 FrameRun);

	void SetFramesPerSecond(float InFramesPerSecond)
	{
		FramesPerSecond = InFramesPerSecond;
	}
};