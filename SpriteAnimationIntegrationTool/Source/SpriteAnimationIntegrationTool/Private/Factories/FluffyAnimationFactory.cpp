// Copyright Matthew Wolfe All Rights Reserved.

#include "FluffyAnimationFactory.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "Factories/BlueprintFactory.h"
#include "Kismet2/KismetEditorUtilities.h"


UMainSpriteObjectBase* UFluffyAnimationFactory::CreateMainAnimationObject(const FString& SourceName, const FString& TargetPath, 
	TMap<FString, FString> ObjectMap) 
{
	FAssetToolsModule& assetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");
	IAssetTools& assetTools = assetToolsModule.Get();

	UBlueprintFactory* blueprintFactory = NewObject<UBlueprintFactory>();
	blueprintFactory->ParentClass = UMainSpriteObjectBase::StaticClass();

	FString destinationPath = TargetPath.Replace(*FPaths::ProjectContentDir(), TEXT("")).Replace(TEXT("\\"), TEXT("/"));

	if (destinationPath.EndsWith("/"))
	{
		destinationPath.LeftChopInline(1);
	}

	//FString packageName = targetPath / newName;
	FString packageName = FString::Printf(TEXT("/Game/%s"), *destinationPath);
	//FString packageName = TargetPath;

	UObject* newAsset = assetTools.CreateAsset(SourceName, packageName, UBlueprint::StaticClass(), blueprintFactory);

	if (newAsset)
	{
		UBlueprint* newBP = Cast<UBlueprint>(newAsset);
		if (newBP)
		{
			//Setup here
			UMainSpriteObjectBase* mainAnimation = Cast<UMainSpriteObjectBase>(newBP->GeneratedClass->GetDefaultObject());
			
			if (mainAnimation)
			{
				
				//Add states here
				TArray<FString> statesList;
				ObjectMap.GetKeys(statesList);

				mainAnimation->SpriteState = FName(*statesList[0]);
				int count = 0;
				for (const FString& state : statesList)
				{
					
					UBlueprint* loadObject = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), nullptr, *ObjectMap[state]));

					if (!loadObject)
					{
						return false;
					}

					UClass* object = loadObject->GeneratedClass;

					if (!object || !object->IsChildOf(USpriteAnimationBase::StaticClass()))
					{
						return false;
					}
					mainAnimation->SpriteCollection.Add(FName(*state), object);
					count++;
				}

				//Save
				newBP->MarkPackageDirty();
				FAssetRegistryModule::AssetCreated(newBP);
				newBP->PostEditChange();
				newBP->AddToRoot();

				FKismetEditorUtilities::CompileBlueprint(newBP);

				return mainAnimation;
			}
		}
	}

	return nullptr;
}

FString UFluffyAnimationFactory::CreateAnimationObject(const FString& SourceName, const FString& TargetPath, TArray<UPaperFlipbook*> Animations)
{
	FAssetToolsModule& assetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");
	IAssetTools& assetTools = assetToolsModule.Get();

	UBlueprintFactory* blueprintFactory = NewObject<UBlueprintFactory>();
	blueprintFactory->ParentClass = USpriteAnimationBase::StaticClass();

	FString destinationPath = TargetPath.Replace(*FPaths::ProjectContentDir(), TEXT("")).Replace(TEXT("\\"), TEXT("/"));

	if (destinationPath.EndsWith("/"))
	{
		destinationPath.LeftChopInline(1);
	}

	//FString packageName = targetPath / newName;
	FString packageName = FString::Printf(TEXT("/Game/%s"), *destinationPath);
	PackageList.Add(packageName);
	UObject* newAsset = assetTools.CreateAsset(SourceName, packageName, UBlueprint::StaticClass(), blueprintFactory);

	if (newAsset)
	{
		UBlueprint* newBP = Cast<UBlueprint>(newAsset);
		if (newBP)
		{
			//Setup here
			USpriteAnimationBase* spriteState = Cast<USpriteAnimationBase>(newBP->GeneratedClass->GetDefaultObject());
			
			if (spriteState)
			{
				
				//Add flipbooks here
				for (int i = 0; i < Animations.Num(); i++)
				{
					spriteState->SpriteAnimations.Add(Animations[i]);
				}

				//Save
				newBP->MarkPackageDirty();
				FAssetRegistryModule::AssetCreated(newBP);
				newBP->PostEditChange();
				newBP->AddToRoot();

				FKismetEditorUtilities::CompileBlueprint(newBP);

				if (UBlueprint* object = Cast<UBlueprint>(spriteState))
				{
					FKismetEditorUtilities::CompileBlueprint(object);
				}
				

				return packageName + "/" + SourceName;
			}
		}
	}
	return "";
}