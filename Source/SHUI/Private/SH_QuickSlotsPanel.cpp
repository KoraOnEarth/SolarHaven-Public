// LightLine Studios. All rights reserved.


#include "SH_QuickSlotsPanel.h"

#include "SH_Slot.h"

#include "Components/UniformGridPanel.h"

#include "Items/PDAs/SH_ItemPDA.h"


void USH_QuickSlotsPanel::NativePreConstruct()
{
	Super::NativePreConstruct();
	PreInit(10);
}

int32 USH_QuickSlotsPanel::PreInit(int32 SlotsCount)
{
	for (int32 i = 0; i < SlotsCount; ++i)
	{
		const int32 Rows = i / SlotsInRow;
		const int32 Cols = i % SlotsInRow;
		
		USH_Slot* NewSlot = CreateWidget<USH_Slot>(GetWorld(), SlotClass);
		if (!NewSlot) return 0;
		NewSlot->SetSlotNumber(i);
		GridPanel->AddChildToUniformGrid(NewSlot, Rows, Cols);
	}
	
	return SlotsCount / SlotsInRow;
}

void USH_QuickSlotsPanel::UpdateAllItems(const TArray<FInventorySlot>& Items)
{
	for (int32 i = 0; i < SlotsInRow; ++i)
	{
		UWidget* Child = GridPanel->GetChildAt(i);
		if (USH_Slot* CurrentSlot = Cast<USH_Slot>(Child))
		{
			if (!Items[i].Item)
			{
				CurrentSlot->Clear();
				continue;
			}
			if (Items[i].Quantity <= 0)
			{
				CurrentSlot->Clear();
				continue;
			}
			
			CurrentSlot->SetIcon(Items[i].Item->Icon);
			CurrentSlot->SetQuantity(Items[i].Quantity);
		}
	}
}

void USH_QuickSlotsPanel::ChangeActiveItem(int32 NewActiveItemIndex)
{
	if (CurrentActiveSlot) CurrentActiveSlot->SetBorderVisibility(false);
	
	UWidget* Child = GridPanel->GetChildAt(NewActiveItemIndex);
	if (!Child) return;
	USH_Slot* CurrentSlot = Cast<USH_Slot>(Child);
	if (!CurrentSlot) return;
	CurrentSlot->SetBorderVisibility(true);
	CurrentActiveSlot = CurrentSlot;
}

void USH_QuickSlotsPanel::NativeDestruct()
{
	Super::NativeDestruct();
}


