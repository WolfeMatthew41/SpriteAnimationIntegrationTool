// Copyright Matthew Wolfe All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "PaperSprite.h"
#include "FluffySpriteFactory.generated.h"

/**  */
UCLASS()
class UFluffySpriteFactory : public UFactory
{
	GENERATED_BODY()

public:
	UFluffySpriteFactory();

	UPaperSprite* CreateSpriteFromTexture(UTexture2D* Texture, const FString& TargetPath);

};