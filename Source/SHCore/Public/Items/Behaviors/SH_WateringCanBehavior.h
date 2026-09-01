// LightLine Studios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SH_BaseToolItemBehavior.h"
#include "SH_WateringCanBehavior.generated.h"

/**
 * 
 */
UCLASS()
class SHCORE_API USH_WateringCanBehavior : public USH_BaseToolItemBehavior
{
	GENERATED_BODY()
	
	virtual bool Use(const FSH_ItemUseContext& Context) override;
};
