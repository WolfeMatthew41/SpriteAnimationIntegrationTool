// Copyright Matthew Wolfe All Rights Reserved.

#include "AnimationConstructor.h"
#include "MiscOperations.h"
#include "SpriteAnimationPreview.h"
#include "FluffyTextureFactory.h"
#include "FluffySpriteFactory.h"
#include "FluffyFlipbookFactory.h"
#include "FluffyAnimationFactory.h"
#include "SpriteAnimationBase.h"
#include "Kismet2/KismetEditorUtilities.h"

void UAnimationConstructor::OnCreateAnimation(int32 WidgetIndex, FString AnimationName, FString SelectedPath, UPluginWindowManager* PluginManager)
{
	SpritePathMap.Empty();
	FlipbookPathMap.Empty();
	TextureMap.Empty();
	SpriteMap.Empty();
	FlipbookMap.Empty();
	ObjectMap.Empty();

	if (!OrganizeFolders(AnimationName, SelectedPath, WidgetIndex, PluginManager))
	{
		return;
	}

	if (!ImportTextures(PluginManager, AnimationName, WidgetIndex))
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to create the files!")));
		return;
	}
	if (!CreateSprites())
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to create the files!")));
		return;
	}
	if (!CreateFlipbooks(AnimationName, PluginManager, WidgetIndex))
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to create the files!")));
		return;
	}
	if (!CreateObjects(AnimationName))
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to create the files!")));
		return;
	}
	if (!CreateMainObject(AnimationName))
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to create the files!")));
		return;
	}
	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Finished creating the files!")));
}

bool UAnimationConstructor::CreateFolder(const FString& BaseFolderPath, const FString& NewFolderName)
{
	FString newFolderPath = FPaths::Combine(BaseFolderPath, NewFolderName);
	if (IFileManager::Get().MakeDirectory(*newFolderPath, true))
	{
		return true;
	}
	return false;
}

//==============================================================================================================================================
bool UAnimationConstructor::OrganizeFolders(FString AnimationName, FString SelectedPath, int32 WidgetIndex, UPluginWindowManager* PluginManager)
{
	//FString AnimationName = AnimationName;

	//Make the initial root folder
	if (!CreateFolder(SelectedPath, AnimationName))
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to create the folders!")));
		return false;
	}

	FString newPath = FPaths::Combine(SelectedPath, AnimationName);


	//Making the base constant folders
	if (!CreateFolder(newPath, AnimationName + "_" + TEXT("Animations")))
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to create the folders!")));
		return false;
	}
	if (!CreateFolder(newPath, AnimationName + "_" + TEXT("Sprites")))
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to create the folders!")));
		return false;
	}
	if (!CreateFolder(newPath, AnimationName + "_" + TEXT("Textures")))
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to create the folders!")));
		return false;
	}

	AnimationPath = FPaths::Combine(newPath, AnimationName + "_" + TEXT("Animations"));
	SpritePath = FPaths::Combine(newPath, AnimationName + "_" + TEXT("Sprites"));
	TexturePath = FPaths::Combine(newPath, AnimationName + "_" + TEXT("Textures"));

	if (!CreateFolder(AnimationPath, AnimationName + "_" + TEXT("AnimationStates")))
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to create the folders!")));
		return false;
	}
	if (!CreateFolder(AnimationPath, AnimationName + "_" + TEXT("Flipbooks")))
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to create the folders!")));
		return false;
	}

	AnimationStatesPath = FPaths::Combine(AnimationPath, AnimationName + "_" + TEXT("AnimationStates"));
	FlipbookPath = FPaths::Combine(AnimationPath, AnimationName + "_" + TEXT("Flipbooks"));


	UMiscOperations* operation = NewObject<UMiscOperations>();

	TArray<FString> StateList = PluginManager->GetAnimationStatesList(WidgetIndex);

	int32 CustomIndex = 0;

	for (int i = 0; i < StateList.Num(); i++)
	{
		//Making sure we get the correct State Name
		FString StateName = StateList[i];

		if (FCString::IsNumeric(*StateList[i]))
		{
			StateName = PluginManager->GetCustomAnimationName(CustomIndex, WidgetIndex).Replace(TEXT(" "), TEXT("_"));
			CustomIndex++;
		}

		//We can skip Textures as it does not need a level of organization than the baseline
		//We start with making the sprites folders
		if (!CreateFolder(SpritePath, AnimationName + "_" + StateName))
		{
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to create the folders!")));
			return false;
		}
		FString SpriteStatePath = FPaths::Combine(SpritePath, AnimationName + "_" + StateName);

		TArray<FString> DirectionList;

		for (int j = 0; j < PluginManager->GetAnimationStateNum(StateList[i], WidgetIndex); j++)
		{
			FString direction;
			if (WidgetIndex != 3)
			{
				direction = operation->GetDirection(j);
			}
			else
			{
				direction = "Flat";
			}
			if (!CreateFolder(SpriteStatePath, AnimationName + "_" + direction))
			{
				FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to create the folders!")));
				return false;
			}

			DirectionList.Add(FPaths::Combine(SpriteStatePath, AnimationName + "_" + direction));
		}

		SpritePathMap.Add(StateName, DirectionList);

		//We now make the flipbooks folders
		if (!CreateFolder(FlipbookPath, AnimationName + "_" + StateName))
		{
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to create the folders!")));
			return false;
		}

		FString FlipbookStatePath = FPaths::Combine(FlipbookPath, AnimationName + "_" + StateName);

		DirectionList.Empty();

		for (int j = 0; j < PluginManager->GetAnimationStateNum(StateList[i], WidgetIndex); j++)
		{
			FString direction;
			if (WidgetIndex != 3)
			{
				direction = operation->GetDirection(j);
			}
			else
			{
				direction = "Flat";
			}
			if (!CreateFolder(FlipbookStatePath, AnimationName + "_" + direction))
			{
				FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to create the folders!")));
				return false;
			}

			DirectionList.Add(FPaths::Combine(FlipbookStatePath, AnimationName + "_" + direction));
		}

		FlipbookPathMap.Add(StateName, DirectionList);
	}

	return true;
}

