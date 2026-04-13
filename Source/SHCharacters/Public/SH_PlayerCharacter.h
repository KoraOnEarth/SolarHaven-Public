// Copyright Lightline Studios. All rights reserved 

#pragma once

#include "CoreMinimal.h"
#include "SH_InventoryComponent.h"
#include "GameFramework/Character.h"
#include "SH_PlayerCharacter.generated.h"

class USH_ItemBaseBehavior;
class ISH_InteractableInterface;
class USH_InventoryComponent;

UCLASS()
class SHCHARACTERS_API ASH_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASH_PlayerCharacter();

	USH_InventoryComponent* GetInventoryComponent() { return InventoryComponent; }
	int32 GetInventorySlotCount() { return InventorySlotCount; }
	TSubclassOf<USH_ItemBaseBehavior> GetActiveItemClass() const;
	
	void SetCurrentActiveItemIndex(int32 InNewIndex);
	void CreateActiveItemSpot();
	void ChangeActiveItem(int32 NewActiveItemIndex);

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FirstPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USH_InventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> ItemInHandHolder;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ASH_ItemVisual> ItemInHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USH_ItemBaseBehavior> ItemInHandBehavior;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	int32 InventorySlotCount = 20;

	UPROPERTY(VisibleInstanceOnly)
	int32 CurrentActiveItemIndex;

	// ---------------------
	// Interactions
	// ---------------------
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = true))
	float InteractionDistance = 400.f;

public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void Move(const FVector2D& InVector);
	void Look(const FVector2D& InVector);
	virtual void Jump() override;
	
	void Use(int32 SlotIndex);

private:
	void InventoryComponentUpdated(int32 UpdatedIndex);
};
