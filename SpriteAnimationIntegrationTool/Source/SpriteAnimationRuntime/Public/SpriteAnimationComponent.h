// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PaperFlipbookComponent.h"
#include "SpriteAnimationBase.h"
#include "MainSpriteObjectBase.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "SpriteAnimationComponent.generated.h"

UENUM(BlueprintType)
enum class EDirectionTypes : uint8
{
	OptionA UMETA(DisplayName = "2 Directions"),
	OptionB UMETA(DisplayName = "4 Directions"),
	OptionC UMETA(DisplayName = "8 Directions"),
	OptionD UMETA(DisplayName = "Flat Animation")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPRITEANIMATIONRUNTIME_API USpriteAnimationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpriteAnimationComponent();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Variables")
	FName SpriteTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Variables")
	EDirectionTypes DirectionCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component Variables")
	TSubclassOf<UMainSpriteObjectBase> SpriteAnimationObject;

private:

	UPROPERTY()
	UPaperFlipbookComponent* MainSprite;

	UPROPERTY()
	int32 StoredDirection = 1;

	UPROPERTY()
	UMainSpriteObjectBase* SpritesMap;

	UPROPERTY()
	TMap<FString, int32> TwoDirection = 
	{
		{"Left", 0},
		{"Right", 1},
		{"Up", -1},
		{"Down", -1},
		{"UpLeft", 0},
		{"DownLeft", 0},
		{"UpRight", 1},
		{"DownRight", 1}
	};

	UPROPERTY()
	TMap<FString, int32> FourDirection =
	{
		{"Left", 0},
		{"Right", 1},
		{"Up", 2},
		{"Down", 3},
		{"UpLeft", -1},
		{"DownLeft", -1},
		{"UpRight", -1},
		{"DownRight", -1}
	};

	UPROPERTY()
	TMap<FString, int32> EightDirection =
	{
		{"Left", 0},
		{"Right", 1},
		{"Up", 2},
		{"Down", 3},
		{"UpLeft", 4},
		{"DownLeft", 5},
		{"UpRight", 6},
		{"DownRight", 7}
	};

	UPROPERTY()
	APlayerCameraManager* CameraManager;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Actor State")
	void SetState(FName CurrentState);

	UFUNCTION(BlueprintCallable, Category = "Main Camera")
	void UpdateMainCamera();

private:

	UFUNCTION()
	void GetMainSpriteFromTag();

	UFUNCTION()
	int32 GetBaseMovement(FString Direction);

	UFUNCTION()
	void SetAnimationState(UPaperFlipbook* AnimationState);

	UFUNCTION()
	void GetAnimationState();

	UFUNCTION()
	FString GetDirection();

	UFUNCTION()
	UPaperFlipbook* GetFlipbookFromState(int32 DirectionIndex);

	//UFUNCTION()
	//int32 Get2DirIndex(FString Direction);

	//UFUNCTION()
	//int32 Get4DirIndex(FString Direction);

	//UFUNCTION()
	//int32 Get8DirIndex(FString Direction);

	UFUNCTION()
	int32 GetFlatIndex();

		
};
