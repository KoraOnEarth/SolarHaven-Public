// Copyright Lightline Studios. All rights reserved 


#include "SHComponents/Public/SH_InventoryComponent.h"

#include "BaseClasses/SH_ItemPDA.h"

USH_InventoryComponent::USH_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void USH_InventoryComponent::Init(int32 SlotsCount)
{
	if (InventoryContents.Num() == SlotsCount) return;
	if (SlotsCount <= 0) return;
	
	InventoryContents.SetNum(SlotsCount, EAllowShrinking::No);
	
	for (int32 i = 0; i < SlotsCount; i++)
	{
		FInventorySlot NewSlot;
		InventoryContents[i] = NewSlot;
	}
}

void USH_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USH_InventoryComponent::AddItem(USH_ItemPDA* InItem, int32 Quantity)
{
	if (!InItem || Quantity <= 0)
		return;

	UE_LOG(LogTemp, Log, TEXT("Try to add item: %s | Quantity: %d"), 
		*InItem->GetPrimaryAssetId().ToString(), 
		Quantity
	);

	if (Quantity > InItem->MaxStackSize * InventoryContents.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Too much items!"
		                                      "\n-------------------"));
		return;
	}

	int32 ItemRemaining = Quantity;

	// Заполняем существующие стаки
	for (int32 i = 0; i < InventoryContents.Num(); ++i)
	{
		FInventorySlot& Slot = InventoryContents[i];

		if (Slot.Item == InItem && Slot.Quantity < InItem->MaxStackSize)
		{
			int32 FreeSpace = InItem->MaxStackSize - Slot.Quantity;
			int32 ItemsToAdd = FMath::Min(ItemRemaining, FreeSpace);

			Slot.Quantity += ItemsToAdd;
			ItemRemaining -= ItemsToAdd;
			
			OnInventoryChanged.Broadcast(i);

			if (ItemRemaining <= 0)
				return;
		}
	}

	// Заполняем пустые слоты
	for (int32 i = 0; i < InventoryContents.Num(); ++i)
	{
		FInventorySlot& Slot = InventoryContents[i];

		if (Slot.IsEmpty())
		{
			int32 ItemsToAdd = FMath::Min(ItemRemaining, InItem->MaxStackSize);

			Slot.Item = InItem;
			Slot.Quantity = ItemsToAdd;

			ItemRemaining -= ItemsToAdd;
			
			OnInventoryChanged.Broadcast(i);

			if (ItemRemaining <= 0)
				return;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("No empty slots!"));
}

void USH_InventoryComponent::ClearInventory()
{
	for (int32 i = 0; i < InventoryContents.Num(); i++)
	{
		ClearInventoryAtSlot(i);
		OnInventoryChanged.Broadcast(i);
	}
}

void USH_InventoryComponent::ClearInventoryAtSlot(int32 TargetSlot)
{
	InventoryContents[TargetSlot].Clear();
	OnInventoryChanged.Broadcast(TargetSlot);
	UE_LOG(LogTemp, Log, TEXT("InventorySlot %d was cleared"), TargetSlot);
}

void USH_InventoryComponent::PrintInventoryData(int32 TargetSlot)
{
	// TargetSlot == -1 это значение по умолчанию
	if (TargetSlot == -1)
	{
		for (int32 i = 0; i < InventoryContents.Num(); i++)
		{
			PrintInventoryData(i);
		}
		return;
	}
	
	InventoryContents[TargetSlot].PrintSlotContents(TargetSlot);
}

void FInventorySlot::PrintSlotContents(int32 TargetSlot)
{
	FString Suffix = (Item && Quantity == Item->MaxStackSize)
		? TEXT("<< MaxStack!")
		: TEXT("");
	
	UE_LOG(LogTemp, Log, TEXT("[%2d] Item name: %-15s\t Quantity: %3d %s"), 
		TargetSlot, 
		Item ? *Item->ItemID.ToString() : TEXT("None") ,
		Quantity,
		*Suffix
	);
}
