// Copyright Lightline Studios. All rights reserved 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SH_ItemPDA.generated.h"

/**
 * 
 */
UCLASS()
class SHITEMS_API USH_ItemPDA : public UPrimaryDataAsset
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

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UPROPERTY(EditDefaultsOnly)
	FName ItemID;
	
	UPROPERTY(EditDefaultsOnly)
	FText Name;

	UPROPERTY(EditDefaultsOnly)
	FText Description;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxStackSize;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USH_ItemBaseBehavior> ItemBehaviorClass;
};
