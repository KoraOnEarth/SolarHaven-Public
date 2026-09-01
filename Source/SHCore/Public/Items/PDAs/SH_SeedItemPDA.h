// LightLine Studios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SH_ItemPDA.h"

#include "Items/Behaviors/SH_BaseSeedBehavior.h"
#include "Items/Structs/SH_SeedItemStructs.h"

#include "SH_SeedItemPDA.generated.h"

class USH_CropItemPDA;

/**
 * 
 */
UCLASS()
class SHCORE_API USH_SeedItemPDA : public USH_ItemPDA
{
	GENERATED_BODY()
	
public:
	USH_SeedItemPDA() { ItemBehaviorClass = USH_BaseSeedBehavior::StaticClass(); }
	
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin = 1))
	int32 BasePriceToBuy;
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin = 1))
	int32 BasePriceToSell;
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin = 1))
	int32 TimeToGrowInMinutes;
	UPROPERTY(EditDefaultsOnly)
	FSH_GrowVisualStages GrowVisualStages;
	UPROPERTY(EditDefaultsOnly)
	FSH_GrowSeasons GrowSeasons;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USH_CropItemPDA> CropItemPDA;
	UPROPERTY(EditDefaultsOnly, meta=(ClampMin = 1, ClampMax = 500))
	int32 CountOfCrops = 1;
};
