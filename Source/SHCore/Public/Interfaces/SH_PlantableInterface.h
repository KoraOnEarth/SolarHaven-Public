// Copyright Lightline Studios. All rights reserved 

#pragma once

#include "CoreMinimal.h"
#include "Enums/SH_SnapDirection.h"
#include "UObject/Interface.h"
#include "SH_PlantableInterface.generated.h"

class USH_SeedItemPDA;

// This class does not need to be modified.
UINTERFACE()
class USH_PlantableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Интерфейс можно читать как "доступный для посева".
 * Подразумевается, что те, кто имеют этот интерфейс, могут содержать в себе семена и выращивать их
 */
class SHCORE_API ISH_PlantableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool PlantSeed(USH_SeedItemPDA* SeedItem) = 0;
	
	virtual bool IsOccupiedSlotByDirection(ESH_SnapDirection Direction) = 0;
	virtual void SetOccupiedSlotByDirection(ESH_SnapDirection Direction) = 0;
	virtual FVector GetTransformToPlaceSoil(ESH_SnapDirection Direction) = 0;
};
