// LightLine Studios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SH_BaseToolItemBehavior.h"

#include "Interfaces/SH_RotatablePreview.h"

#include "SH_BuildCertificateBehavior.generated.h"

/**
 * 
 */
UCLASS()
class SHCORE_API USH_BuildCertificateBehavior : public USH_BaseToolItemBehavior, public ISH_RotatablePreview
{
	GENERATED_BODY()
	
public:
	virtual void Tick(const FSH_ItemUseContext& Context) override;
	virtual bool Use(const FSH_ItemUseContext& Context) override;
	virtual void RotatePreview(int32 RotateDirection) override;
	
private:
	void UpdatePreviewActorLocation(const FSH_ItemUseContext& Context);
	
	UPROPERTY()
	FRotator CurrentRotation = FRotator::ZeroRotator;
};
