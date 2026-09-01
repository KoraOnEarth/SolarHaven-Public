// LightLine Studios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SH_PlantableInterface.generated.h"

class USH_SeedItemPDA;
class USH_ItemPDA;

// This class does not need to be modified.
UINTERFACE()
class USH_PlantableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SHCORE_API ISH_PlantableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool Plant(USH_SeedItemPDA* SeedItemPDA) = 0;
	virtual USH_ItemPDA* Crop() = 0;
	virtual void ToWater() = 0;
};
