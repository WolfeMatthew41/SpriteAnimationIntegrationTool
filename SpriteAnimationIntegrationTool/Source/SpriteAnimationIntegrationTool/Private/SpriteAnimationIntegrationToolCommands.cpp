// Copyright Matthew Wolfe All Rights Reserved.

#include "SpriteAnimationIntegrationToolCommands.h"

#define LOCTEXT_NAMESPACE "FSpriteAnimationIntegrationToolModule"

void FSpriteAnimationIntegrationToolCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "SpriteAnimationIntegrationTool", "Bring up SpriteAnimationIntegrationTool window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
