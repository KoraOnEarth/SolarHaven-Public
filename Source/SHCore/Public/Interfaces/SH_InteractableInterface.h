// Copyright Lightline Studios. All rights reserved 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SH_InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USH_InteractableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SHCORE_API ISH_InteractableInterface
{
	GENERATED_BODY()

public:
	virtual void InteractAction() = 0;
};
