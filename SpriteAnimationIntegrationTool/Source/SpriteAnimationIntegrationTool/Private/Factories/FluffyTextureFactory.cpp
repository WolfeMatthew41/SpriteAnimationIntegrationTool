// Copyright Matthew Wolfe All Rights Reserved.

#include "FluffyTextureFactory.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Factories/TextureFactory.h"
#include "Misc/FileHelper.h"

UFluffyTextureFactory::UFluffyTextureFactory()
{

}

UTexture2D* UFluffyTextureFactory::ImportTexture(FString importFile, FString targetPath, FString newName)
{
	if (!FPaths::FileExists(importFile))
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Missing a texture file!")));
		return nullptr;
	}

	if (!targetPath.StartsWith(FPaths::ProjectContentDir()))
	{
		//FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed!")));
		return nullptr;
	}

	TArray<uint8> fileData;
	if (!FFileHelper::LoadFileToArray(fileData, *importFile))
	{
		return nullptr;
	}
	UTexture2D* newTexture = nullptr;

	UTextureFactory* textureFactory = NewObject<UTextureFactory>();
	textureFactory->AddToRoot();
	textureFactory->SuppressImportOverwriteDialog();

	const uint8* dataStart = fileData.GetData();
	const uint8* dataEnd = fileData.GetData() + fileData.Num();


	FString destinationPath = targetPath.Replace(*FPaths::ProjectContentDir(), TEXT("")).Replace(TEXT("\\"), TEXT("/"));

	if (destinationPath.EndsWith("/"))
	{
		destinationPath.LeftChopInline(1);
	}

	//FString packageName = targetPath / newName;
	FString packageName = FString::Printf(TEXT("/Game/%s/%s"), *destinationPath, *newName);

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *packageName);

	UPackage* package = CreatePackage(*packageName);

	newTexture = (UTexture2D*)textureFactory->FactoryCreateBinary(
		UTexture2D::StaticClass(), package, FName(*newName), RF_Standalone | RF_Public, nullptr, *FPaths::GetExtension(importFile),
		dataStart, dataEnd, GWarn
	);

	if (newTexture)
	{
		FAssetRegistryModule::AssetCreated(newTexture);
		package->MarkPackageDirty();
		newTexture->PostEditChange();
		return newTexture;
	}

	return nullptr;

}