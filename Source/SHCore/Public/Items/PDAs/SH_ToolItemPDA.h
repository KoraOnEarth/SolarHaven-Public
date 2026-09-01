// LightLine Studios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SH_ItemPDA.h"
#include "SH_ToolItemPDA.generated.h"

/**
 * 
 */
UCLASS()
class SHCORE_API USH_ToolItemPDA : public USH_ItemPDA
{
	GENERATED_BODY()
	
public:
	USH_ToolItemPDA()
		:SpawnActorClass(nullptr)
	{ ItemBehaviorClass = USH_BaseToolItemBehavior::StaticClass(); }
	
	// Determines which class this tool can spawn. 
	// For example, a hoe can spawn a plowed soil block (Soil).
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> SpawnActorClass;
};
