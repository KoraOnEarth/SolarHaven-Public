// LightLine Studios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SH_PlayerCharacter.generated.h"

struct FInventorySlot;
class USH_ItemPDA;
class USH_InventoryComponent;

UCLASS()
class SOLARHAVEN_API ASH_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASH_PlayerCharacter();
	
	void CreateActiveItemSpot();
	void ChangeActiveItem(int32 NewActiveItemIndex);
	USH_InventoryComponent* GetInventoryComponent() const { return InventoryComponent; };

protected:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;
	
	void EquipItem(USH_ItemPDA* ItemPDA);
	void UnEquipItem();
	void UpdateInHandVisuals(const FInventorySlot* NewCurrentSlot);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FirstPersonCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USH_InventoryComponent> InventoryComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 InventorySlotsCount = 20;
	
	// The item held by the player character has separate responsibilities for its visual representation and its behavior. 
	// ItemInHand, located in ItemInHandSceneComponent, is responsible for the visual representation. 
	// ItemInHandBehavior is responsible for the item's behavior.
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "ItemsInHand", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> ItemInHandSceneComponent;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "ItemsInHand", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ASH_ItemInHandVisual> ItemInHand;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "ItemsInHand", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USH_BaseItemBehavior> ItemInHandBehavior;
	UPROPERTY(EditDefaultsOnly, Category = "ItemsInHand", meta = (AllowPrivateAccess = "true", ClampMin = "1", ClampMax = "1000"))
	float InteractionDistance = 350.f;
	
public:	
	void Move(const FVector2D& InVector);
	void Look(const FVector2D& InVector);
	virtual void Jump() override;
	void Use(int32 CurrentItemIndex);
	void Interact();
	void RotateBuildPreview(int32 RotateDirection);
};
