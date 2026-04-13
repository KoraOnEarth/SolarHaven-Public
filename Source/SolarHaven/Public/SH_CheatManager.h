// Copyright Lightline Studios. All rights reserved 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "SH_CheatManager.generated.h"

/**
 * 
 */
UCLASS()
class SOLARHAVEN_API USH_CheatManager : public UCheatManager
{
	GENERATED_BODY()

public:
	UFUNCTION(Exec)
	void AddItem(const FString& Args);

	UFUNCTION(Exec)
	void ClearInventory();

	UFUNCTION(Exec)
	void ClearInventoryAtSlot(int32 TargetSlot);

	UFUNCTION(Exec)
	void PrintInventoryData(const FString& Args);

	UFUNCTION(Exec)
	void PrintCurrentTime();
};
