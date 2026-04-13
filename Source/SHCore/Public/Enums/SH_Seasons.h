#pragma once

#include "CoreMinimal.h"

UENUM()
enum class ESH_Seasons : uint8
{
	None = 0    UMETA(DisplayName = "None"),
	Spring      UMETA(DisplayName = "Spring"),
	Summer      UMETA(DisplayName = "Summer"),
	Autumn      UMETA(DisplayName = "Autumn"),
	Winter      UMETA(DisplayName = "Winter")
};