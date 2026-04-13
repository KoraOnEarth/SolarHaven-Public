// Copyright Lightline Studios. All rights reserved 

#pragma once

#include "CoreMinimal.h"
#include "BaseClasses/SH_ItemPDA.h"
#include "SH_ToolItemPDA.generated.h"

/**
 * 
 */
UCLASS()
class SHITEMS_API USH_ToolItemPDA : public USH_ItemPDA
{
	GENERATED_BODY()

public:
	USH_ToolItemPDA() :
		SpawnActorClass(nullptr)
	{	}
	
	// Нужен для определения того, что этот класс может спавнить.
	// Например, мотыга (hoe) может спавнить BaseSoil (распаханная земля)
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> SpawnActorClass;
};
