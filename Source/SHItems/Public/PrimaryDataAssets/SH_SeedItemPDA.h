// Copyright Lightline Studios. All rights reserved 

#pragma once

#include "CoreMinimal.h"
#include "BaseClasses/SH_ItemPDA.h"
#include "SH_SeedItemPDA.generated.h"

#pragma once

USTRUCT(BlueprintType)
struct SHITEMS_API FSH_GrowVisualStages
{
	GENERATED_BODY()

public:
	TSoftObjectPtr<UStaticMesh> GetMeshAtStage(int Stage) const;
	int32 GetMaxStages() const { return Meshes.Num(); }

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TArray<TSoftObjectPtr<UStaticMesh>> Meshes;
};

/**
 * Эта структура хранит модификаторы скорости роста для каждого сезона
 */
USTRUCT(BlueprintType)
struct SHITEMS_API FSH_GrowSeasons
{
	GENERATED_BODY()

	FSH_GrowSeasons() :
		SpringGrowModifier(1.f),
		SummerGrowModifier(1.f),
		AutumnGrowModifier(1.f),
		WinterGrowModifier(1.f)
	{	}

	UPROPERTY(EditDefaultsOnly)
	float SpringGrowModifier;
	UPROPERTY(EditDefaultsOnly)
	float SummerGrowModifier;
	UPROPERTY(EditDefaultsOnly)
	float AutumnGrowModifier;
	UPROPERTY(EditDefaultsOnly)
	float WinterGrowModifier;
};

/**
 * 
 */
UCLASS()
class SHITEMS_API USH_SeedItemPDA : public USH_ItemPDA
{
	GENERATED_BODY()

public:
	USH_SeedItemPDA() :
		BasePriceToBuy(0),
		BasePriceToSell(0),
		TimeToGrowInMinutes(0)
	{	}
	
	UPROPERTY(EditDefaultsOnly)
	int32 BasePriceToBuy;

	// Цена продажи именно семян!!
	UPROPERTY(EditDefaultsOnly)
	int32 BasePriceToSell;

	UPROPERTY(EditDefaultsOnly)
	int32 TimeToGrowInMinutes;

	UPROPERTY(EditDefaultsOnly)
	FSH_GrowVisualStages GrowVisualStages;

	UPROPERTY(EditDefaultsOnly)
	FSH_GrowSeasons GrowSeasons;
};
