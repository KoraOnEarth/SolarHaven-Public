// LightLine Studios. All rights reserved.


#include "SH_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SH_GameCanvas.h"
#include "SH_GameTimeSubsystem.h"
#include "SH_InventoryComponent.h"
#include "SH_PlayerCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Items/SH_ItemLibrary.h"


void ASH_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (const ULocalPlayer* LP = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->AddMappingContext(MainMappingContext, 0);
		}
	}
	SetCurrentGameState(ESH_GameState::Game);
	
	ASH_PlayerCharacter* PlayerCharacter = Cast<ASH_PlayerCharacter>(GetPawn());
	if (!PlayerCharacter) return;
	PlayerInventoryComponent = PlayerCharacter->GetInventoryComponent();
	if (!PlayerInventoryComponent) { UE_LOG(LogTemp, Warning, TEXT("No inventory component found!")); return; }
	PlayerInventoryComponent->OnInventoryChanged.AddUObject(this, &ThisClass::HandleOnInventoryChanged);
	
	AddItem("axe", 1);
	AddItem("hoe", 1);
	AddItem("watering_can", 1);
	AddItem("corn_seed", 100);
	AddItem("lettuce_seed", 100);
	AddItem("hydroponic_certificate", 1);
	AddItem("test_cert", 1);
	ChangeActiveItem(0);
	
	GameCanvas->UpdateQuickSlotsPanel(PlayerInventoryComponent->GetInventoryContents());
}
void ASH_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
	EIC->BindAction(UseAction, ETriggerEvent::Started, this, &ThisClass::Use);
	EIC->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::Interact);
	EIC->BindAction(ChangeActiveItemIndexByKeyboardAction, ETriggerEvent::Started, this, &ThisClass::ChangeActiveItemByKeyboard);
	EIC->BindAction(ChangeActiveItemIndexByScrollAction, ETriggerEvent::Started, this, &ThisClass::ChangeActiveItemByScroll);
	EIC->BindAction(RotateBuildPreviewAction, ETriggerEvent::Started, this, &ThisClass::RotateBuildPreview);
}
void ASH_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	CreateHUD();
	
	ASH_PlayerCharacter* PlayerCharacter = Cast<ASH_PlayerCharacter>(InPawn);
	PlayerCharacter->CreateActiveItemSpot();
}
void ASH_PlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ASH_PlayerController::AddItem(const FName& ItemID, int32 Quantity)
{
	USH_ItemPDA* Item = LoadItemAssetByItemID(ItemID);
	if (!Item) return;
	if (PlayerInventoryComponent)
	{
		PlayerInventoryComponent->AddItem(Item, Quantity);
	}
}
void ASH_PlayerController::RemoveItem(const FName& ItemID, int32 Quantity)
{
	USH_ItemPDA* Item = LoadItemAssetByItemID(ItemID);
	if (!Item) return;
	if (PlayerInventoryComponent) PlayerInventoryComponent->RemoveItem(Item, Quantity);
}
void ASH_PlayerController::RemoveItemAtSlot(int32 SlotIndex, int32 Quantity)
{
	if (!PlayerInventoryComponent) return;
	PlayerInventoryComponent->RemoveItemAtSlot(SlotIndex, Quantity);
}
void ASH_PlayerController::ClearInventory()
{
	if (!PlayerInventoryComponent) return;
	PlayerInventoryComponent->ClearInventory();
}
void ASH_PlayerController::PrintCurrentTime()
{
	USH_GameTimeSubsystem* TimeSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USH_GameTimeSubsystem>();
	if (TimeSubsystem)
	{
		TimeSubsystem->PrintCurrentTime();
	}
}

USH_ItemPDA* ASH_PlayerController::LoadItemAssetByItemID(const FName& ItemID)
{
	USH_ItemPDA* Item = USH_ItemLibrary::GetItemByID(ItemID);
	if (!Item)
	{
		UE_LOG(LogTemp, Error, TEXT("No item found to remove"));
		return nullptr;
	}
	return Item;
}

void ASH_PlayerController::CreateHUD()
{
	if (!GameWidgetClass) return;
	GameCanvas = CreateWidget<USH_GameCanvas>(this, GameWidgetClass);
	if (!GameCanvas) return;
	GameCanvas->AddToViewport();
}

