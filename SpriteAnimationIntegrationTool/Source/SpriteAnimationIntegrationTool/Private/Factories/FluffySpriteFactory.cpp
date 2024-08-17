// Copyright Matthew Wolfe All Rights Reserved.

#include "FluffySpriteFactory.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "AssetRegistry/AssetRegistryModule.h"

UFluffySpriteFactory::UFluffySpriteFactory()
{
	SupportedClass = UPaperSprite::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UPaperSprite* UFluffySpriteFactory::CreateSpriteFromTexture(UTexture2D* Texture, const FString& TargetPath)
{
	if (!Texture)
	{
		return nullptr;
	}
	FString destinationPath = TargetPath.Replace(*FPaths::ProjectContentDir(), TEXT("")).Replace(TEXT("\\"), TEXT("/"));

	if (destinationPath.EndsWith("/"))
	{
		destinationPath.LeftChopInline(1);
	}

	//FString packageName = targetPath / newName;
	

	FString textureName = Texture->GetName();
	FString packageName = FString::Printf(TEXT("/Game/%s/%s"), *destinationPath, *textureName);

	UPackage* package = CreatePackage(*packageName);

	if (!package)
	{
		return nullptr;
	}

	UPaperSprite* newSprite = NewObject<UPaperSprite>(package, FName(*Texture->GetName()),
		RF_Standalone | RF_Public);

	if (newSprite)
	{
		newSprite->Modify();
		
		FSpriteAssetInitParameters initParams;
		initParams.SetTextureAndFill(Texture);
		
		newSprite->InitializeSprite(initParams);


		FAssetRegistryModule::AssetCreated(newSprite);
		package->MarkPackageDirty();


		FString packageFileName = FPackageName::LongPackageNameToFilename(packageName, FPackageName::GetAssetPackageExtension());
		UPackage::SavePackage(package, newSprite, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *packageFileName, GError, nullptr, true, true, SAVE_NoError);
	}

	return newSprite;
}