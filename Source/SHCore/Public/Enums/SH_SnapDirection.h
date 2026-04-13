#pragma once

#include "CoreMinimal.h"

UENUM()
enum class ESH_SnapDirection : uint8
{
	None = 0    UMETA(DisplayName = "None"),
	Top         UMETA(DisplayName = "Top"),
	Bottom      UMETA(DisplayName = "Bottom"),
	Left        UMETA(DisplayName = "Left"),
	Right       UMETA(DisplayName = "Right")
};