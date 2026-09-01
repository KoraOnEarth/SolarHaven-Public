// LightLine Studios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "SH_ItemPDA.generated.h"

class USH_BaseItemBehavior;

/**
 * Base class for all items
 */
UCLASS()
class SHCORE_API USH_ItemPDA : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	USH_ItemPDA() :
		Name(FText::GetEmpty()),
		Description(FText::GetEmpty()),
		MaxStackSize(1),
		Icon(nullptr),
		StaticMesh(nullptr)
	{	}
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Item", ItemID);
	};
	
	UPROPERTY(EditDefaultsOnly)
	FName ItemID;
	UPROPERTY(EditDefaultsOnly)
	FText Name;
	UPROPERTY(EditDefaultsOnly)
	FText Description;
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1", ClampMax = "1024"))
	int32 MaxStackSize;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> Icon;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMesh> StaticMesh;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USH_BaseItemBehavior> ItemBehaviorClass;
};
