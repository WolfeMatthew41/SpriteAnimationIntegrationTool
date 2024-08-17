// Copyright Matthew Wolfe All Rights Reserved.

#include "FluffyFlipbookFactory.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "PaperFlipbookFactory.h"
#include "PaperFlipbookComponent.h"
#include "FluffyFlipbook.h"

UFluffyFlipbookFactory::UFluffyFlipbookFactory() 
{
	SupportedClass = UPaperFlipbook::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UPaperFlipbook* UFluffyFlipbookFactory::CreateFlipbookFromSprites(const TArray<UPaperSprite*> Sprites, float FPS, const FString& TargetPath,
		const FString& FlipbookName) 
{
	if (Sprites.Num() == 0)
	{
		return nullptr;
	}

	FString destinationPath = TargetPath.Replace(*FPaths::ProjectContentDir(), TEXT("")).Replace(TEXT("\\"), TEXT("/"));

	if (destinationPath.EndsWith("/"))
	{
		destinationPath.LeftChopInline(1);
	}

	FString packageName = FString::Printf(TEXT("/Game/%s/%s"), *destinationPath, *FlipbookName);
	UPackage* package = CreatePackage(*packageName);

	UFluffyFlipbook* newFlipbook = NewObject<UFluffyFlipbook>(package, FName(FlipbookName), RF_Standalone | RF_Public);

	if (newFlipbook)
	{

		float frameInterval = 1.0f / FPS;
		for (UPaperSprite* sprite : Sprites)
		{
			newFlipbook->AddKeyFrame(sprite, FMath::RoundToInt(frameInterval * FPS));
		}

		newFlipbook->SetFramesPerSecond(FPS);

		FAssetRegistryModule::AssetCreated(newFlipbook);
		package->MarkPackageDirty();

		FString packageFileName = FPackageName::LongPackageNameToFilename(packageName, FPackageName::GetAssetPackageExtension());
		
		UPackage::SavePackage(package, newFlipbook, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *packageFileName, GError, nullptr, true, true, SAVE_NoError);
		
	}

	return newFlipbook;
}