// LightLine Studios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SH_BuildableGridInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USH_BuildableGridInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * This interface is used to interact with the building grid (SH_GridSystem)
 */
class SHCORE_API ISH_BuildableGridInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FVector GetLocationToBuild(const FVector& HitLocation, const FRotator& CharacterRotation, int32 xSize = 1, int32 ySize = 1) = 0;
	virtual bool GetIsCellOccupied(const FVector& HitLocation, int32 xSize = 1, int32 ySize = 1) = 0; 
	virtual void SetCellOccupation(const FVector& HitLocation, bool bIsOccupied, int32 xSize = 1, int32 ySize = 1) = 0;
};
