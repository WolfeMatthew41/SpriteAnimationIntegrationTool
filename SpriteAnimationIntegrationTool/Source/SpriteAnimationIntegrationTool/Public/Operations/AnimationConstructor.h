// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PluginWindowManager.h"
#include "PaperSprite.h"
#include "PaperFlipbook.h"
#include "AnimationConstructor.generated.h"

/**
 * 
 */
UCLASS()
class UAnimationConstructor : public UObject
{
	GENERATED_BODY()

public:

	//UPluginWindowManager();

	void OnCreateAnimation(int32 WidgetIndex, FString AnimationName, FString SelectedPath, UPluginWindowManager* PluginManager);

	

private:

	bool CreateFolder(const FString& BaseFolderPath, const FString& NewFolderName);

	bool OrganizeFolders(FString AnimationName, FString SelectedPath, int32 WidgetIndex, UPluginWindowManager* PluginManager);

	bool ImportTextures(UPluginWindowManager* PluginManager, FString AnimationName, int32 WidgetIndex);

	bool CreateSprites();

	bool CreateFlipbooks(FString AnimationName, UPluginWindowManager* PluginManager, int32 WidgetIndex);

	bool CreateObjects(FString AnimationName);

	bool CreateMainObject(FString AnimationName);


	FString AnimationPath;
	FString SpritePath;
	FString TexturePath;
	FString AnimationStatesPath;
	FString FlipbookPath;

	TMap<FString, TArray<FString>> SpritePathMap;
	TMap<FString, TArray<FString>> FlipbookPathMap;

	TMap<FString, TArray<TArray<UTexture2D*>>> TextureMap;
	TMap<FString, TArray<TArray<UPaperSprite*>>> SpriteMap;
	TMap<FString, TArray<UPaperFlipbook*>> FlipbookMap;

	TMap<FString, FString> ObjectMap;
	
};
