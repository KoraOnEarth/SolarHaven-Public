// LightLine Studios. All rights reserved.


#include "SH_Slot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"


void USH_Slot::SetIcon(UTexture2D* Icon)
{
	IconImage->SetColorAndOpacity(FLinearColor::White);
	IconImage->SetBrushFromTexture(Icon);
}
void USH_Slot::SetQuantity(int32 InQuantity)
{
	QuantityText->SetText(FText::AsNumber(InQuantity));
}
void USH_Slot::Clear()
{
	IconImage->SetBrush(FSlateBrush());
	IconImage->SetColorAndOpacity(FLinearColor::Gray);
	QuantityText->SetText(FText::AsNumber(0));
}

void USH_Slot::SetBorderVisibility(bool bIsVisible)
{
	if (bIsVisible) BorderImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	else BorderImage->SetVisibility(ESlateVisibility::Collapsed);
}

void USH_Slot::SetSlotNumber(const int32 InSlotNumber)
{
	SlotNumber = InSlotNumber;
}

