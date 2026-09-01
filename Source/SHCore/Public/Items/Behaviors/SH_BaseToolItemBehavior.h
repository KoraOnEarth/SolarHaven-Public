// LightLine Studios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SH_BaseItemBehavior.h"
#include "SH_BaseToolItemBehavior.generated.h"

/**
 * 
 */
UCLASS()
class SHCORE_API USH_BaseToolItemBehavior : public USH_BaseItemBehavior
{
	GENERATED_BODY()
	
public:
	virtual void Init(AActor* PlayerCharacter, USH_ItemPDA* ItemPDA) override;
	virtual bool Use(const FSH_ItemUseContext& Context) override;
};
