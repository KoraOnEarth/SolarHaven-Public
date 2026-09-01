#pragma once

#include "CoreMinimal.h"

#include "SH_GridCell.generated.h"

USTRUCT()
struct FSH_GridCell
{
	GENERATED_BODY()
	
	FSH_GridCell() = default;
	FSH_GridCell(int32 x, int32 y, bool IsOccupied = false) { bIsOccupied = IsOccupied; };
	
	bool bIsOccupied = false;
	
	bool GetIsOccupied() const { return bIsOccupied; }
	void SetOccupation(bool IsOccupied) { bIsOccupied = IsOccupied; }
};