void ASH_PlayerController::Move(const FInputActionValue& Value)
{
	if (ASH_PlayerCharacter* PlayerCharacter = Cast<ASH_PlayerCharacter>(GetPawn()))
	{
		const FVector2D& Vector = Value.Get<FVector2D>();
		PlayerCharacter->Move(Vector);
	}
}
void ASH_PlayerController::Look(const FInputActionValue& Value)
{
	if (ASH_PlayerCharacter* PlayerCharacter = Cast<ASH_PlayerCharacter>(GetPawn()))
	{
		const FVector2D& Vector = Value.Get<FVector2D>();
		PlayerCharacter->Look(Vector);
	}
}
void ASH_PlayerController::Jump()
{
	if (ASH_PlayerCharacter* PlayerCharacter = Cast<ASH_PlayerCharacter>(GetPawn())) PlayerCharacter->Jump();
}
void ASH_PlayerController::Use()
{
	if (ASH_PlayerCharacter* PlayerCharacter = Cast<ASH_PlayerCharacter>(GetPawn())) PlayerCharacter->Use(CurrentActiveItemIndex);
}
void ASH_PlayerController::Interact()
{
	if (ASH_PlayerCharacter* PlayerCharacter = Cast<ASH_PlayerCharacter>(GetPawn())) PlayerCharacter->Interact();
}

void ASH_PlayerController::ChangeActiveItemByKeyboard(const FInputActionValue& Value)
{
	const int PressedButtonIndex = FMath::RoundToInt(Value.Get<float>()) - 1 % 10;
	ChangeActiveItem(PressedButtonIndex);
}

void ASH_PlayerController::ChangeActiveItemByScroll(const FInputActionValue& Value)
{
	int Delta = FMath::RoundToInt(Value.Get<float>());
	ChangeActiveItem(CurrentActiveItemIndex + Delta);
}

void ASH_PlayerController::RotateBuildPreview(const FInputActionValue& Value)
{
	// -1 - Left rotation
	//  1 - Right rotation
	int RotateDirection = FMath::RoundToInt(Value.Get<float>());
	ASH_PlayerCharacter* PlayerCharacter = Cast<ASH_PlayerCharacter>(GetPawn());
	if (!PlayerCharacter) return;
	PlayerCharacter->RotateBuildPreview(RotateDirection);
}

void ASH_PlayerController::ChangeActiveItem(int32 NewActiveItemIndex)
{
	ASH_PlayerCharacter* PlayerCharacter = Cast<ASH_PlayerCharacter>(GetPawn());
	if (!PlayerCharacter) return;
	
	int32 PrevActiveItemIndex = CurrentActiveItemIndex;
	CurrentActiveItemIndex = FMath::Clamp(NewActiveItemIndex, 0, PlayerInventoryComponent->GetCountOfSlots() - 1);
	if (PrevActiveItemIndex == CurrentActiveItemIndex) return;
	
	PlayerCharacter->ChangeActiveItem(NewActiveItemIndex);
	if (GameCanvas) GameCanvas->ChangeActiveItem(CurrentActiveItemIndex);
}
void ASH_PlayerController::HandleOnInventoryChanged(int32 UpdatedSlotIndex)
{
	if (UpdatedSlotIndex == CurrentActiveItemIndex)
	{
		ASH_PlayerCharacter* PlayerCharacter = Cast<ASH_PlayerCharacter>(GetPawn());
		if (!PlayerCharacter) return;
		PlayerCharacter->ChangeActiveItem(UpdatedSlotIndex);
	}
	GameCanvas->UpdateQuickSlotsPanel(PlayerInventoryComponent->GetInventoryContents());
}

void ASH_PlayerController::SetCurrentGameState(ESH_GameState NewGameState)
{
	if (CurrentGameState == NewGameState) return;
	CurrentGameState = NewGameState;
	
	switch (CurrentGameState)
	{
		case ESH_GameState::Game:
			{
				bShowMouseCursor = false;
				const FInputModeGameOnly InputMode;
				SetInputMode(InputMode);
				break;
			}
		
		default:
			{
				UE_LOG(LogTemp, Error, TEXT("Strange new state of game to set"));
			}
	}
}
