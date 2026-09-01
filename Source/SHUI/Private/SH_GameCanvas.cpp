// LightLine Studios. All rights reserved.


#include "SH_GameCanvas.h"

#include "SH_QuickSlotsPanel.h"


void USH_GameCanvas::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void USH_GameCanvas::NativeConstruct()
{
	Super::NativeConstruct();
}

void USH_GameCanvas::NativeDestruct()
{
	Super::NativeDestruct();
}

void USH_GameCanvas::UpdateQuickSlotsPanel(const TArray<FInventorySlot>& Items)
{
	QuickSlotsPanel->UpdateAllItems(Items);
}

void USH_GameCanvas::ChangeActiveItem(int32 NewActiveItemIndex)
{
	QuickSlotsPanel->ChangeActiveItem(NewActiveItemIndex);
}
