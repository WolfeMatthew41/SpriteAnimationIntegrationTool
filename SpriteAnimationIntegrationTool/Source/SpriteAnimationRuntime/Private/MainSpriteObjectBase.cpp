// Fill out your copyright notice in the Description page of Project Settings.


#include "MainSpriteObjectBase.h"

UMainSpriteObjectBase::UMainSpriteObjectBase() 
{

}

TArray<UPaperFlipbook*> UMainSpriteObjectBase::GetSpriteCollection()
{
	TArray<UPaperFlipbook*> SpriteAnimation;
	if (SpriteCollection.Find(SpriteState))
	{
		USpriteAnimationBase* SpriteObject = SpriteCollection.Find(SpriteState)->GetDefaultObject();

		if (SpriteObject)
		{
			SpriteAnimation =  SpriteObject->SpriteAnimations;
		}
	}
	return SpriteAnimation;
}