// Copyright Lightline Studios. All rights reserved 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SH_ItemLibrary.generated.h"

class USH_ItemPDA;
/**
 * 
 */
UCLASS()
class SHITEMS_API USH_ItemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION()
	static USH_ItemPDA* GetItemByID(FName ItemID);
};
