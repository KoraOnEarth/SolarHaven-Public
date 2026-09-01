// LightLine Studios. All rights reserved.


#include "SH_InventoryComponent.h"
#include "Items/SH_ItemLibrary.h"
#include "Items/PDAs/SH_ItemPDA.h"


USH_InventoryComponent::USH_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USH_InventoryComponent::Init(int32 SlotsCount)
{
	if (SlotsCount <= 0 || InventorySlots.Num() == SlotsCount) return;
	InventorySlots.SetNum(SlotsCount, EAllowShrinking::No);
	
	for (int32 i = 0; i < SlotsCount; i++)
	{
		FInventorySlot NewSlot{};
		InventorySlots[i] = NewSlot;
	}
}

void USH_InventoryComponent::AddItem(USH_ItemPDA* ItemToAdd, int32 Quantity)
{
	if (!ItemToAdd || Quantity <= 0) return;
	int32 ItemsRemaining = Quantity;
	
	// Fill existing stacks.
	for (int32 i = 0; i < InventorySlots.Num(); ++i)
	{
		FInventorySlot& Slot = InventorySlots[i];
		if (Slot.Item == ItemToAdd && Slot.Quantity < ItemToAdd->MaxStackSize)
		{
			int32 FreeSpace = ItemToAdd->MaxStackSize - Slot.Quantity;
			int32 QuantityToAdd = FMath::Min(ItemsRemaining, FreeSpace);
			Slot.Quantity += QuantityToAdd;
			ItemsRemaining -= QuantityToAdd;
			
			if (ItemsRemaining <= 0)
			{
				OnInventoryChanged.Broadcast(i);
				return;
			}
		}
	}
	
	// Fill empty slots
	for (int32 i = 0; i < InventorySlots.Num(); ++i)
	{
		FInventorySlot& Slot = InventorySlots[i];
		if (Slot.IsEmpty())
		{
			int32 ItemsToAdd = FMath::Min(ItemsRemaining, ItemToAdd->MaxStackSize);
			
			Slot.Item = ItemToAdd;
			Slot.Quantity = ItemsToAdd;

			ItemsRemaining -= ItemsToAdd;
			
			OnInventoryChanged.Broadcast(i);
			
			if (ItemsRemaining <= 0) { return;}
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("No empty Slots more!"))
}
void USH_InventoryComponent::AddItem(const FName& ItemName, int32 Quantity)
{
	USH_ItemPDA* Item = USH_ItemLibrary::GetItemByID(ItemName);
	if (!Item)
	{
		UE_LOG(LogTemp, Error, TEXT("Item with name: %s does not found"), *ItemName.ToString());
		return;
	}
	AddItem(Item, Quantity);
}
void USH_InventoryComponent::RemoveItem(USH_ItemPDA* ItemToRemove, int32 Quantity)
{
	if (!ItemToRemove || InventorySlots.Num() <= 0 || Quantity <= 0) return;

	int32 ItemsRemaining = Quantity;

	// Remove items from the end of the list rather than from the beginning.
	for (int32 i = InventorySlots.Num() - 1; i >= 0; --i)
	{
		if (InventorySlots[i].Item == ItemToRemove)
		{
			FInventorySlot& Slot = InventorySlots[i];
		
			int32 ItemsToRemove = FMath::Min(ItemsRemaining, Slot.Quantity);
			Slot.Quantity -= ItemsToRemove;
			ItemsRemaining -= ItemsToRemove;
			
			if (Slot.Quantity <= 0) { Slot.Clear(); }
			
			OnInventoryChanged.Broadcast(i);
			
			if (ItemsRemaining == 0) return;
		}
	}
}
void USH_InventoryComponent::RemoveItem(const FName& ItemName, int32 Quantity)
{
	if (InventorySlots.Num() <= 0 || Quantity <= 0) return;
	USH_ItemPDA* Item = USH_ItemLibrary::GetItemByID(ItemName);
	if (!Item)
	{
		UE_LOG(LogTemp, Error, TEXT("Item with name: %s does not found"), *ItemName.ToString());
		return;
	}
	RemoveItem(Item, Quantity);
}
void USH_InventoryComponent::RemoveItemAtSlot(int32 SlotIndex, int32 Quantity)
{
	if (SlotIndex < 0 || SlotIndex >= InventorySlots.Num()) return;
	if (!InventorySlots[SlotIndex].IsEmpty())
	{
		InventorySlots[SlotIndex].Quantity -= Quantity;
		if (InventorySlots[SlotIndex].Quantity <= 0)
		{
			UE_LOG(LogTemp, Log, TEXT("Clearing InventoryAtSlot!"))
			ClearInventoryAtSlot(SlotIndex);
		}
	}
	OnInventoryChanged.Broadcast(SlotIndex);
}
void USH_InventoryComponent::ClearInventory()
{
	if (InventorySlots.Num() <= 0) return;
	for (int32 i = 0; i < InventorySlots.Num(); ++i)
	{
		InventorySlots[i].Clear();
		OnInventoryChanged.Broadcast(i);
	}
}
void USH_InventoryComponent::ClearInventoryAtSlot(int32 SlotIndex)
{
	if (SlotIndex < 0 || SlotIndex >= InventorySlots.Num()) return;
	InventorySlots[SlotIndex].Clear();
}

