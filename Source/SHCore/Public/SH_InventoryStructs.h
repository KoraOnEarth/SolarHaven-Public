#pragma once

#include "CoreMinimal.h"

#include "SH_InventoryStructs.generated.h"

class USH_ItemPDA;

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()
	
	bool IsEmpty() const {	return Item == nullptr;	}
	void Clear() {	Item = nullptr;	Quantity = 0;	}
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USH_ItemPDA> Item;
	
	UPROPERTY(VisibleInstanceOnly)
	int32 Quantity;
};