//==============================================================================================================================================
bool UAnimationConstructor::ImportTextures(UPluginWindowManager* PluginManager, FString AnimationName, int32 WidgetIndex)
{
	//This is a temporary method
	//ToDo: Make it work with sprite sheets importing and creation of sprite sheets from individual frames and pad to power of 2


	//Need to create a map of all of the states and the files to be included
	TMap<FString, TArray<TArray<FString>>> ImportMap;

	TArray<FString> TextureStates= PluginManager->GetAnimationStatesList(WidgetIndex);

	int32 CustomIndex = 0;

	for (int i = 0; i < TextureStates.Num(); i++)
	{
		//Making sure we get the correct State Name
		FString StateName = TextureStates[i];

		if (FCString::IsNumeric(*TextureStates[i]))
		{
			StateName = PluginManager->GetCustomAnimationName(CustomIndex, WidgetIndex).Replace(TEXT(" "), TEXT("_"));
			CustomIndex++;
		}

		int32 CurrentStateNum = PluginManager->GetAnimationStateNum(TextureStates[i], WidgetIndex);
		TArray<TArray<FString>> FileList;
		for (int j = 0; j < CurrentStateNum; j++)
		{
			TArray<FString> DirectionList;
			TSharedPtr<SSpriteAnimationPreview> CurrentAnimation = PluginManager->GetAnimationPreview(TextureStates[i], j, WidgetIndex);

			for (const TSharedPtr<FString>& File : CurrentAnimation->GetImageList())
			{
				if (!File.IsValid())
				{
					return false;
				}
				DirectionList.Add(*File);
			}

			FileList.Add(DirectionList);
		}

		ImportMap.Add(StateName, FileList);
	}

	//ImportMap = tempMap;
	ImportMap.GetKeys(TextureStates);

	UMiscOperations* operation = NewObject<UMiscOperations>();
	UFluffyTextureFactory* textureFactory = NewObject<UFluffyTextureFactory>();

	for (FString state : TextureStates)
	{
		TArray<TArray<FString>> fileList = ImportMap[state];

		//Making the texture for the state
		TArray<TArray<UTexture2D*>> stateList;
		for (int i = 0; i < fileList.Num(); i++)
		{
			TArray<UTexture2D*> DirectionList;
			TArray<FString> importFileList = fileList[i];

			for (int j = 0; j < importFileList.Num(); j++)
			{
				FString importFile = importFileList[j];

				FString newName= AnimationName.Replace(TEXT(" "), TEXT("_"))
					+ "_"
					+ state.Replace(TEXT(" "), TEXT("_"))
					+ "_";
				if (WidgetIndex == 3)
				{
					newName += operation->PadNumberString(j, FString::FromInt(importFileList.Num()).Len());
				}
				else 
				{
					newName += operation->GetDirection(i).Replace(TEXT(" "), TEXT("_"))
						+ operation->PadNumberString(j, FString::FromInt(importFileList.Num()).Len());
				}

				UTexture2D* texture = textureFactory->ImportTexture(ImportMap[state][i][j], TexturePath, newName);

				if (!texture)
				{
					return false;
				}
				DirectionList.Add(texture);
			}
			stateList.Add(DirectionList);
		}
		TextureMap.Add(state, stateList);
	}
	return true;
}

