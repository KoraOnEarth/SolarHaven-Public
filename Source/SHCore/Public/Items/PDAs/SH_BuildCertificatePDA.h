// LightLine Studios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SH_ToolItemPDA.h"

#include "SH_BuildCertificatePDA.generated.h"

/**
 * 
 */
UCLASS()
class SHCORE_API USH_BuildCertificatePDA : public USH_ToolItemPDA
{
	GENERATED_BODY()
	
public:
	FIntVector2 GetSize() const { return {xSize, ySize}; }
private:
	UPROPERTY(EditDefaultsOnly)
	int32 xSize = 1;
	UPROPERTY(EditDefaultsOnly)
	int32 ySize = 1;
};
