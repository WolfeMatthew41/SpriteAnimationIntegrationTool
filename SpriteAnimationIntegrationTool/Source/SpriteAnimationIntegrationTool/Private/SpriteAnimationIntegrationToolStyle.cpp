// Copyright Matthew Wolfe All Rights Reserved.

#include "SpriteAnimationIntegrationToolStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FSpriteAnimationIntegrationToolStyle::StyleInstance = nullptr;

void FSpriteAnimationIntegrationToolStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FSpriteAnimationIntegrationToolStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FSpriteAnimationIntegrationToolStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("SpriteAnimationIntegrationToolStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FSpriteAnimationIntegrationToolStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("SpriteAnimationIntegrationToolStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("SpriteAnimationIntegrationTool")->GetBaseDir() / TEXT("Resources"));

	Style->Set("SpriteAnimationIntegrationTool.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));

	return Style;
}

void FSpriteAnimationIntegrationToolStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FSpriteAnimationIntegrationToolStyle::Get()
{
	return *StyleInstance;
}