//==============================================================================================================================================
bool UAnimationConstructor::CreateSprites()
{
	//This process will have to change to work with sprite sheets
	//ToDo: Make the sprite creation work with sprite sheets

	TArray<FString> SpriteStates;
	TextureMap.GetKeys(SpriteStates);

	for (FString state : SpriteStates)
	{
		TArray<TArray<UTexture2D*>> textureList = TextureMap[state];


		TArray<TArray<UPaperSprite*>> stateList;
		for (int i = 0; i < textureList.Num(); i++)
		{
			TArray<UPaperSprite*> DirectionList;
			TArray<UTexture2D*> textureDirectionsList = textureList[i];

			for (int j = 0; j < textureDirectionsList.Num(); j++)
			{
						UFluffySpriteFactory* spriteFactory = NewObject<UFluffySpriteFactory>();
						spriteFactory->AddToRoot();

						UPaperSprite* sprite = spriteFactory->CreateSpriteFromTexture(textureList[i][j], SpritePathMap[state][i]);

						if (!sprite)
						{
							return false;
						}
						spriteFactory->RemoveFromRoot();

						DirectionList.Add(sprite);
			}
			stateList.Add(DirectionList);
		}
		SpriteMap.Add(state, stateList);
	}

	return true;
}

//==============================================================================================================================================
bool UAnimationConstructor::CreateFlipbooks(FString AnimationName, UPluginWindowManager* PluginManager, int32 WidgetIndex)
{
	TArray<FString> FlipbookStates;
	TArray<FString> StateList = PluginManager->GetAnimationStatesList(WidgetIndex);
	SpriteMap.GetKeys(FlipbookStates);

	UMiscOperations* operation = NewObject<UMiscOperations>();
	int32 StateCount = -1;

	for (FString state : FlipbookStates)
	{
		StateCount++;
		TArray<TArray<UPaperSprite*>> spriteList = SpriteMap[state];

		TArray<UPaperFlipbook*> stateList;
		for (int i = 0; i < spriteList.Num(); i++)
		{

			float framerate = PluginManager->GetAnimationPreview(StateList[StateCount], i, WidgetIndex)->GetFrameRate();
			//float framerate = 10.0f;

			FString newName = AnimationName.Replace(TEXT(" "), TEXT("_"));
			if (WidgetIndex != 3)
			{
				newName += operation->GetDirection(i).Replace(TEXT(" "), TEXT("_"));
			}

			UFluffyFlipbookFactory* flipbookFactory = NewObject<UFluffyFlipbookFactory>();
			flipbookFactory->AddToRoot();

			UPaperFlipbook* flipbook = flipbookFactory->CreateFlipbookFromSprites(spriteList[i], framerate, FlipbookPathMap[state][i], newName);

			if (!flipbook)
			{
				return false;
			}

			stateList.Add(flipbook);

			flipbookFactory->RemoveFromRoot();
		}

			FlipbookMap.Add(state, stateList);
	}

	return true;
}

//==============================================================================================================================================
bool UAnimationConstructor::CreateObjects(FString AnimationName)
{
	TArray<FString> ObjectStates;
	FlipbookMap.GetKeys(ObjectStates);

	UFluffyAnimationFactory* animationFactory = NewObject<UFluffyAnimationFactory>();
	animationFactory->AddToRoot();

	//First step is creating all of the base objects needed for the main object that is selected in the component
	for (const FString& state : ObjectStates)
	{

		TArray<UPaperFlipbook*> flipbookDirectionList = FlipbookMap[state];

		FString object = animationFactory->CreateAnimationObject(AnimationName + "_" + state, AnimationStatesPath, flipbookDirectionList);
		if (object == "")
		{
			return false;
		}
		ObjectMap.Add(state, object);
	}

	animationFactory->RemoveFromRoot();

	return true;
}

bool UAnimationConstructor::CreateMainObject(FString AnimationName)
{
	UFluffyAnimationFactory* animationFactory = NewObject<UFluffyAnimationFactory>();
	animationFactory->AddToRoot();

	//Second step is now to create the main object from the object map
	if (!animationFactory->CreateMainAnimationObject(AnimationName, AnimationPath, ObjectMap))
	{
		return false;
	}

	animationFactory->RemoveFromRoot();

	return true;
}