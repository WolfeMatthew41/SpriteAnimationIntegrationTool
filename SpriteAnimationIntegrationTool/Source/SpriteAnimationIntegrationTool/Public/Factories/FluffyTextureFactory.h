// Copyright Matthew Wolfe All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "Engine/Texture2D.h"
#include "FluffyTextureFactory.generated.h"

/**  */
UCLASS()
class UFluffyTextureFactory : public UFactory
{
	GENERATED_BODY()

public:
	UFluffyTextureFactory();

	UTexture2D* ImportTexture(FString importFile, FString targetPath, FString newName);
};