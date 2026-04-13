// Copyright Lightline Studios. All rights reserved 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SH_PlayerController.generated.h"

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

UENUM()
enum class ESH_GameState : uint8
{
	None = 0    		UMETA(DisplayName = "None"),
	Game        		UMETA(DisplayName = "Game"),
	Widget      		UMETA(DisplayName = "Widget"),
};

/**
 * 
 */
UCLASS()
class SOLARHAVEN_API ASH_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASH_PlayerController();

	// ------------------------
	// Inventory Functions
	// ------------------------
	void AddItem(const FName& ItemID, int32 Quantity);
	void ClearInventory();
	void ClearInventoryAtSlot(int32 TargetSlot);
	void PrintInventoryData(int32 TargetSlot = -1);
	int32 GetCurrentActiveItemIndex() const { return CurrentActiveItemIndex; };

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;
	virtual void OnUnPossess() override;

	// ------------------------
	// Inventory
	// ------------------------
	UPROPERTY()
	int32 CurrentActiveItemIndex;

	// ------------------------
	// UI Control
	// ------------------------
	void SwitchUIState(ESH_GameState InState);

	UPROPERTY()
	ESH_GameState CurrentGameState;

private:

#pragma region Input

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void Use();
	void ChangeActiveItem(const FInputActionValue& Value);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> MainMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> UseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ChangeActiveItemAction;
	
#pragma endregion Input
	
};
