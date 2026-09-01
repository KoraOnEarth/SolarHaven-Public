// LightLine Studios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SH_InventoryStructs.h"
#include "Components/ActorComponent.h"

#include "SH_InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, int32 /* UpdatedSlotIndex */);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHCOMPONENTS_API USH_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USH_InventoryComponent();
	
	void Init(int32 SlotsCount);
	int32 GetCountOfSlots() const { return InventorySlots.Num(); };
	
	FOnInventoryChanged OnInventoryChanged;
	
	void AddItem(USH_ItemPDA* ItemToAdd, int32 Quantity);
	void AddItem(const FName& ItemName, int32 Quantity);
	void RemoveItem(USH_ItemPDA* ItemToRemove, int32 Quantity);
	void RemoveItem(const FName& ItemName, int32 Quantity);
	void RemoveItemAtSlot(int32 SlotIndex, int32 Quantity = 1);
	void ClearInventory();
	
	
	int32 GetInventorySlotsCount() const { return InventorySlots.Num(); };
	FInventorySlot* GetSlot(int32 SlotIndex) {return &InventorySlots[SlotIndex];}
	const TArray<FInventorySlot>& GetInventoryContents() { return InventorySlots; };

private:
	UPROPERTY(VisibleInstanceOnly, Category = "Inventory")
	TArray<FInventorySlot> InventorySlots;
	
	void ClearInventoryAtSlot(int32 SlotIndex);
};
