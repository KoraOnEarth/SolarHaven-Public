// LightLine Studios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SH_BaseItemBehavior.h"
#include "SH_BaseSeedBehavior.generated.h"

/**
 * 
 */
UCLASS()
class SHCORE_API USH_BaseSeedBehavior : public USH_BaseItemBehavior
{
	GENERATED_BODY()
	
	virtual void Init(AActor* PlayerCharacter, USH_ItemPDA* ItemPDA) override;
	virtual bool Use(const FSH_ItemUseContext& Context) override;
};
