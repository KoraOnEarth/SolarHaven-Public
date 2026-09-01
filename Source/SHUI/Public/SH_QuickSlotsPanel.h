// LightLine Studios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SH_InventoryStructs.h"

#include "Blueprint/UserWidget.h"
#include "SH_QuickSlotsPanel.generated.h"

class UUniformGridPanel;
/**
 * 
 */
UCLASS()
class SHUI_API USH_QuickSlotsPanel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	int32 PreInit(int32 SlotsCount);
	void UpdateAllItems(const TArray<FInventorySlot>& Items);
	void ChangeActiveItem(int32 NewActiveItemIndex);
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> GridPanel;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> SlotClass;
	UPROPERTY(EditDefaultsOnly)
	int32 SlotsInRow = 10;
	
	UPROPERTY()
	TObjectPtr<class USH_Slot> CurrentActiveSlot;
};
