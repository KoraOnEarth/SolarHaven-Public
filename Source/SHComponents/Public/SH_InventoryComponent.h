// Copyright Lightline Studios. All rights reserved 

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SH_InventoryComponent.generated.h"

class USH_ItemPDA;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryChanged, int32 /* UpdatedIndex */);

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	FInventorySlot() :
		Item(nullptr),
		Quantity(0)
	{	}
	
	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<USH_ItemPDA> Item;

	UPROPERTY(VisibleInstanceOnly)
	int32 Quantity;

	bool IsEmpty() const { return Item == nullptr; }
	void Clear() { Item = nullptr; Quantity = 0; }
	void PrintSlotContents(int32 TargetSlot);
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHCOMPONENTS_API USH_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USH_InventoryComponent();

	void Init(int32 SlotsCount);

	FOnInventoryChanged OnInventoryChanged;

protected:
	virtual void BeginPlay() override;

public:
	void AddItem(USH_ItemPDA* InItem, int32 Quantity);
	void ClearInventory();
	void ClearInventoryAtSlot(int32 TargetSlot);
	void PrintInventoryData(int32 TargetSlot = -1);
	
	FInventorySlot* GetSlot(int32 Slot) {return &InventoryContents[Slot];}

private:
	UPROPERTY(VisibleInstanceOnly, Category = Inventory)
	TArray<FInventorySlot> InventoryContents;
};
