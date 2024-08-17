// Copyright Matthew Wolfe All Rights Reserved.

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "PluginWindowManager.h"

class FSpriteAnimationIntegrationToolModule : public IModuleInterface,
	public TSharedFromThis<FSpriteAnimationIntegrationToolModule>
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

	void PluginButtonClicked();

	void RegisterMenus();

	FReply OnOutputFolderSelectionButton();

	bool CheckAnimationCreatability();

	FReply OnCreateAnimationButton();

	FReply OnAddCustomStateButton();

	int DirectionSwitcherIndex = 0;
	int DirectionalFrameRateBoxCount = 8;

	UPluginWindowManager* PluginManager;


	TSharedPtr<SEditableTextBox> OutputPathBox;
	TSharedPtr<SEditableTextBox> AnimationNameBox;

private:

	TSharedPtr<FUICommandList> PluginCommands;

	TSharedRef<SDockTab> OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs);

	
};

