// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SpriteAnimationBase.h"
#include "PaperFlipbookComponent.h"
#include "MainSpriteObjectBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SPRITEANIMATIONRUNTIME_API UMainSpriteObjectBase : public UObject
{
	GENERATED_BODY()
	
public:

	UMainSpriteObjectBase();

	UFUNCTION(BlueprintCallable, Category = "Sprite Retrieve")
	TArray<UPaperFlipbook*> GetSpriteCollection();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TMap<FName, TSubclassOf<USpriteAnimationBase>> SpriteCollection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	FName SpriteState;
};
