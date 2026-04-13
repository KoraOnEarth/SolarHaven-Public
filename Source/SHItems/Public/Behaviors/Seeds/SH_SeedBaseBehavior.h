// Copyright Lightline Studios. All rights reserved 

#pragma once

#include "CoreMinimal.h"
#include "BaseClasses/SH_ItemBaseBehavior.h"
#include "SH_SeedBaseBehavior.generated.h"

/**
 * 
 */
UCLASS()
class SHITEMS_API USH_SeedBaseBehavior : public USH_ItemBaseBehavior
{
	GENERATED_BODY()

public:
	virtual void Init(AActor* InPlayerCharacter, USH_ItemPDA* InItemPDA) override;
	virtual void Use(const FSH_ItemUseContext& Context) override;
};
