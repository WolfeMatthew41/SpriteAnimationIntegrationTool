// Copyright Matthew Wolfe All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "PaperFlipbook.h"
#include "FluffyFlipbookFactory.generated.h"

/**  */
UCLASS()
class UFluffyFlipbookFactory : public UFactory
{
	GENERATED_BODY()

public:
	UFluffyFlipbookFactory();

	UPaperFlipbook* CreateFlipbookFromSprites(const TArray<UPaperSprite*> Sprites, float FPS, const FString& TargetPath, const FString& FlipbookName);

};