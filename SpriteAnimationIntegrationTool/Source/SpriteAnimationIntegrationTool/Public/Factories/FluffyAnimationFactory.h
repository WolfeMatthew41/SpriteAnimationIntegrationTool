// Copyright Matthew Wolfe All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "PaperFlipbook.h"
#include "SpriteAnimationBase.h"
#include "MainSpriteObjectBase.h"
#include "FluffyAnimationFactory.generated.h"


/**  */
UCLASS()
class UFluffyAnimationFactory : public UFactory
{
	GENERATED_BODY()

public:

	UMainSpriteObjectBase* CreateMainAnimationObject(const FString& SourceName, const FString& TargetPath, TMap<FString, FString> ObjectMap);

	FString CreateAnimationObject(const FString& SourceName, const FString& TargetPath, TArray<UPaperFlipbook*> Animations);

	TArray<FString> PackageList;
};