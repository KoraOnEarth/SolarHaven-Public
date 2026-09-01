// LightLine Studios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SH_RotatablePreview.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USH_RotatablePreview : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SHCORE_API ISH_RotatablePreview
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void RotatePreview(int32 RotateDirection) = 0;
};
