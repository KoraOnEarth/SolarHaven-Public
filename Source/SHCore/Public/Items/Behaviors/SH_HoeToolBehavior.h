// LightLine Studios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SH_BaseToolItemBehavior.h"
#include "SH_HoeToolBehavior.generated.h"

class ASH_BuildPreview;

/**
 * 
 */
UCLASS()
class SHCORE_API USH_HoeToolBehavior : public USH_BaseToolItemBehavior
{
	GENERATED_BODY()
	
public:
	virtual void Tick(const FSH_ItemUseContext& Context) override;
	virtual bool Use(const FSH_ItemUseContext& Context) override;
	
private:	
	void UpdatePreviewActorLocation(const FSH_ItemUseContext& Context);
	void CreateSoilBlock(const FSH_ItemUseContext& Context);
};
