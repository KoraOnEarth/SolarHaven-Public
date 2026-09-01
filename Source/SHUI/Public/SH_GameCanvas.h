// LightLine Studios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SH_InventoryStructs.h"

#include "Blueprint/UserWidget.h"
#include "SH_GameCanvas.generated.h"

class USH_QuickSlotsPanel;
/**
 * 
 */
UCLASS()
class SHUI_API USH_GameCanvas : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateQuickSlotsPanel(const TArray<FInventorySlot>& Items);
	void ChangeActiveItem(int32 NewActiveItemIndex);
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USH_QuickSlotsPanel> QuickSlotsPanel;
};
