// LightLine Studios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SH_ItemPDA.h"

#include "Items/Behaviors/SH_BaseCropBehavior.h"

#include "SH_CropItemPDA.generated.h"

/**
 * 
 */
UCLASS()
class SHCORE_API USH_CropItemPDA : public USH_ItemPDA
{
	GENERATED_BODY()
	
public:
	USH_CropItemPDA() { ItemBehaviorClass = USH_BaseCropBehavior::StaticClass(); }
	
	UPROPERTY(EditDefaultsOnly)
	int32 BasePriceToBuy;
	UPROPERTY(EditDefaultsOnly)
	int32 BasePriceToSell;
};
