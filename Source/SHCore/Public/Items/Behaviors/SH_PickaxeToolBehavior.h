// LightLine Studios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SH_BaseToolItemBehavior.h"
#include "SH_PickaxeToolBehavior.generated.h"

/**
 * 
 */
UCLASS()
class SHCORE_API USH_PickaxeToolBehavior : public USH_BaseToolItemBehavior
{
	GENERATED_BODY()
	
public:
	virtual void Init(AActor* PlayerCharacter, USH_ItemPDA* ItemPDA) override;
	virtual bool Use(const FSH_ItemUseContext& Context) override;
};
