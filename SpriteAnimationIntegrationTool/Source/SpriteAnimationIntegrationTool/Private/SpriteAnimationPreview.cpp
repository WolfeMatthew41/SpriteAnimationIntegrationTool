// Copyright Matthew Wolfe All Rights Reserved.

#include "SpriteAnimationPreview.h"
#include "IDesktopPlatform.h"
#include "Widgets/Images/SImage.h"
#include "TimerManager.h"
#include "Editor.h"
#include "ImageUtils.h"
#include "Misc/MessageDialog.h"


void SSpriteAnimationPreview::Construct(const FArguments& InArgs)
{
	FrameRate = 10.f;
	CurrentFrame = 0;

	ChildSlot
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(2.0)
				[
					SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(SButton)
								.Text(FText::FromString(TEXT("Select Folder with Sprites")))
								.OnClicked(FOnClicked::CreateRaw(this, &SSpriteAnimationPreview::OnInputFolderSelectionButton))
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(SButton)
								.Text(FText::FromString(TEXT("Generate Sprite")))
								.OnClicked(FOnClicked::CreateRaw(this, &SSpriteAnimationPreview::OnGenerateAnimationSpriteButton))
						]
						+SVerticalBox::Slot()
						.AutoHeight()
						[
							SAssignNew(FolderPathList, SEditableTextBox)
							.Text(FText::FromString(TEXT("Folder Path")))
						]
				]
				+SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(STextBlock)
								.Text(FText::FromString(TEXT("Frame Rate:")))
						]
						+SVerticalBox::Slot()
						.AutoHeight()
						[
							SAssignNew(FrameRateBox, SEditableTextBox)
								.Text(FText::AsNumber(FrameRate))
						]
						+ SVerticalBox::Slot()
						//.AutoHeight()
						.FillHeight(200)
						[
							//Put image here
							SAssignNew(ImageWidget, SImage)
						]
						+SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(SHorizontalBox)
								+SHorizontalBox::Slot()
								.AutoWidth()
								[
									SNew(SButton)
										.Text(FText::FromString(TEXT("Test Animation")))
										.OnClicked(FOnClicked::CreateRaw(this, &SSpriteAnimationPreview::OnPlayButton))
								]
								+ SHorizontalBox::Slot()
								.AutoWidth()
								[
									SNew(SButton)
										.Text(FText::FromString(TEXT("Stop Test")))
										.OnClicked(FOnClicked::CreateRaw(this, &SSpriteAnimationPreview::OnStopButton))
								]
						]
				]
		];
}

FReply SSpriteAnimationPreview::OnInputFolderSelectionButton()
{
	IDesktopPlatform* desktopPlatform = FDesktopPlatformModule::Get();

	if (desktopPlatform)
	{
		FString folderName;
		const void* parentWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
		desktopPlatform->OpenDirectoryDialog(parentWindowHandle, TEXT("Select Folder"), TEXT(""), folderName);

		if (!folderName.IsEmpty())
		{
			//Assign to a fstring here for the folder path
			FolderPathList->SetText(FText::FromString(folderName));
			//GetImageFiles(folderName);
			return FReply::Handled();
		}

	}

	return FReply::Unhandled();
}

FReply SSpriteAnimationPreview::OnGenerateAnimationSpriteButton()
{
	//Check if folder path is correct here
	if (!FPaths::DirectoryExists(FolderPathList->GetText().ToString()))
	{
		//Make a pop up window appear here
		FText title = FText::FromString("Error");
		FText content = FText::FromString("That folder list didn't exist!");
		FMessageDialog::Open(EAppMsgType::Ok, content, &title);
		return FReply::Unhandled();
	}

	FrameRate = FCString::Atof(*FrameRateBox->GetText().ToString());

	if (FrameRate <= 0)
	{
		FrameRate = 1;
	}

	GetImageFiles(FolderPathList->GetText().ToString());


	return FReply::Handled();
}

void SSpriteAnimationPreview::GetImageFiles(const FString& folderPath)
{
	ImageList.Empty();

	TArray<FString> fileList;
	IFileManager& fileManager = IFileManager::Get();

	fileManager.FindFilesRecursive(fileList, *folderPath, TEXT("*.png"), true, false, false);
	fileManager.FindFilesRecursive(fileList, *folderPath, TEXT("*.jpg"), true, false, false);
	fileManager.FindFilesRecursive(fileList, *folderPath, TEXT("*.jpeg"), true, false, false);

	fileList.Sort();

	for (const FString& image : fileList)
	{
		ImageList.Add(MakeShareable(new FString(image)));
	}

	OnStopButton();
	CurrentFrame = 0;
	UpdateAnimation();
}

void SSpriteAnimationPreview::StartAnimation() 
{
	if (ImageList.Num() > 0)
	{
		CurrentFrame = 0;

		TWeakPtr<SSpriteAnimationPreview> weakSelf = SharedThis(this);

		if (GEditor && GEditor->GetEditorWorldContext().World())
		{
			GEditor->GetEditorWorldContext().World()->GetTimerManager()
				.SetTimer(AnimationTimerHandle, [weakSelf]()
			{
						if (weakSelf.IsValid())
						{
							weakSelf.Pin()->UpdateAnimation();
						}
			}, 1.0f / FrameRate, true
			);
		}
	}
}

FReply SSpriteAnimationPreview::OnPlayButton()
{
	StartAnimation();
	return FReply::Handled();
}

FReply SSpriteAnimationPreview::OnStopButton()
{
	if (GEditor && GEditor->GetEditorWorldContext().World())
	{
		GEditor->GetEditorWorldContext().World()->GetTimerManager().ClearTimer(AnimationTimerHandle);
	}
	return FReply::Handled();
}

void SSpriteAnimationPreview::UpdateAnimation()
{
	if (ImageList.Num() > 0)
	{
		UTexture2D* loadedTexture = nullptr;

		loadedTexture = FImageUtils::ImportFileAsTexture2D(**ImageList[CurrentFrame]);

		if (loadedTexture)
		{
			FSlateBrush* brush = new FSlateBrush();
			brush->SetResourceObject(loadedTexture);
			brush->ImageSize = GetImageSize(loadedTexture);
			ImageWidget->SetImage(brush);
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load %s"), **ImageList[CurrentFrame]);
		}

		CurrentFrame = (CurrentFrame + 1) % ImageList.Num();
	}
}

FVector2D SSpriteAnimationPreview::GetImageSize(UTexture2D* texture)
{
	float Width = texture->GetSizeX();
	float Height = texture->GetSizeY();

	float AspectRatio = Width / Height;

	float MaxHeight = 200.0f;
	float MaxWidth = 200.0f;



	if (Width > MaxWidth)
	{
		Width = MaxWidth;
		Height = Width / AspectRatio;
	}

	if (Height > MaxHeight)
	{
		Height = MaxHeight;
		Width = Height * AspectRatio;
	}

	return FVector2D(Width, Height);
}

FString SSpriteAnimationPreview::GetFolderPath() const
{
	return FolderPathList->GetText().ToString();
}

float SSpriteAnimationPreview::GetFrameRate() const
{
	return FrameRate;
}

TArray<TSharedPtr<FString>> SSpriteAnimationPreview::GetImageList() const
{
	return ImageList;
}