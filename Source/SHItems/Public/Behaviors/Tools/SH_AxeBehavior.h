// Copyright Lightline Studios. All rights reserved 

#pragma once

#include "CoreMinimal.h"
#include "SH_ToolBaseBehaviour.h"
#include "SH_AxeBehavior.generated.h"

/**
 * 
 */
UCLASS()
class SHITEMS_API USH_AxeBehavior : public USH_ToolBaseBehaviour
{
	GENERATED_BODY()

public:
	virtual void Use(const FSH_ItemUseContext& Context) override;
};
