// Copyright Matthew Wolfe All Rights Reserved.

#include "SpriteAnimationIntegrationTool.h"
#include "SpriteAnimationIntegrationToolStyle.h"
#include "SpriteAnimationIntegrationToolCommands.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "Widgets/Layout/SScrollBox.h"
#include "ToolMenus.h"
#include "MiscOperations.h"
#include "AnimationConstructor.h"

static const FName SpriteAnimationIntegrationToolTabName("SpriteAnimationIntegrationTool");

#define LOCTEXT_NAMESPACE "FSpriteAnimationIntegrationToolModule"

void FSpriteAnimationIntegrationToolModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FSpriteAnimationIntegrationToolStyle::Initialize();
	FSpriteAnimationIntegrationToolStyle::ReloadTextures();

	FSpriteAnimationIntegrationToolCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FSpriteAnimationIntegrationToolCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FSpriteAnimationIntegrationToolModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FSpriteAnimationIntegrationToolModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(SpriteAnimationIntegrationToolTabName, FOnSpawnTab::CreateRaw(this, &FSpriteAnimationIntegrationToolModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FSpriteAnimationIntegrationToolTabTitle", "SpriteAnimationIntegrationTool"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);


}

void FSpriteAnimationIntegrationToolModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FSpriteAnimationIntegrationToolStyle::Shutdown();

	FSpriteAnimationIntegrationToolCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(SpriteAnimationIntegrationToolTabName);


}

TSharedRef<SDockTab> FSpriteAnimationIntegrationToolModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	DirectionSwitcherIndex = 0;
	DirectionalFrameRateBoxCount = 8;

	if (!PluginManager)
	{
		PluginManager = NewObject<UPluginWindowManager>();
		PluginManager->AddToRoot();
	}
	PluginManager->SpriteSelectionConstruct(DirectionalFrameRateBoxCount);

	TSharedRef<SDockTab> pluginWindow = SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SButton)
								.Text(FText::FromString(TEXT("2 Directions")))
								.OnClicked_Lambda([=]() -> FReply {
									DirectionSwitcherIndex = 0;
									PluginManager->SetWidgetIndex(0);
									//if (directionCountSwitcher.IsValid())
									//{
										//directionCountSwitcher->SetActiveWidgetIndex(0);
									//}
									return FReply::Handled();
								})
						]
						 //Setting widget switcher button tab for index 1
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SButton)
								.Text(FText::FromString(TEXT("4 Directions")))
								.OnClicked_Lambda([=]() -> FReply {
								DirectionSwitcherIndex = 1;
								PluginManager->SetWidgetIndex(1);
								//if (directionCountSwitcher.IsValid())
								//{
									//directionCountSwitcher->SetActiveWidgetIndex(1);
								//}
								return FReply::Handled();
									})
						]
						
						//Setting widget switcher button tab for index 2
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SButton)
								.Text(FText::FromString(TEXT("8 Directions")))
								.OnClicked_Lambda([=]() -> FReply {
								DirectionSwitcherIndex = 2;
								PluginManager->SetWidgetIndex(2);
								//if (directionCountSwitcher.IsValid())
								//{
									//directionCountSwitcher->SetActiveWidgetIndex(1);
								//}
								return FReply::Handled();
									})
						]

						//Setting widget switcher button tab for index 3
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SButton)
								.Text(FText::FromString(TEXT("Flat Sequence")))
								.OnClicked_Lambda([=]() -> FReply {
								DirectionSwitcherIndex = 3;
								PluginManager->SetWidgetIndex(3);
								//if (directionCountSwitcher.IsValid())
								//{
									//directionCountSwitcher->SetActiveWidgetIndex(1);
								//}
								return FReply::Handled();
									})
						]
				]
				+ SVerticalBox::Slot()
				.FillHeight(1.0f)
				[
					SNew(SScrollBox)
						.Orientation(Orient_Vertical)
						+ SScrollBox::Slot()
						[
									//SAssignNew(directionCountSwitcher, SWidgetSwitcher)
									SNew(SWidgetSwitcher)
										//.WidgetIndex(this, &FSpriteAnimationIntegrationToolModule::GetDirectionSwitcherIndex)
										.WidgetIndex_Lambda([this]() {
										return DirectionSwitcherIndex;
											})
										//Widget Switcher index 0
												+ SWidgetSwitcher::Slot()
												[
													SNew(SVerticalBox)
														+SVerticalBox::Slot()
														.AutoHeight()
														[
															//SAssignNew(twoDirSpriteSelectionContainer, SVerticalBox)
															PluginManager->GetTwoDirContainer().ToSharedRef()
																//SNew(SVerticalBox)
														]
														+ SVerticalBox::Slot()
														.AutoHeight()
														[
															PluginManager->GetTwoDirCustomContainer().ToSharedRef()
															//SAssignNew(twoDirCustomStatesContainer, SVerticalBox)
														]
														+ SVerticalBox::Slot()
														.AutoHeight()
														[
															SNew(SButton)
																.Text(FText::FromString(TEXT("Add Custom State")))
																.OnClicked(FOnClicked::CreateRaw(this, &FSpriteAnimationIntegrationToolModule::OnAddCustomStateButton))
														]

												]
												//Widget Switcher index 1
												+ SWidgetSwitcher::Slot()
												[
													SNew(SVerticalBox)
														+SVerticalBox::Slot()
														.AutoHeight()
														[
															PluginManager->GetFourDirContainer().ToSharedRef()
														]
														+ SVerticalBox::Slot()
														.AutoHeight()
														[
															PluginManager->GetFourDirCustomContainer().ToSharedRef()
														]
														+ SVerticalBox::Slot()
														.AutoHeight()
														[
															SNew(SButton)
																.Text(FText::FromString(TEXT("Add Custom State")))
																.OnClicked(FOnClicked::CreateRaw(this, &FSpriteAnimationIntegrationToolModule::OnAddCustomStateButton))
														]

												]
												
												//Widget Switcher index 2
												+ SWidgetSwitcher::Slot()
												[
													SNew(SVerticalBox)
														+SVerticalBox::Slot()
														.AutoHeight()
														[
															PluginManager->GetEightDirContainer().ToSharedRef()
														]
														+ SVerticalBox::Slot()
														.AutoHeight()
														[
															PluginManager->GetEightDirCustomContainer().ToSharedRef()
														]
														+ SVerticalBox::Slot()
														.AutoHeight()
														[
															SNew(SButton)
																.Text(FText::FromString(TEXT("Add Custom State")))
																.OnClicked(FOnClicked::CreateRaw(this, &FSpriteAnimationIntegrationToolModule::OnAddCustomStateButton))
														]

												]
												
												//Widget Switcher index 3
												+ SWidgetSwitcher::Slot()
												[
													SNew(SVerticalBox)
														+SVerticalBox::Slot()
														.AutoHeight()
														[
															PluginManager->GetFlatContainer().ToSharedRef()
														]
														+ SVerticalBox::Slot()
														.AutoHeight()
														[
															PluginManager->GetFlatCustomContainer().ToSharedRef()
														]
														+ SVerticalBox::Slot()
														.AutoHeight()
														[
															SNew(SButton)
																.Text(FText::FromString(TEXT("Add Custom State")))
																.OnClicked(FOnClicked::CreateRaw(this, &FSpriteAnimationIntegrationToolModule::OnAddCustomStateButton))
														]

												]
												
						]
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SBox)
						.HAlign(HAlign_Left)
						.VAlign(VAlign_Center)
						[
							SNew(STextBlock)
								.Text(FText::FromString(TEXT("Output Location")))
						]
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SAssignNew(OutputPathBox, SEditableTextBox)
							//SNew(STextBlock)
								.Text(FText::FromString(TEXT("Output Location")))
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SButton)
								.Text(FText::FromString(TEXT("Select Output Path")))
								.OnClicked(FOnClicked::CreateRaw(this, &FSpriteAnimationIntegrationToolModule::OnOutputFolderSelectionButton))
						]
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SBox)
						.HAlign(HAlign_Left)
						.VAlign(VAlign_Center)
						[
							SNew(STextBlock)
								.Text(FText::FromString(TEXT("Name")))
						]
				]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
							SAssignNew(AnimationNameBox, SEditableTextBox)
							//SNew(STextBlock)
								.Text(FText::FromString(TEXT("Name")))
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SBox)
							.HAlign(HAlign_Right)
							.VAlign(VAlign_Center)
							[
								SNew(SButton)
									.Text(FText::FromString(TEXT("Create Animation")))
									.OnClicked(FOnClicked::CreateRaw(this, &FSpriteAnimationIntegrationToolModule::OnCreateAnimationButton))
							]
					]
		];

		return pluginWindow;
		//return pluginWindow.ToSharedRef();
}

