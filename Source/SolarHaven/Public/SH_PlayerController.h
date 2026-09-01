// LightLine Studios. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SH_GameState.h"

#include "SH_PlayerController.generated.h"

class USH_ItemPDA;
class USH_InventoryComponent;
class USH_GameCanvas;
enum class ESH_GameState : uint8;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class SOLARHAVEN_API ASH_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Exec)
	void AddItem(const FName& ItemID, int32 Quantity);
	UFUNCTION(Exec)
	void RemoveItem(const FName& ItemID, int32 Quantity);
	UFUNCTION(Exec)
	void RemoveItemAtSlot(int32 SlotIndex, int32 Quantity);
	UFUNCTION(Exec)
	void ClearInventory();
	UFUNCTION(Exec)
	void PrintCurrentTime();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
	UPROPERTY()
	TObjectPtr<USH_InventoryComponent> PlayerInventoryComponent;
	
	USH_ItemPDA* LoadItemAssetByItemID(const FName& ItemID);
	void ChangeActiveItem(int32 NewActiveItemIndex);
	void HandleOnInventoryChanged(int32 UpdatedSlotIndex);
	
	UPROPERTY()
	int32 CurrentActiveItemIndex;
	
	// ---------------
	// UI
	// ---------------
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameWidgetClass;
	UPROPERTY()
	TObjectPtr<USH_GameCanvas> GameCanvas;
	
	void CreateHUD();
	
	// ---------------
	// INPUTS
	// ---------------
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void Use();
	void Interact();
	void ChangeActiveItemByKeyboard(const FInputActionValue& Value);
	void ChangeActiveItemByScroll(const FInputActionValue& Value);
	void RotateBuildPreview(const FInputActionValue& Value);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Main Context", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> MainMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Main Context", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Main Context", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Main Context", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Main Context", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> UseAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Main Context", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Main Context", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ChangeActiveItemIndexByKeyboardAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Main Context", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ChangeActiveItemIndexByScrollAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input | Main Context", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RotateBuildPreviewAction;
	
	// ---------------
	// GAME STATES
	// ---------------
	UPROPERTY()
	ESH_GameState CurrentGameState = ESH_GameState::None;
	
	void SetCurrentGameState(ESH_GameState NewGameState);
};
