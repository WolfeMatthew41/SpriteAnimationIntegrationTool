// Copyright Matthew Wolfe All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "SpriteAnimationIntegrationToolStyle.h"

class FSpriteAnimationIntegrationToolCommands : public TCommands<FSpriteAnimationIntegrationToolCommands>
{
public:

	FSpriteAnimationIntegrationToolCommands()
		: TCommands<FSpriteAnimationIntegrationToolCommands>(TEXT("SpriteAnimationIntegrationTool"), NSLOCTEXT("Contexts", "SpriteAnimationIntegrationTool", "SpriteAnimationIntegrationTool Plugin"), NAME_None, FSpriteAnimationIntegrationToolStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};