FReply FSpriteAnimationIntegrationToolModule::OnAddCustomStateButton()
{
	PluginManager->AddCustomState(DirectionalFrameRateBoxCount);
	return FReply::Handled();
}

FReply FSpriteAnimationIntegrationToolModule::OnOutputFolderSelectionButton()
{
	UMiscOperations* operation = NewObject<UMiscOperations>();

	OutputPathBox->SetText(FText::FromString(operation->OutputFolderSelect()));

	return FReply::Handled();
}

bool FSpriteAnimationIntegrationToolModule::CheckAnimationCreatability() 
{

	//Check if the output path has an actual path in the projects directory
	FString selectedPath = OutputPathBox->GetText().ToString();
	if (!selectedPath.Contains(FPaths::ProjectContentDir()))
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("That output folder does not exist in the projects directory!")));
		return false;
	}

	//Go through the custom animations and make sure there is no duplicate name
	TSet<FString> uniqueValues;
	for (int i = 0; i < PluginManager->GetCustomAnimationNameListSize(DirectionSwitcherIndex); i++)
	{
		FString animationName = PluginManager->GetCustomAnimationName(i, DirectionSwitcherIndex);
		if (uniqueValues.Contains(animationName))
		{
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("There is a duplicate custom state name! Make sure they are all unique!")));
			return false;
		}
		uniqueValues.Add(animationName);
	}

	return true;
}

FReply FSpriteAnimationIntegrationToolModule::OnCreateAnimationButton()
{
	if (!CheckAnimationCreatability())
	{
		return FReply::Handled();
	}
	
	UAnimationConstructor* animationConstruct = NewObject<UAnimationConstructor>();
	animationConstruct->AddToRoot();

	animationConstruct->OnCreateAnimation(DirectionSwitcherIndex, AnimationNameBox->GetText().ToString(),
	OutputPathBox->GetText().ToString(), PluginManager);

	PluginManager->FullReset();

	PluginManager->SpriteSelectionConstruct(DirectionalFrameRateBoxCount);

	animationConstruct->RemoveFromRoot();

	return FReply::Handled();
}

void FSpriteAnimationIntegrationToolModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(SpriteAnimationIntegrationToolTabName);
}

void FSpriteAnimationIntegrationToolModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FSpriteAnimationIntegrationToolCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FSpriteAnimationIntegrationToolCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSpriteAnimationIntegrationToolModule, SpriteAnimationIntegrationTool)