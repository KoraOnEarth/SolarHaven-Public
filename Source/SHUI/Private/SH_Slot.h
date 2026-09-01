// LightLine Studios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SH_Slot.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class SHUI_API USH_Slot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetIcon(UTexture2D* Icon);
	void SetQuantity(int32 InQuantity);
	void Clear();
	
	void SetBorderVisibility(bool bIsVisible);
	
	void SetSlotNumber(const int32 InSlotNumber);
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> IconImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> BorderImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuantityText;
	
	UPROPERTY()
	int32 SlotNumber;
};
