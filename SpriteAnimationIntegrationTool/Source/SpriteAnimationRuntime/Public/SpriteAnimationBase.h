// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "UObject/NoExportTypes.h"
#include "SpriteAnimationBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SPRITEANIMATIONRUNTIME_API USpriteAnimationBase : public UObject
{
	GENERATED_BODY()

public:

	USpriteAnimationBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animations")
	TArray<UPaperFlipbook*> SpriteAnimations;
	
};
