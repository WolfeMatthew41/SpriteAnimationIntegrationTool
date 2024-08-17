// Fill out your copyright notice in the Description page of Project Settings.


#include "SpriteAnimationComponent.h"

// Sets default values for this component's properties
USpriteAnimationComponent::USpriteAnimationComponent()
{
	GetMainSpriteFromTag();

	if (SpriteAnimationObject)
	{
		SpritesMap = Cast<UMainSpriteObjectBase>(SpriteAnimationObject->GetDefaultObject());
	}
	
	
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpriteAnimationComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	GetMainSpriteFromTag();

	if (SpriteAnimationObject)
	{
		SpritesMap = Cast<UMainSpriteObjectBase>(SpriteAnimationObject->GetDefaultObject());
	}

	UpdateMainCamera();
}


// Called every frame
void USpriteAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	GetAnimationState();
}

int32 USpriteAnimationComponent::GetBaseMovement(FString Direction)
{
	switch (DirectionCount)
	{
	case EDirectionTypes::OptionA:
		return TwoDirection[Direction];
	case EDirectionTypes::OptionB:
		return FourDirection[Direction];
	case EDirectionTypes::OptionC:
		return EightDirection[Direction];
	case EDirectionTypes::OptionD:
		return GetFlatIndex();
	}

	return 0;
}

void USpriteAnimationComponent::SetState(FName CurrentState) 
{
	if (SpritesMap)
	{
		SpritesMap->SpriteState = CurrentState;
	}
}

void USpriteAnimationComponent::UpdateMainCamera() 
{
	CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
}

void USpriteAnimationComponent::GetMainSpriteFromTag()
{
	if (SpriteTag.IsNone())
	{
		return;
	}
	MainSprite = Cast<UPaperFlipbookComponent>(GetOwner()->GetComponentsByTag(UPaperFlipbookComponent::StaticClass(), SpriteTag)[0]);
}

void USpriteAnimationComponent::SetAnimationState(UPaperFlipbook* AnimationState)
{
	if (MainSprite)
	{
		if (MainSprite->GetFlipbook() != AnimationState)
		{
			MainSprite->SetFlipbook(AnimationState);
		}
	}
}

void USpriteAnimationComponent::GetAnimationState()
{
	FString Direction = GetDirection();
	
	int32 DirectionIndex = GetBaseMovement(Direction);

	UPaperFlipbook* AnimationState = GetFlipbookFromState(DirectionIndex);

	SetAnimationState(AnimationState);
}

FString USpriteAnimationComponent::GetDirection()
{
	if (!CameraManager)
	{
		UpdateMainCamera();
	}
	if (!CameraManager)
	{
		return "Left";
	}

	FVector DirectionToCamera = CameraManager->GetCameraLocation() - GetOwner()->GetActorLocation();
	DirectionToCamera.Z = 0;
	DirectionToCamera.Normalize();

	float AngleToCamera = atan2f(DirectionToCamera.Y, DirectionToCamera.X) * (180.0f / PI);

	FVector Direction = Cast<UArrowComponent>(GetOwner()->GetComponentByClass(UArrowComponent::StaticClass()))->GetForwardVector();
	float ArrowAngle = atan2f(Direction.Y, Direction.X) * (180.0 / PI);

	float Angle = (ArrowAngle + 90) - (AngleToCamera);

	if (Angle > 180.0f)
	{
		Angle -= 360.0f;
	}
	else if (Angle < -180.0f)
	{
		Angle += 360.0f;
	}

	//Right
	if (Angle >= -22.5 && Angle <= 22.5)
	{
		return "Right";
	}
	//UpRight
	if (Angle >= -67.5 && Angle <= -22.5)
	{
		return "UpRight";
	}
	//Up
	if (Angle >= -112.5 && Angle <= -67.5)
	{
		return "Up";
	}
	//UpLeft
	if (Angle >= -157.5 && Angle <= -112.5)
	{
		return "UpLeft";
	}
	//DownRight
	if (Angle >= 22.5 && Angle <= 67.5)
	{
		return "DownRight";
	}
	//Down
	if (Angle >= 67.5 && Angle <= 112.5)
	{
		return "Down";
	}
	//DownLeft
	if (Angle >= 112.5 && Angle <= 157.5)
	{
		return "DownLeft";
	}
	return "Left";
}

UPaperFlipbook* USpriteAnimationComponent::GetFlipbookFromState(int32 DirectionIndex)
{
	UPaperFlipbook* CurrentFlipbook = nullptr;

	int32 Index = DirectionIndex;

	if (DirectionIndex < 0)
	{
		Index = StoredDirection;
	}

	if (SpritesMap)
	{
		TArray<UPaperFlipbook*> SpritesCollection = SpritesMap->GetSpriteCollection();

		if (SpritesCollection.Num() > 0)
		{
				CurrentFlipbook = SpritesCollection[Index];
		}
		//In case no states are being managed, should use base movements
		else
		{
			if (DirectionCount == EDirectionTypes::OptionD)
			{
				SetState("Flat_Animation");
				SpritesCollection = SpritesMap->GetSpriteCollection();
				if (SpritesCollection.Num() > 0)
				{
					CurrentFlipbook = SpritesCollection[Index];
				}
			}
			else if (GetOwner()->GetVelocity().Length() > 0.0f)
			{
				SetState("Move");
				SpritesCollection = SpritesMap->GetSpriteCollection();
				if (SpritesCollection.Num() > 0)
				{
					CurrentFlipbook = SpritesCollection[Index];
				}
			}
			else
			{
				SetState("Idle");
				SpritesCollection = SpritesMap->GetSpriteCollection();
				if (SpritesCollection.Num() > 0)
				{
					CurrentFlipbook = SpritesCollection[Index];
				}
			}
		}
	}

	StoredDirection = Index;
	return CurrentFlipbook;
}

int32 USpriteAnimationComponent::GetFlatIndex()
{
	return 0;
}